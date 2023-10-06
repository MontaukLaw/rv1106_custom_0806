#include <stdio.h>
// #include <cstring>
// #include <cstdlib>
#include "rk_debug.h"
#include "rk_mpi_amix.h"

typedef struct _rkTEST_AMIX_CTX_S
{
    RK_S32 s32DevId;
    const char *psControlName;
    char *psControlValue;
    RK_BOOL bListControls;
    RK_BOOL bListContents;
} TEST_AMIX_CTX_S;

void control_vol(void)
{
    // TEST_AMIX_CTX_S *ctx = reinterpret_cast<TEST_AMIX_CTX_S *>(malloc(sizeof(TEST_AMIX_CTX_S)));
    TEST_AMIX_CTX_S *ctx = (TEST_AMIX_CTX_S *)malloc(sizeof(TEST_AMIX_CTX_S));

    ctx->s32DevId = 0;
    ctx->psControlName = "ADC ALC Left Volume";
    ctx->psControlValue = "31";

    RK_MPI_AMIX_SetControl(ctx->s32DevId, ctx->psControlName, ctx->psControlValue);
    ctx->psControlName = "ADC ALC Right Volume";
    ctx->psControlValue = "31";

    char value[64] = {0};

    RK_MPI_AMIX_ListControls(ctx->s32DevId);
    RK_MPI_AMIX_ListContents(ctx->s32DevId);
}