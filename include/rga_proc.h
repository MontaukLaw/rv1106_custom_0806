#ifndef RGA_PROCESS_H_125
#define RGA_PROCESS_H_125

#include <stdbool.h>

#include "rk_comm_video.h"

#ifndef CROP_RECT_D
typedef struct CROP_RECT_s {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
} CROP_RECT;
#endif

#ifdef __cplusplus
extern "C" {
#endif


void * rga_proc_init(int src_w, int src_h, PIXEL_FORMAT_E s_format);

bool rga_proc_covert_rgb(void *hd, VIDEO_FRAME_INFO_S * frame, CROP_RECT *crop, unsigned char ** outbuf_ptr, bool is_output_rgb888);

bool rga_proc_deinit(void *hd);


#ifdef __cplusplus
};
#endif

#endif