
#include "yolo_detect.h"
#include "post_process.h"
#include "rknn_api.h"
#include "time.h"

#include <vector>
#include <set>

///////////////////// static /////////////////////
static rknn_list_t *rknn_list_;
static rknn_context ctx = 0;
static rknn_input_output_num io_num;
static rknn_tensor_attr input_attrs[IO_IN_NUMBER];
static rknn_tensor_attr output_attrs[IO_OUT_NUMBER];
const float nms_threshold = NMS_THRESH;
const float box_conf_threshold = BOX_THRESH;


///////////////////// static func//////////////////////

static void dump_tensor_attr(rknn_tensor_attr *attr)
{
    char dims[128] = {0};
    for (int i = 0; i < attr->n_dims; ++i)
    {
        int idx = strlen(dims);
        sprintf(&dims[idx], "%d%s", attr->dims[i], (i == attr->n_dims - 1) ? "" : ", ");
    }
    printf("  index=%d, name=%s, n_dims=%d, dims=[%s], n_elems=%d, size=%d, fmt=%s, type=%s, qnt_type=%s, "
           "zp=%d, scale=%f\n",
           attr->index, attr->name, attr->n_dims, dims, attr->n_elems, attr->size, get_format_string(attr->fmt),
           get_type_string(attr->type), get_qnt_type_string(attr->qnt_type), attr->zp, attr->scale);
}

void create_rknn_list(void)
{
    if (rknn_list_ != NULL)
        return;
    rknn_list_ = (rknn_list_t *)malloc(sizeof(rknn_list_t));
    (rknn_list_)->top = NULL;
    (rknn_list_)->size = 0;
    printf("create rknn_list success\n");
}

// 量化模型的npu输出结果为int8数据类型，后处理要按照int8数据类型处理
// 如下提供了int8排布的NC1HWC2转换成int8的nchw转换代码
int NC1HWC2_int8_to_NCHW_int8(const int8_t *src, int8_t *dst, int *dims, int channel, int h, int w)
{
    int batch = dims[0];
    int C1 = dims[1];
    int C2 = dims[4];
    int hw_src = dims[2] * dims[3];
    int hw_dst = h * w;
    for (int i = 0; i < batch; i++)
    {
        src = src + i * C1 * hw_src * C2;
        dst = dst + i * channel * hw_dst;
        for (int c = 0; c < channel; ++c)
        {
            int plane = c / C2;
            const int8_t *src_c = plane * hw_src * C2 + src;
            int offset = c % C2;
            for (int cur_h = 0; cur_h < h; ++cur_h)
                for (int cur_w = 0; cur_w < w; ++cur_w)
                {
                    int cur_hw = cur_h * w + cur_w;
                    dst[c * hw_dst + cur_h * w + cur_w] = src_c[C2 * cur_hw + offset];
                }
        }
    }

    return 0;
}

