#ifndef __TDE_H__
#define __TDE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>
#include "comm_type.h"
#include "rk_comm_video.h"
#include "test/test_comm_tde.h"
#include "test/test_comm_sys.h"
// #include <rga/rga.h>
// #include <rga/im2d.h>

// #include "codec2_process.h"
#include "yolo_detect.h"
#include "test/test_comm_sys.h"
#include "test/test_comm_imgproc.h"

#define RKNN_VI_WIDTH 1920
#define RKNN_VI_HEIGHT 1080
#define MODLE_WIDTH 640
#define MID_TERM_WIDTH 640
#define MID_TERM_HEIGHT 360

#define DETECT_X_START ((MODLE_WIDTH - MID_TERM_HEIGHT) / 2)

    typedef struct
    {
        const char *srcFilePath;
        const char *dstFilePath;
        RK_S32 s32LoopCount;
        RK_S32 s32JobNum;
        RK_S32 s32TaskNum;
        RK_S32 s32Rotation;
        RK_S32 s32Operation;
        TDE_SURFACE_S stSrcSurface;
        TDE_SURFACE_S stDstSurface;
        RECT_S stSrcRect;
        RECT_S stDstRect;
        RK_U32 u32SrcVirWidth;
        RK_U32 u32SrcVirHeight;
        RK_S32 s32SrcCompressMode;
        RK_S32 s32DstCompressMode;
        RK_S32 s32Color;
        RK_S32 s32Mirror;
    } TEST_TDE_CTX_S;

    // typedef struct _TEST_TDE_PROC_CTX
    // {
    //     const char *srcFileName;
    //     const char *dstFileName;
    //     TDE_SURFACE_S pstSrc;
    //     TDE_RECT_S pstSrcRect;
    //     TDE_SURFACE_S pstDst;
    //     TDE_RECT_S pstDstRect;
    //     TDE_OPT_S stOpt;
    //     RK_S32 fillData;
    //     ROTATION_E rotateAngle;
    //     RK_S32 opType;
    //     RK_S32 s32JobNum;
    //     RK_S32 s32TaskNum;
    //     RK_S32 s32ProcessTimes;
    // } TEST_TDE_PROC_CTX_S;

    void init_tde_ctx(TEST_TDE_CTX_S *ctx);

    // RK_S32 single_tde_job(TEST_TDE_CTX_S *ctx, VIDEO_FRAME_INFO_S *pstFrames);
    // RK_S32 single_tde_job(TEST_TDE_CTX_S *ctx, VIDEO_FRAME_INFO_S *pstFrames, RK_VOID *dataPtr640x640);

    int trans_640x360_to_640x640(VIDEO_FRAME_INFO_S pstFrames, MB_BLK *dstBlk);

    void trans_1920x1080_yuv_to_640x640_rgb(VIDEO_FRAME_INFO_S *pstFrames, MB_BLK *dstBlk);

#ifdef __cplusplus
}
#endif

#endif
