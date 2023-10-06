/*
	RGB -> jpg file
 */
#ifndef __SNAPSHOT_H__
#define __SNAPSHOT_H__

#include "rk_mpi_sys.h"
#include <rga/im2d.h>
#include <rga/rga.h>


#ifdef __cplusplus
extern "C" {
#endif
// input format : RK_FORMAT_RGBA_8888, RK_FORMAT_BGR_888, IMAGE_TYPE_RGB888, IMAGE_TYPE_NV12 ...
// channel: 0 ~15, cannot use same value in multi process.
void * snapshot_init(int w, int h, int channel, int format);
void* snapshot_get_osd_hd(void *hd_);
int snapshot_run_once(void *hd_, unsigned char *nv12_data,  char *out_file_path);
int snapshot_run_once_ex(void *hd_, unsigned char *nv12_data,  char *pbuff,int *nBuffLen);
void snapshot_deinit(void *hd_);

#ifdef __cplusplus
}
#endif

#endif
