#include "tde.h"

typedef enum TEST_TDE_OP_TYPE_E
{
    TDE_OP_QUICK_COPY = 0,
    TDE_OP_QUICK_RESIZE,
    TDE_OP_QUICK_FILL,
    TDE_OP_ROTATION,
    TDE_OP_MIRROR,
    TDE_OP_COLOR_KEY
} TEST_TDE_OP_TYPE_E;

static RK_S32 saveCounter = 0;

RK_U32 u32TaskIndex = 0;
TDE_HANDLE hHandle[TDE_MAX_JOB_NUM];
TDE_SURFACE_S pstSrc[TDE_MAX_TASK_NUM];
TDE_RECT_S pstSrcRect[TDE_MAX_TASK_NUM];
TDE_SURFACE_S pstDst[TDE_MAX_TASK_NUM];
TDE_RECT_S pstDstRect[TDE_MAX_TASK_NUM];

void init_tde_ctx_1920x1080_to_640x340(TEST_TDE_CTX_S *ctx)
{
    ctx->stSrcSurface.u32Width = RKNN_VI_WIDTH;
    ctx->stSrcSurface.u32Height = RKNN_VI_HEIGHT;
    ctx->u32SrcVirWidth = RKNN_VI_WIDTH;
    ctx->u32SrcVirHeight = RKNN_VI_HEIGHT;
    ctx->s32SrcCompressMode = COMPRESS_MODE_NONE;

    ctx->stDstSurface.u32Width = MID_TERM_WIDTH;
    ctx->stDstSurface.u32Height = MID_TERM_HEIGHT;

    ctx->s32DstCompressMode = COMPRESS_MODE_NONE;

    ctx->dstFilePath = "/data/tde";
    ctx->s32LoopCount = 1;
    ctx->s32JobNum = 1;
    ctx->s32TaskNum = 1;
    ctx->stSrcSurface.enColorFmt = RK_FMT_YUV420SP;
    ctx->stDstSurface.enColorFmt = RK_FMT_RGB888;

    ctx->s32Operation = TDE_OP_QUICK_RESIZE; // TDE_OP_QUICK_RESIZE;

    ctx->stSrcRect.s32X = 0;
    ctx->stSrcRect.s32Y = 0;

    ctx->stSrcRect.u32Width = RKNN_VI_WIDTH;
    ctx->stSrcRect.u32Height = RKNN_VI_HEIGHT;

    ctx->stDstRect.s32X = 0;
    ctx->stDstRect.s32Y = 0;

    ctx->stDstRect.u32Width = MID_TERM_WIDTH;
    ctx->stDstRect.u32Height = MID_TERM_HEIGHT; // MODLE_WIDTH;

    ctx->s32Color = 0x7F7F7F7F;
    ctx->s32Rotation = 0;
    ctx->s32Mirror = 0;
}

void init_tde_ctx_640x360_to_640x640(TEST_TDE_CTX_S *ctx)
{
    ctx->stSrcSurface.u32Width = MID_TERM_WIDTH;
    ctx->stSrcSurface.u32Height = MID_TERM_HEIGHT;

    ctx->u32SrcVirWidth = MID_TERM_WIDTH;
    ctx->u32SrcVirHeight = MID_TERM_HEIGHT;

    ctx->s32SrcCompressMode = COMPRESS_MODE_NONE;

    ctx->stDstSurface.u32Width = MODLE_WIDTH;
    ctx->stDstSurface.u32Height = MODLE_WIDTH;

    ctx->s32DstCompressMode = COMPRESS_MODE_NONE;

    ctx->dstFilePath = "/data/tde";
    ctx->s32LoopCount = 1;
    ctx->s32JobNum = 1;
    ctx->s32TaskNum = 1;
    ctx->stSrcSurface.enColorFmt = RK_FMT_RGB888;
    ctx->stDstSurface.enColorFmt = RK_FMT_RGB888;

    // operation type. default(0). 0: quick copy. t1: quick resize."
    // 2: quick fill. 3: rotation. 4: mirror and flip."
    // 5: colorkey
    ctx->s32Operation = TDE_OP_QUICK_COPY; // TDE_OP_QUICK_RESIZE;

    ctx->stSrcRect.s32X = 0;
    ctx->stSrcRect.s32Y = 0;

    ctx->stSrcRect.u32Width = MID_TERM_WIDTH;
    ctx->stSrcRect.u32Height = MID_TERM_HEIGHT;

    ctx->stDstRect.s32X = 0;
    ctx->stDstRect.s32Y = (MODLE_WIDTH - MID_TERM_HEIGHT) / 2;

    ctx->stDstRect.u32Width = MODLE_WIDTH;
    ctx->stDstRect.u32Height = MID_TERM_HEIGHT; // MODLE_WIDTH;

    ctx->s32Color = 0x7F7F7F7F;
    ctx->s32Rotation = 0;
    ctx->s32Mirror = 0;
}

