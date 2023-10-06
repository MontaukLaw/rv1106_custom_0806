#ifndef _RKNN_ZERO_COPY_DEMO_POSTPROCESS_H_
#define _RKNN_ZERO_COPY_DEMO_POSTPROCESS_H_

#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include "yolo_detect.h"
#include "comm_type.h"

#include <stdint.h>

#define VECTOR_LIST_LEN 2000
#define LABEL_NALE_TXT_PATH "/data/coco.txt"

    typedef struct
    {
        float data[VECTOR_LIST_LEN];
        int list_len;
    } float_list_t;

    typedef struct
    {
        int data[VECTOR_LIST_LEN];
        int list_len;

    } int_list_t;

    // int post_process(int8_t *input0, int8_t *input1, int8_t *input2, int model_in_h, int model_in_w,
    //                  float conf_threshold, float nms_threshold, float scale_w, float scale_h,
    //                  int_list_t *qnt_zps, float_list_t *qnt_scales,
    //                  detect_result_group_t *group);

    // int post_process(int8_t *input0, int8_t *input1, int8_t *input2, int model_in_h, int model_in_w,
    //                  float conf_threshold, float nms_threshold, float scale_w, float scale_h,
    //                  int_list_t *qnt_zps, float_list_t *qnt_scales,
    //                  nn_object_array_t *group);

    // int post_process(int8_t *input0, int8_t *input1, int8_t *input2, int model_in_h, int model_in_w,
    //                  float conf_threshold, float nms_threshold, float scale_w, float scale_h,
    //                  std::vector<int32_t> &qnt_zps, std::vector<float> &qnt_scales,
    //                  nn_object_array_t *group);

    int post_process(int8_t *input0, int8_t *input1, int8_t *input2, int model_in_h, int model_in_w,
                     float conf_threshold, float nms_threshold, float scale_w, float scale_h,
                     std::vector<int32_t> &qnt_zps, std::vector<float> &qnt_scales);

    // void push_data_in_float_list(float_list_t *list, float data);

    // void push_data_in_int_list(int_list_t *list, int data);

    void deinitPostProcess();

    void init_lables(void);

#ifdef __cplusplus
};
#endif

#endif //_RKNN_ZERO_COPY_DEMO_POSTPROCESS_H_
