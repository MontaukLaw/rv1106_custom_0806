#ifndef __COMM_CODEC_H
#define __COMM_CODEC_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CODEC_MAX_TASK			8
#define CODEC_MAX_EVENT		256
#define CODEC_MAX_MQ			2000
#define BASE_CODEC_MODULE		0xF000

#define CODEC_UPDATE_OSD_CONFIG_MSG	0x8001
#define CODEC_UPDATE_OSD_CONTEXT_MSG	0x8002
#define CODEC_START_CAPTURE_MSG	0x8003
#define CODEC_STOP_CAPTURE_MSG	0x8004

COMM_API void comm_app_init();
COMM_API void comm_app_deinit();
COMM_API void *comm_app_get_task_mgr();

COMM_API void comm_codec_init();
COMM_API void comm_codec_deinit();

COMM_API void comm_set_video_stream_callback(net_stream_cb_fun_t cb,void *pUser);
COMM_API int comm_ai_init();
//COMM_API int comm_ai_process(int Chann, void *pFrame, int AlgoType);
COMM_API void comm_set_nn_obj(int nChann,nn_object_array_t *nn_obj_p);

#ifdef __cplusplus
}
#endif

#endif