RK_S32 TDE_CreateDstFrame(TEST_TDE_PROC_CTX_S *pstCtx)
{
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    VIDEO_FRAME_INFO_S stVideoFrame;

    RK_U32 u32DstWidth = pstCtx->pstDst.u32Width;
    RK_U32 u32DstHeight = pstCtx->pstDst.u32Height;
    RK_U32 u32DstPixelFormat = pstCtx->pstDst.enColorFmt;
    RK_S32 s32DstCompressMode = pstCtx->pstDst.enComprocessMode;

    stPicBufAttr.u32Width = u32DstWidth;
    stPicBufAttr.u32Height = u32DstHeight;
    stPicBufAttr.enPixelFormat = (PIXEL_FORMAT_E)u32DstPixelFormat;
    stPicBufAttr.enCompMode = (COMPRESS_MODE_E)s32DstCompressMode;

    s32Ret = TEST_SYS_CreateVideoFrame(&stPicBufAttr, &stVideoFrame);
    if (s32Ret != RK_SUCCESS)
    {
        goto __FAILED;
    }

    pstCtx->pstDst.pMbBlk = stVideoFrame.stVFrame.pMbBlk;

__FAILED:
    if (s32Ret != RK_SUCCESS)
    {
        RK_MPI_MB_ReleaseMB(stVideoFrame.stVFrame.pMbBlk);
    }
    return s32Ret;
}

TDE_HANDLE TDE_BeginJob()
{
    TDE_HANDLE jobHandle = -1;
    jobHandle = RK_TDE_BeginJob();
    return jobHandle;
}
RK_S32 TDE_EndJob(TDE_HANDLE jobHandle)
{
    RK_S32 s32Ret = RK_SUCCESS;
    s32Ret = RK_TDE_EndJob(jobHandle, RK_FALSE, RK_TRUE, 10);
    if (s32Ret != RK_SUCCESS)
    {
        RK_TDE_CancelJob(jobHandle);
        return RK_FAILURE;
    }
    RK_TDE_WaitForDone(jobHandle);
    return s32Ret;
}

RK_S32 TDE_AddTask(TEST_TDE_PROC_CTX_S *pstCtx, TDE_HANDLE jobHandle)
{
    RK_S32 s32Ret = RK_SUCCESS;

    switch (pstCtx->opType)
    {
    case TDE_OP_QUICK_COPY:
    {
        s32Ret = RK_TDE_QuickCopy(jobHandle,
                                  &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                                  &pstCtx->pstDst, &pstCtx->pstDstRect);
    }
    break;
    case TDE_OP_QUICK_RESIZE:
    {
        s32Ret = RK_TDE_QuickResize(jobHandle,
                                    &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                                    &pstCtx->pstDst, &pstCtx->pstDstRect);
    }
    break;
    case TDE_OP_QUICK_FILL:
    {
        s32Ret = RK_TDE_QuickFill(jobHandle,
                                  &pstCtx->pstDst, &pstCtx->pstDstRect,
                                  pstCtx->fillData);
    }
    break;
    case TDE_OP_ROTATION:
    {
        s32Ret = RK_TDE_Rotate(jobHandle,
                               &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                               &pstCtx->pstDst, &pstCtx->pstDstRect,
                               (ROTATION_E)pstCtx->rotateAngle);
    }
    break;
    case TDE_OP_COLOR_KEY:
    case TDE_OP_MIRROR:
    {
        s32Ret = RK_TDE_Bitblit(jobHandle,
                                &pstCtx->pstDst, &pstCtx->pstDstRect,
                                &pstCtx->pstSrc, &pstCtx->pstSrcRect,
                                &pstCtx->pstDst, &pstCtx->pstDstRect,
                                &pstCtx->stOpt);
    }
    break;
    default:
    {
        RK_LOGE("unknown operation type %d", pstCtx->opType);
        break;
    }
    }
    return s32Ret;
}

