#include <stdio.h>
#include "yolo_detect.h"
#include "rk_comm_video.h"
#include "rockiva/rockiva_ba_api.h"
#include "rockiva/rockiva_det_api.h"
#include "rockiva/rockiva_face_api.h"

#define Log(fmt, ...)                                                                                     \
    do                                                                                                    \
    {                                                                                                     \
        fprintf(stdout, "[-DBG-][%s:%s:%d]=>" fmt "\n", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define OUT_CAPTURE_PATH "/data/nfs"
// #define MODEL_DATA_PATH "/data/nfs/iva"
#define MODEL_DATA_PATH "/oem/usr/lib"
#define MAX_IMAGE_BUFFER_SIZE 5
typedef struct
{
    int index;
    int state; // 0:uninitial; 1:unused; 2:using
    RockIvaImage image;
} IvaImageBuf;

typedef struct
{
    RockIvaHandle handle;
    RockIvaInitParam commonParams;
    //    RockIvaDetectTaskParams detParams;
    //    RockIvaBaTaskParams baParams;
    RockIvaFaceTaskParams faceParams;
    //    RockIvaPlateTaskParam plateParams;
    //    IvaImageBuf imageBuf[MAX_IMAGE_BUFFER_SIZE];
    //    int hasImageBufAlloc;
} IvaAppContext;

static RockIvaImage IvaImage;
static IvaAppContext iva_ctx;
// static nn_object_array_t main_ai_result;
static void *pRgaHd = NULL;

void FrameReleaseCallback(const RockIvaReleaseFrames *releaseFrames, void *userdata)
{
#if 0
    for (int i = 0; i < releaseFrames->count; i++) {
        uint32_t frameId = releaseFrames->frameId[i];
        printf("frameId = %d\n",frameId);
    }
#endif
}

void FaceDetResultCallback(const RockIvaFaceDetResult *result, const RockIvaExecuteStatus status, void *userData)
{
    //    printf("FaceDetResultCallback frame %d face num: %d\n", result->frameId, result->objNum);
    if (status == ROCKIVA_SUCCESS)
    {
        int i = 0;
#if 1
        for (i = 0; i < result->objNum; i++)
        {
            printf("box_%d=(%d %d %d %d)\n", i, result->faceInfo[i].faceRect.topLeft.x, result->faceInfo[i].faceRect.topLeft.y,
                   result->faceInfo[i].faceRect.bottomRight.x, result->faceInfo[i].faceRect.bottomRight.y);
        }
#else
#if 1
        pthread_mutex_lock(&g_stResult_lock);
        for (i = 0; i < MAX_NNA_DETECT_NUM; i++)
        {
            g_rmm_info.g_fd_result_list[i].fgHasTrigger = FALSE;
            if (i < result->objNum)
            {
                g_rmm_info.g_fd_result_list[i].x1 = result->faceInfo[i].faceRect.topLeft.x * g_rmm_info.DrawPicWidth / 10000;
                g_rmm_info.g_fd_result_list[i].y1 = result->faceInfo[i].faceRect.topLeft.y * g_rmm_info.DrawPicHight / 10000;
                g_rmm_info.g_fd_result_list[i].x2 = result->faceInfo[i].faceRect.bottomRight.x * g_rmm_info.DrawPicWidth / 10000;
                g_rmm_info.g_fd_result_list[i].y2 = result->faceInfo[i].faceRect.bottomRight.y * g_rmm_info.DrawPicHight / 10000;
                printf("###num=%d, detect_type=%d (1:md,2:human,3:face)##x1=%d, y1=%d, x2=%d, y2=%d\n", result->objNum, AI_DETECT_MODE_FACE,
                       g_rmm_info.g_fd_result_list[i].x1, g_rmm_info.g_fd_result_list[i].y1, g_rmm_info.g_fd_result_list[i].x2, g_rmm_info.g_fd_result_list[i].y2);
            }
        }

        g_rmm_info.g_fd_num = result->objNum;
        g_rmm_info.g_fd_result_list[0].mode = AI_DETECT_MODE_FACE;
        pthread_mutex_unlock(&g_stResult_lock);
#else
        pthread_mutex_lock(&g_stResult_lock);
        main_ai_result.count = result->objNum;
        main_ai_result.object[0].color = 0xffff00;
        for (i = 0; i < main_ai_result.count; i++)
        {
            main_ai_result.object[i].box.left = result->faceInfo[i].faceRect.topLeft.x * g_rmm_info.DrawPicWidth / 10000;
            main_ai_result.object[i].box.top = result->faceInfo[i].faceRect.topLeft.y * g_rmm_info.DrawPicHight / 10000;
            main_ai_result.object[i].box.right = result->faceInfo[i].faceRect.bottomRight.x * g_rmm_info.DrawPicWidth / 10000;
            main_ai_result.object[i].box.bottom = result->faceInfo[i].faceRect.bottomRight.y * g_rmm_info.DrawPicHight / 10000;
            printf("###num=%d, detect_type=%d (1:md,2:human,3:face)##x1=%d, y1=%d, x2=%d, y2=%d\n", result->objNum, AI_DETECT_MODE_FACE,
                   main_ai_result.object[i].box.left, main_ai_result.object[i].box.top, main_ai_result.object[i].box.right, main_ai_result.object[i].box.bottom);

            if (i < 4)
                main_ai_result.object[i].color = 0xff0000;
            else if (i < 6)
                main_ai_result.object[i].color = 0x00ff00; // G
            else if (i < 8)
                main_ai_result.object[i].color = 0x0000ff;
        }
        pthread_mutex_unlock(&g_stResult_lock);
#endif
//        app_set_nn_obj(1, &main_ai_result);
#endif

#if 0
        avcodec_osd_rect_t OsdRectCfg;
        for (i = 0; i < g_rmm_info.g_fd_num; i++) {
            OsdRectCfg.index = i+2;
            OsdRectCfg.enable = 1;
            OsdRectCfg.x = g_rmm_info.g_fd_result_list[i].x1;
            OsdRectCfg.y = g_rmm_info.g_fd_result_list[i].y1;
            OsdRectCfg.w = g_rmm_info.g_fd_result_list[i].x2 - g_rmm_info.g_fd_result_list[i].x1;
            OsdRectCfg.h = g_rmm_info.g_fd_result_list[i].y2 - g_rmm_info.g_fd_result_list[i].y1;
            OsdRectCfg.thick = 2;
            OsdRectCfg.color = g_rmm_info.iRectColor;
            
            app_set_stream_osd_rect(H264_SUB_STREAM, &OsdRectCfg);
        }
#if 1
        for (i = 0; i < g_rmm_info.g_fd_num; i++) {
            OsdRectCfg.index = i+2;
            OsdRectCfg.enable = 1;
            OsdRectCfg.x = g_rmm_info.g_fd_result_list[i].x1*g_rmm_info.MainPicWidth/g_rmm_info.DrawPicWidth;
            OsdRectCfg.y = g_rmm_info.g_fd_result_list[i].y1*g_rmm_info.MainPicHight/g_rmm_info.DrawPicHight;
            OsdRectCfg.w = (g_rmm_info.g_fd_result_list[i].x2 - g_rmm_info.g_fd_result_list[i].x1)*g_rmm_info.MainPicWidth/g_rmm_info.DrawPicWidth;
            OsdRectCfg.h = (g_rmm_info.g_fd_result_list[i].y2 - g_rmm_info.g_fd_result_list[i].y1)*g_rmm_info.MainPicHight/g_rmm_info.DrawPicHight;
            OsdRectCfg.thick = 4;
            OsdRectCfg.color = g_rmm_info.iRectColor;
            
            app_set_stream_osd_rect(H264_MAIN_STREAM, &OsdRectCfg);
        }
#endif
#endif
        // g_rmm_info.fgdetect_rect_change = TRUE;
    }
}

void FaceAnalyseResultCallback(const RockIvaFaceCapResult *result, const RockIvaExecuteStatus status, void *userData)
{
    printf("***************FaceAnalyseResultCallback face id=%d****************\n", result->faceInfo.objId);

    if (status == ROCKIVA_SUCCESS)
    {
        printf("capture face id=%d box=(%d %d %d %d) attribute: gender=%d age=%d eyeGlass=%d mask=%d beard=%d\n",
               result->faceInfo.objId, result->faceInfo.faceRect.topLeft.x, result->faceInfo.faceRect.topLeft.y,
               result->faceInfo.faceRect.bottomRight.x, result->faceInfo.faceRect.bottomRight.y,
               result->faceAnalyseInfo.faceAttr.gender, result->faceAnalyseInfo.faceAttr.age,
               result->faceAnalyseInfo.faceAttr.eyeGlass, result->faceAnalyseInfo.faceAttr.mask,
               result->faceAnalyseInfo.faceAttr.beard);
        char save_path[260];
        memset(save_path, 0, 260);
        snprintf(save_path, 260, "%s/%d_%d_%d_%dx%d_mask%d_face.yuv", OUT_CAPTURE_PATH, result->faceInfo.objId, result->faceInfo.faceQuality.score,
                 result->captureImage.frameId, result->captureImage.info.width, result->captureImage.info.height, result->faceAnalyseInfo.faceAttr.mask);
        // printf("save capture to %s\n", save_path);

#if 0
        RK_U8	*pVirAddr = RK_NULL;
        VIDEO_FRAME_INFO_S   stFrame;
        MB_BLK blk = RK_NULL;
        memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));

        blk = RK_MPI_MB_GetMB(vencPoolInput, u32BufferSize, RK_TRUE);
        pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);

        memcpy(pVirAddr, result->captureImage.dataAddr, u32BufferSize);

        RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);
        stFrame.stVFrame.pMbBlk = blk;
        stFrame.stVFrame.u32Width = result->captureImage.info.width;
        stFrame.stVFrame.u32Height = result->captureImage.info.height;
        stFrame.stVFrame.u32VirWidth = result->captureImage.info.width;
        stFrame.stVFrame.u32VirHeight = result->captureImage.info.height;
        stFrame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
        stFrame.stVFrame.u32FrameFlag |= 0;
        stFrame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
        RK_MPI_VENC_SendFrame(0, &stFrame, -1);
        RK_MPI_MB_ReleaseMB(blk);