int init_model(void)
{

    int ret = rknn_init(&ctx, (void *)"/data/yolov5.rknn", 0, 0, NULL);
    if (ret < 0)
    {
        printf("rknn_init fail! ret=%d\n", ret);
        return -1;
    }

    // Get sdk and driver version
    rknn_sdk_version sdk_ver;
    ret = rknn_query(ctx, RKNN_QUERY_SDK_VERSION, &sdk_ver, sizeof(sdk_ver));
    if (ret != RKNN_SUCC)
    {
        printf("rknn_query fail! ret=%d\n", ret);
        return -1;
    }
    printf("rknn_api/rknnrt version: %s, driver version: %s\n", sdk_ver.api_version, sdk_ver.drv_version);

    // Get Model Input Output Info
    ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
    if (ret != RKNN_SUCC)
    {
        printf("rknn_query fail! ret=%d\n", ret);
        return -1;
    }
    printf("model input num: %d, output num: %d\n", io_num.n_input, io_num.n_output);

    printf("input tensors:\n");

    memset(input_attrs, 0, IO_IN_NUMBER * sizeof(rknn_tensor_attr));
    for (uint32_t i = 0; i < IO_IN_NUMBER; i++)
    {
        input_attrs[i].index = i;
        // query info
        ret = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret < 0)
        {
            printf("rknn_init error! ret=%d\n", ret);
            return -1;
        }
        dump_tensor_attr(&input_attrs[i]);
    }

    printf("output tensors:\n");

    memset(output_attrs, 0, IO_OUT_NUMBER * sizeof(rknn_tensor_attr));
    for (uint32_t i = 0; i < IO_OUT_NUMBER; i++)
    {
        output_attrs[i].index = i;
        // query info
        ret = rknn_query(ctx, RKNN_QUERY_NATIVE_OUTPUT_ATTR, &(output_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC)
        {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        dump_tensor_attr(&output_attrs[i]);
    }

    return 0;
}

// int rknn_detect(unsigned char *input_data, detect_result_group_t *detect_result_group)
// int rknn_detect(unsigned char *input_data, nn_object_array_t *detect_result_group)
int rknn_detect(unsigned char *input_data)
{

    // printf("start detect\n");
    int ret = 0;
    rknn_tensor_type input_type = RKNN_TENSOR_UINT8;
    rknn_tensor_format input_layout = RKNN_TENSOR_NHWC;

    // Create input tensor memory
    rknn_tensor_mem *input_mems[1];
    // default input type is int8 (normalize and quantize need compute in outside)
    // if set uint8, will fuse normalize and quantize to npu
    input_attrs[0].type = input_type;
    // default fmt is NHWC, npu only support NHWC in zero copy mode
    input_attrs[0].fmt = input_layout;

    input_mems[0] = rknn_create_mem(ctx, input_attrs[0].size_with_stride);
    // printf("rknn_create_mem done\n");

    // Copy input data to input tensor memory
    int width = input_attrs[0].dims[2];
    int stride = input_attrs[0].w_stride;

    // printf("input width: %d, stride: %d\n", width, stride);
    // input_mems[0]->virt_addr = input_data;
    // printf("cpdata len: %d\n", width * input_attrs[0].dims[1] * input_attrs[0].dims[3]);

    if (width == stride)
    {
        // printf("memcpy start\n");
        memcpy(input_mems[0]->virt_addr, input_data, width * input_attrs[0].dims[1] * input_attrs[0].dims[3]);
        // printf("memcpy end\n");
    }

    // printf("memcpy done\n");
    // Create output tensor memory
    rknn_tensor_mem *output_mems[IO_OUT_NUMBER];
    for (uint32_t i = 0; i < IO_OUT_NUMBER; ++i)
    {
        output_mems[i] = rknn_create_mem(ctx, output_attrs[i].size_with_stride);
    }
    // printf("rknn_create_mem done\n");

    // Set input tensor memory
    ret = rknn_set_io_mem(ctx, input_mems[0], &input_attrs[0]);
    if (ret < 0)
    {
        printf("rknn_set_io_mem fail! ret=%d\n", ret);
        return -1;
    }

    // Set output tensor memory
    for (uint32_t i = 0; i < IO_OUT_NUMBER; ++i)
    {
        // set output memory and attribute
        ret = rknn_set_io_mem(ctx, output_mems[i], &output_attrs[i]);
        if (ret < 0)
        {
            printf("rknn_set_io_mem fail! ret=%d\n", ret);
            return -1;
        }
    }

    // Run
    // printf("Begin perf ...\n");
    // int64_t start_us = getCurrentTimeUs();
    ret = rknn_run(ctx, NULL);

    if (ret < 0)
    {
        printf("rknn run error %d\n", ret);
        return -1;
    }

    // printf("output origin tensors:\n");
    rknn_tensor_attr orig_output_attrs[io_num.n_output];
    memset(orig_output_attrs, 0, io_num.n_output * sizeof(rknn_tensor_attr));
    for (uint32_t i = 0; i < io_num.n_output; i++)
    {
        orig_output_attrs[i].index = i;
        // query info
        ret = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(orig_output_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC)
        {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        // dump_tensor_attr(&orig_output_attrs[i]);
    }

    int8_t *output_mems_nchw[IO_OUT_NUMBER];
    for (uint32_t i = 0; i < IO_OUT_NUMBER; ++i)
    {
        int size = orig_output_attrs[i].size_with_stride;
        output_mems_nchw[i] = (int8_t *)malloc(size);
    }

    for (uint32_t i = 0; i < IO_OUT_NUMBER; i++)
    {
        int channel = orig_output_attrs[i].dims[1];
        int h = orig_output_attrs[i].n_dims > 2 ? orig_output_attrs[i].dims[2] : 1;
        int w = orig_output_attrs[i].n_dims > 3 ? orig_output_attrs[i].dims[3] : 1;
        int hw = h * w;
        NC1HWC2_int8_to_NCHW_int8((int8_t *)output_mems[i]->virt_addr,
                                  (int8_t *)output_mems_nchw[i],
                                  (int *)output_attrs[i].dims, channel, h, w);
    }

    int model_width = 0;
    int model_height = 0;
    if (input_attrs[0].fmt == RKNN_TENSOR_NCHW)
    {
        // printf("model is NCHW input fmt\n");
        model_width = input_attrs[0].dims[2];
        model_height = input_attrs[0].dims[3];
    }
    else
    {
        // printf("model is NHWC input fmt\n");
        model_width = input_attrs[0].dims[1];
        model_height = input_attrs[0].dims[2];
    }
    // printf("model_width: %d, model_height: %d\n", model_width, model_height);

    // post process
    float scale_w = (float)model_width / IMAGE_INPUT_WIDTH;
    float scale_h = (float)model_height / IMAGE_INPUT_HEIGHT;

    // detect_result_group_t detect_result_group;

    // float_list_t out_scales;
    // int_list_t out_zps;
    // memset(&out_scales, 0, sizeof(float_list_t));
    // memset(&out_zps, 0, sizeof(int_list_t));

    std::vector<float> out_scales;
    std::vector<int32_t> out_zps;

    // std::vector<int32_t> out_zps;
    for (int i = 0; i < io_num.n_output; ++i)
    {
        out_scales.push_back(output_attrs[i].scale);
        out_zps.push_back(output_attrs[i].zp);
        // push_data_in_float_list(&out_scales, output_attrs[i].scale);
        // push_data_in_int_list(&out_zps, output_attrs[i].zp);
    }

    // printf("start pp\n");
    // detect_result_group_t detect_result_group;
    post_process(output_mems_nchw[0], output_mems_nchw[1], output_mems_nchw[2], IMAGE_INPUT_WIDTH, IMAGE_INPUT_HEIGHT,
                 box_conf_threshold, nms_threshold, scale_w, scale_h, out_zps, out_scales);

    // printf("pp end\n");

    // char text[256];
    // for (int i = 0; i < ai_result.count; i++)
    // {
    //     nn_object_t *det_result = &(ai_result.object[i]);
    //     // sprintf(text, "%s %.1f%%", det_result->name, det_result->score * 100);
    //     printf("%d %s @ (%d %d %d %d) %f\n", det_result->cls_idx, det_result->name,
    //            det_result->box.left, det_result->box.top,
    //            det_result->box.right, det_result->box.bottom, det_result->score);
    // }
    // int64_t elapse_us = get_current_time_us() - start_us;
    // printf(">>>>>>>>>>>>>>>One time detect cost = %.2fms, FPS = %.2f\n", elapse_us / 1000.f, 1000.f * 1000.f / elapse_us);

    // Destroy rknn memory
    rknn_destroy_mem(ctx, input_mems[0]);
    for (uint32_t i = 0; i < IO_OUT_NUMBER; ++i)
    {
        rknn_destroy_mem(ctx, output_mems[i]);
        free(output_mems_nchw[i]);
    }
    return 0;
}

void quit_rknn(void)
{
    // destroy
    rknn_destroy(ctx);
}

long int get_current_time_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}