RK_S32 TDE_TransSurfaceToVideoFrame(TEST_TDE_PROC_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstFrames)
{
    TDE_SURFACE_S *surface = &pstCtx->pstDst;
    pstFrames->stVFrame.enCompressMode = surface->enComprocessMode;
    pstFrames->stVFrame.pMbBlk = surface->pMbBlk;
    pstFrames->stVFrame.u32Width = surface->u32Width;
    pstFrames->stVFrame.u32Height = surface->u32Height;
    pstFrames->stVFrame.u32VirWidth = surface->u32Width;
    pstFrames->stVFrame.u32VirHeight = surface->u32Height;
    pstFrames->stVFrame.enPixelFormat = surface->enColorFmt;
    return RK_SUCCESS;
}

RK_S32 TDE_ProcessJob(TEST_TDE_PROC_CTX_S *pstCtx, VIDEO_FRAME_INFO_S *pstFrames)
{
    RK_S32 s32Ret = RK_SUCCESS;

    // TDE_LoadSrcFrame(pstCtx);
    pstCtx->pstSrc.pMbBlk = pstFrames->stVFrame.pMbBlk;

    // 建好dst帧
    TDE_CreateDstFrame(pstCtx);

    TDE_HANDLE jobHandle = TDE_BeginJob();
    RK_S32 s32TaskNum = (pstCtx->s32TaskNum == 0) ? 1 : pstCtx->s32TaskNum;

    s32Ret = TDE_AddTask(pstCtx, jobHandle);
    if (s32Ret != RK_SUCCESS)
    {
        goto __FAILED;
    }

    s32Ret = TDE_EndJob(jobHandle);
    if (s32Ret != RK_SUCCESS)
    {
        goto __FAILED;
    }
    if (pstFrames)
    {
        TDE_TransSurfaceToVideoFrame(pstCtx, pstFrames);
    }

__FAILED:
    if (s32Ret != RK_SUCCESS)
    {
        RK_MPI_MB_ReleaseMB(pstCtx->pstSrc.pMbBlk);
        RK_MPI_MB_ReleaseMB(pstCtx->pstDst.pMbBlk);
    }
    return s32Ret;
}

RK_S32 tde_create_dstBlk(TEST_TDE_CTX_S *ctx, MB_BLK *pstDstBlk, RK_U32 *dstDataSize)
{
    RK_S32 s32Ret = RK_SUCCESS;
    PIC_BUF_ATTR_S stPicBufAttr;
    MB_PIC_CAL_S stMbPicCalResult;

    stPicBufAttr.u32Width = ctx->stDstSurface.u32Width;
    stPicBufAttr.u32Height = ctx->stDstSurface.u32Height;
    stPicBufAttr.enPixelFormat = ctx->stDstSurface.enColorFmt;
    stPicBufAttr.enCompMode = COMPRESS_MODE_NONE; //  COMPRESS_MODE_E(ctx->s32DstCompressMode);
    s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
    if (s32Ret != RK_SUCCESS)
    {
        return s32Ret;
    }
    *dstDataSize = stMbPicCalResult.u32MBSize;

    // write_log("dstDataSize:%d \n", *dstDataSize);

    // 申请内存
    s32Ret = RK_MPI_SYS_MmzAlloc(pstDstBlk, RK_NULL, RK_NULL, stMbPicCalResult.u32MBSize);
    if (s32Ret != RK_SUCCESS)
    {
        write_log(">>>>>>>>>>>>>>>>>>>>>>>>> RK_MPI_SYS_MmzAlloc failed :%d \n", s32Ret);
    }

    return s32Ret;
}

RK_S32 tde_fill_dst(TEST_TDE_CTX_S *ctx, TDE_SURFACE_S *pstDstSurface, TDE_RECT_S *pstDstRect)
{
    pstDstSurface->u32Width = ctx->stDstSurface.u32Width;
    pstDstSurface->u32Height = ctx->stDstSurface.u32Height;
    pstDstSurface->enColorFmt = ctx->stDstSurface.enColorFmt;
    pstDstSurface->enComprocessMode = ctx->stDstSurface.enComprocessMode;
    pstDstRect->s32Xpos = ctx->stDstRect.s32X;
    pstDstRect->s32Ypos = ctx->stDstRect.s32Y;
    pstDstRect->u32Width = ctx->stDstRect.u32Width;
    pstDstRect->u32Height = ctx->stDstRect.u32Height;
    return RK_SUCCESS;
}