#endif
    }
}

int InitIvaCommon(IvaAppContext *ctx)
{
    printf("\n========%s========\n", __func__);
    snprintf(ctx->commonParams.modelPath, ROCKIVA_PATH_LENGTH, MODEL_DATA_PATH);

    RockIvaRetCode ret = ROCKIVA_Init(&ctx->handle, ROCKIVA_MODE_VIDEO, &ctx->commonParams, ctx);
    if (ret != ROCKIVA_RET_SUCCESS)
    {
        printf("ROCKIVA_Init error %d\n", ret);
        return -1;
    }

    ROCKIVA_SetFrameReleaseCallback(ctx->handle, FrameReleaseCallback);

    return 0;
}

static int InitIvaFace(IvaAppContext *ctx)
{
    printf("\n========%s========\n", __func__);
    RockIvaFaceTaskParams *faceParams = &ctx->faceParams;

    faceParams->faceTaskType.faceCaptureEnable = 1;
    faceParams->faceTaskType.faceAttributeEnable = 1;
    faceParams->faceTaskType.faceRecognizeEnable = 0;

    faceParams->faceCaptureRule.optType = ROCKIVA_FACE_OPT_FAST;
    faceParams->faceCaptureRule.faceQualityThrehold = 80;

    //    faceParams->faceCaptureRule.captureImageFlag = 1;
    faceParams->faceCaptureRule.faceCapacity.maxCaptureNum = 3;
    //    faceParams->faceCaptureRule.captureImageInfo.width = 240;
    //    faceParams->faceCaptureRule.captureImageInfo.height = 320;
    //    faceParams->faceCaptureRule.captureImageInfo.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;//ROCKIVA_IMAGE_FORMAT_RGB888;
    //    faceParams->faceCaptureRule.alignWidth = 16;

    //    faceParams->faceCaptureRule.captureExpand.up = 1;
    //    faceParams->faceCaptureRule.captureExpand.down = 1;
    //    faceParams->faceCaptureRule.captureExpand.left = 1;
    //    faceParams->faceCaptureRule.captureExpand.right = 1;

    RockIvaFaceCallback callback;
    callback.detCallback = FaceDetResultCallback;
    callback.analyseCallback = FaceAnalyseResultCallback;
    RockIvaRetCode ret = ROCKIVA_FACE_Init(ctx->handle, &ctx->faceParams, callback);
    if (ret != ROCKIVA_RET_SUCCESS)
    {
        printf("ROCKIVA_FACE_Init error %d\n", ret);
        return -1;
    }

#if 0
    char cVerInfo[128] = {0};
    ret = ROCKIVA_GetVersion(128, cVerInfo);
    if (ret == ROCKIVA_RET_SUCCESS) {
        Log("ROCKIVA_GetVersion=%s", cVerInfo);
    }
#endif
    Log("InitIvaFace ok.");
    return 0;
}

