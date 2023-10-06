#ifndef RGAPROC_H_125
#define RGAPROC_H_125

#include <pthread.h>

#include "rk_comm_video.h"
#include <rga/rga.h>

#ifndef CROP_RECT_D
typedef struct CROP_RECT_s {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
} CROP_RECT;
#endif


class RgaProc{
public:

	RgaProc(int src_w, int src_h, PIXEL_FORMAT_E s_format);
	~RgaProc();

	bool rga_proc_covert_rgb(VIDEO_FRAME_INFO_S * frame, CROP_RECT *crop, unsigned char ** outbuf_ptr, bool is_output_rgb888);

	bool rga_proc_scale_mb(VIDEO_FRAME_INFO_S * frame,  int dst_w, int dst_h, void **dst_mb);
	
private:
	im_handle_param_t src_param;
	im_handle_param_t dst_param;
	
	MB_POOL pool;
	MB_BLK dst_mb;	
	
	//static pthread_mutex_t global_lock;

};


#endif