RK_S32 tde_fill_src(TEST_TDE_CTX_S *ctx, TDE_SURFACE_S *pstSrcSurface, TDE_RECT_S *pstSrcRect)
{
    pstSrcSurface->u32Width = ctx->stSrcSurface.u32Width;
    pstSrcSurface->u32Height = ctx->stSrcSurface.u32Height;
    pstSrcSurface->enColorFmt = ctx->stSrcSurface.enColorFmt;
    pstSrcSurface->enComprocessMode = ctx->stSrcSurface.enComprocessMode;
    pstSrcRect->s32Xpos = ctx->stSrcRect.s32X;
    pstSrcRect->s32Ypos = ctx->stSrcRect.s32Y;
    pstSrcRect->u32Width = ctx->stSrcRect.u32Width;
    pstSrcRect->u32Height = ctx->stSrcRect.u32Height;
    return RK_SUCCESS;
}

RK_S32 tde_quick_copy_resize_rotate_task(TEST_TDE_CTX_S *ctx, TDE_SURFACE_S *pstSrc, TDE_RECT_S *pstSrcRect, TDE_SURFACE_S *pstDst, TDE_RECT_S *pstDstRect)
{
    tde_fill_src(ctx, pstSrc, pstSrcRect);
    tde_fill_dst(ctx, pstDst, pstDstRect);
    return RK_SUCCESS;
}

RK_S32 tde_quick_fill_task(TEST_TDE_CTX_S *ctx, TDE_SURFACE_S *pstSrc, TDE_SURFACE_S *pstDst, TDE_RECT_S *pstDstRect)
{
    tde_fill_dst(ctx, pstDst, pstDstRect);
    memcpy(RK_MPI_MB_Handle2VirAddr(pstDst->pMbBlk), RK_MPI_MB_Handle2VirAddr(pstSrc->pMbBlk), RK_MPI_MB_GetSize(pstSrc->pMbBlk));
    return RK_SUCCESS;
}

RK_S32 tde_add_task(TEST_TDE_CTX_S *ctx, TDE_HANDLE hHandle, TDE_SURFACE_S *pstSrc, TDE_RECT_S *pstSrcRect, TDE_SURFACE_S *pstDst, TDE_RECT_S *pstDstRect)
{
    RK_S32 s32Ret = RK_SUCCESS;
    ROTATION_E enRotateAngle = (ROTATION_E)ctx->s32Rotation;
    RK_U32 u32FillData = ctx->s32Color;
    RK_S32 s32Operation = ctx->s32Operation;
    RK_S32 s32SrcCompressMode = ctx->s32SrcCompressMode;
    TDE_OPT_S stOpt;
    memset(&stOpt, 0, sizeof(TDE_OPT_S));

    switch (s32Operation)
    {
    case TDE_OP_QUICK_COPY:
    {
        s32Ret = tde_quick_copy_resize_rotate_task(ctx, pstSrc, pstSrcRect, pstDst, pstDstRect);
        s32Ret = RK_TDE_QuickCopy(hHandle, pstSrc, pstSrcRect, pstDst, pstDstRect);
    }
    break;
    case TDE_OP_QUICK_RESIZE:
    {
        s32Ret = tde_quick_copy_resize_rotate_task(ctx, pstSrc, pstSrcRect, pstDst, pstDstRect);
        s32Ret = RK_TDE_QuickResize(hHandle, pstSrc, pstSrcRect, pstDst, pstDstRect);
    }
    break;
    case TDE_OP_QUICK_FILL:
    {
        s32Ret = tde_quick_fill_task(ctx, pstSrc, pstDst, pstDstRect);
        s32Ret = RK_TDE_QuickFill(hHandle, pstDst, pstDstRect, u32FillData);
    }
    break;
    case TDE_OP_ROTATION:
    {
        s32Ret = tde_quick_copy_resize_rotate_task(ctx,
                                                   pstSrc, pstSrcRect, pstDst, pstDstRect);
        s32Ret = RK_TDE_Rotate(hHandle,
                               pstSrc, pstSrcRect, pstDst, pstDstRect,
                               enRotateAngle);
    }
    break;
    case TDE_OP_COLOR_KEY:
    case TDE_OP_MIRROR:
        break;
    default:
    {
        RK_LOGE("unknown operation type %d", ctx->s32Operation);
        break;
    }
    }
    if (s32Ret != RK_SUCCESS)
    {
        RK_TDE_CancelJob(hHandle);
        return RK_FAILURE;
    }
    return s32Ret;
}