int rmm_process_yuv(void *mb, int w, int h)
{
    MB_BLK pMb = ((VIDEO_FRAME_INFO_S *)mb)->stVFrame.pMbBlk;
    void *data = RK_MPI_MB_Handle2VirAddr(pMb);
    int fd = RK_MPI_MB_Handle2Fd(pMb);
    IvaImage.info.width = w;
    IvaImage.info.height = h;
    IvaImage.info.format = ROCKIVA_IMAGE_FORMAT_RGB888; // ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
    IvaImage.dataPhyAddr = NULL;
    IvaImage.dataAddr = NULL;
    IvaImage.dataFd = fd;
    IvaImage.frameId++;

    ROCKIVA_PushFrame(iva_ctx.handle, &IvaImage, NULL);

    return 0;
}

int rmm_process_rgb(void *buff, int w, int h)
{
    IvaImage.info.width = w;
    IvaImage.info.height = h;
    IvaImage.info.format = ROCKIVA_IMAGE_FORMAT_RGB888; // ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
    IvaImage.dataPhyAddr = NULL;
    IvaImage.dataAddr = buff;
    IvaImage.dataFd = 0;
    IvaImage.frameId++;

    ROCKIVA_PushFrame(iva_ctx.handle, &IvaImage, NULL);

    return 0;
}

void rmm_yuv_handle_init()
{
    //    return;
    int w, h;
    // app_get_resolution(iStreamType, &w, &h, NULL);
    Log("#####rmm_yuv_handle#####w=%d, h=%d", w, h);

    //    IvaAppContext iva_ctx;
    memset(&iva_ctx, 0, sizeof(IvaAppContext));

    int ret = InitIvaCommon(&iva_ctx);
    if (ret != 0)
    {
        Log("InitIvaCommon fail!!");
        return;
    }

    ret = InitIvaFace(&iva_ctx);
    if (ret != 0)
    {
        Log("InitIvaFace fail!!");
        return;
    }

    Log("rmm_get_yuv_cb strat....");
    // app_set_cb_yuv_data(iStreamType, rmm_get_yuv_cb);
}
