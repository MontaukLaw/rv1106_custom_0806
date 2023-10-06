#ifndef RKAI_DEMO_H
#define RKAI_DEMO_H

#ifdef __cplusplus
extern "C"
{
#endif

    int rmm_yuv_handle_init();
    int rmm_process_rgb(void *mb, int w, int h);
    int rmm_process_yuv(void *mb, int w, int h);

#ifdef __cplusplus
};
#endif

#endif