RK_S32 single_tde_job(TEST_TDE_CTX_S *ctx, MB_BLK srcBlk, MB_BLK *dstBlk)
{

    // MB_BLK srcBlk = RK_NULL;

    RK_S32 s32Ret = RK_SUCCESS;

    RK_U32 dstDataSize = 0;
    RK_VOID *phyAddr = NULL;

    // 获取输入数据的物理内存地址
    // srcBlk = pstFrames.stVFrame.pMbBlk;

    RK_MPI_MB_Handle2VirAddr(srcBlk);
    RK_MPI_SYS_MmzFlushCache(srcBlk, RK_FALSE);

    // 建好dst帧
    s32Ret = tde_create_dstBlk(ctx, dstBlk, &dstDataSize);
    if (s32Ret != RK_SUCCESS)
    {
        // write_log(" >>>>>>>>>>>>>>>>>>>>>>>>>> tde_create_dstBlk %d\n", s32Ret);
        goto __FAILED;
    }
    // write_log(" >>>>>>>>>>>>>>>>>>>>>>>>>> tde_create_dstBlk done \n");

    // write_log("dstDataSize:%d \n", dstDataSize);
    // 刷上背景
    phyAddr = RK_MPI_MB_Handle2VirAddr(*dstBlk);
    memset(phyAddr, 0x7F, dstDataSize);

    // write_log(" >>>>>>>>>>>>>>>>>>>>>>>>>> srcBlk: %#X dstBlk : %#X  \n", srcBlk, dstBlk);

    for (RK_S32 u32JobIdx = 0; u32JobIdx < ctx->s32JobNum; u32JobIdx++)
    {
        hHandle[u32JobIdx] = RK_TDE_BeginJob();
        if (RK_ERR_TDE_INVALID_HANDLE == hHandle[u32JobIdx])
        {
            RK_LOGE("start job fail");
            goto __FAILED;
        }
        for (u32TaskIndex = 0; u32TaskIndex < ctx->s32TaskNum; u32TaskIndex++)
        {
            pstSrc[u32TaskIndex].pMbBlk = srcBlk;
            pstDst[u32TaskIndex].pMbBlk = *dstBlk;
            s32Ret = tde_add_task(ctx, hHandle[u32JobIdx],
                                  &pstSrc[u32TaskIndex], &pstSrcRect[u32TaskIndex],
                                  &pstDst[u32TaskIndex], &pstDstRect[u32TaskIndex]);
            if (s32Ret != RK_SUCCESS)
            {
                RK_LOGE("add job fail");
                goto __FAILED;
            }
        }
        // timeout是10ms
        s32Ret = RK_TDE_EndJob(hHandle[u32JobIdx], RK_FALSE, RK_TRUE, 100);
        if (s32Ret != RK_SUCCESS)
        {
            RK_LOGE("enable job fail : 0x%#X", s32Ret);
            RK_TDE_CancelJob(hHandle[u32JobIdx]);
            goto __FAILED;
        }
        RK_TDE_WaitForDone(hHandle[u32JobIdx]);
    }

    // saveCounter++;

    *dstBlk = pstDst[0].pMbBlk;

__FAILED:

    return s32Ret;
}

// int trans_1920x1080_to_640x640(VIDEO_FRAME_INFO_S pstFrames, MB_BLK *dstBlk)
// {

//     RK_S32 s32Ret = RK_SUCCESS;
//     TEST_TDE_CTX_S ctx;

//     init_tde_ctx(&ctx);

//     s32Ret = RK_TDE_Open();
//     if (s32Ret != RK_SUCCESS)
//     {
//         return RK_FAILURE;
//     }

//     single_tde_job(&ctx, pstFrames, dstBlk);

//     RK_TDE_Close();
//     return s32Ret;
// }

void trans_1920x1080_yuv_to_640x640_rgb(VIDEO_FRAME_INFO_S *pstFrames, MB_BLK *dstBlk)
{
    RK_S32 s32Ret = RK_SUCCESS;
    TEST_TDE_CTX_S ctx;

    MB_BLK midBlk = RK_NULL;

    // 第1步转成640x360, resize, 并转成rgb
    init_tde_ctx_1920x1080_to_640x340(&ctx);

    s32Ret = RK_TDE_Open();
    if (s32Ret != RK_SUCCESS)
    {
        return RK_FAILURE;
    }

    single_tde_job(&ctx, pstFrames->stVFrame.pMbBlk, &midBlk);
    RK_TDE_Close();

    // 第2步转成640x640, copy, fill
    init_tde_ctx_640x360_to_640x640(&ctx);

    s32Ret = RK_TDE_Open();
    if (s32Ret != RK_SUCCESS)
    {
        return RK_FAILURE;
    }

    single_tde_job(&ctx, midBlk, dstBlk);
    RK_TDE_Close();

    RK_MPI_SYS_MmzFree(midBlk);

    return s32Ret;
}