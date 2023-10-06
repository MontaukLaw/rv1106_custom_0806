#ifndef CODEC2_PROCESS_H
#define CODEC2_PROCESS_H

#include "rk_comm_video.h"
#include "comm_type.h"
// #include "rk_aiq_tool_api.h"

typedef struct CROP_RECT_s
{
	unsigned int x;
	unsigned int y;
	unsigned int w;
	unsigned int h;
} CROP_RECT;
#define CROP_RECT_D

#define MAX_CAM_NUM 2
typedef struct CAM_HARD_PARA_T
{
	char name[20];
	int resolution;
	int fps;

} CAM_HARD_PARA;

#ifndef AUDIO_CODE_TYPE
#define AUDIO_CODE_TYPE

// for audio
typedef enum AUDIO_CODEC_TYPE_t
{
	AUDIO_CODEC_G711U = 0,
	AUDIO_CODEC_G711A = 8,
	// AUDIO_CODEC_OPUS  = 98,   //not support for now
	ANDUIO_CODEC_PCM = 10,

	AUDIO_CODEC_NOT_SUPPORT = 255
} AUDIO_CODEC_TYPE;

#endif

#ifndef IRCUT_H_
typedef enum IRCUT_OUTPUT_TYPE_T
{
	IRCUT_OUT_ADC,	// 通过adc 检测确认 ircut切换
	IRCUT_OUT_GPIO, // 通过gpio 高低电平 确认ircut切换
	IRCUT_OUT_SW,	// 软光敏的方式，判断亮度确认ircut切换
} IRCUT_OUTPUT_TYPE;
#endif

#define MAX_CAMERA_WIDTH 2688
#define MAX_CAMERA_HEIGHT 1520

#define CAM0_STREAM_MAIN 0
#define CAM0_STREAM_SUB 1
#define CAM0_STREAM_AI 4

#define CAM1_STREAM_MAIN 2
#define CAM1_STREAM_SUB 3
#define CAM1_STREAM_AI 5

#define FONT_BG_COLOR_BLACK 0
#define FONT_BG_COLOR_WHITE 1
#define FONT_BG_COLOR_RED 2
#define FONT_BG_COLOR_GREEN 3
#define FONT_BG_COLOR_BLUE 4

// #define ISP_PC_ADJUST 1

#ifdef __cplusplus
extern "C"
{
#endif

	const char *app_get_sdk_version();

	/**************************** 设置视频VI的 帧率 ***************************/
	// 如果不设置，默认都是25fps; 另外设置的帧率，不能大于摄像头的原始帧率。
	int app_set_cameras_global(int *fps, unsigned int num_p, bool work_mode_is_hdr);

	/****************************** video stream  操作  ***********************/
	int app_initCodec(int stream_index);

	// 此函数不调用，则 vi 与 编码的 分辨率一样。
	int app_set_vi_resolution(int stream_index, ENCODE_RESOLUTION_E resol);

	// start  stream
	int app_startCodec(int stream_index);

	// stop  stream and delete cam hd
	int app_deinitCodec(int stream_index);

	/****************************** 编码 参数 设定 ***********************/
	// 此函数在开始编码前调用。
	// avcodec_video_encode_t 里的 成员为-1 表示不修改。
	int app_set_default_codec_param(int stream_index, avcodec_video_encode_t *pCodec);

	// 此函数在start codec 后调用。
	int app_set_codec_param(int stream_index, avcodec_video_encode_t *pCodec);

	int app_set_Color2Gray(int bColorFlag /* 0: 黑白 1: 彩色*/);

	/***************************** OSD 操作 *****************************/
	// 操作字符显示的
	// bg_palette_table 的长度 为5；如果设置为NULL, 则使用默认的。
	int app_set_osd_cfg(const char *pFontpath, unsigned int *bg_palette_table, unsigned char bg_alpha); // 需要在 start codec 之前设置
	int app_set_stream_osd_config(int stream_index, avcodec_osd_config_t *osd_config);
	int app_get_stream_osd_config(int stream_index, avcodec_osd_config_t *osd_config);
	int app_set_stream_osd_context(int stream_index, avcodec_osd_context_t *osd_context);
	int app_get_stream_osd_context(int stream_index, avcodec_osd_context_t *osd_config);
	int app_set_capture_osd_context(int stream_index, /*int qfactor,*/ avcodec_osd_context_t *osd_context);

	// 其他: 画框， 线， 图
	int app_set_stream_osd_rect(int stream_index, avcodec_osd_rect_t *cfg);
	int app_set_stream_osd_line(int stream_index, avcodec_osd_line_t *cfg);
	int app_set_stream_osd_graph(int stream_index, avcodec_osd_graph_t *cfg);

	// yuv 上 直接画线函数
	int app_set_stream_sw_osd_line(int stream_index, avcodec_osd_line_t *ocfg);

	int app_set_stream_sw_osd_rect(int stream_index, avcodec_osd_rect_t *ocfg);

	int app_set_stream_sw_osd_bitmap(int stream_index, avcodec_osd_bitmap_t *ocfg);

	/*************************** snapshot 操作 ***************************/
	int app_capture_pic(int stream_index, /*int quality,*/ unsigned char *pBuff, int *iBuffLen);
	void *app_capture_get_handle(int stream_index);

	/*************************** 视频数据操作 （输出/输入） ********************************/
	typedef int (*CB_YUV_DATA)(int stream_index, void *mb);
	typedef void (*CB_RGB_DATA)(int stream_index, unsigned char *data, unsigned int w, unsigned int h);
	typedef void (*CB_ENC_DATA)(int stream_index, av_frame_t *frame);

	// 输入 如yuv 的数据修改等
	int app_set_cb_yuv_data(int stream_index, CB_YUV_DATA cb);
	// int app_set_cb_yuv_output(int stream_index, );
	void app_release_cb_data(int stream_index, void *mb);

	int app_set_rgb_convert_ratio(int stream_index, int divisor);
	int app_set_rgb_crop(int stream_index, CROP_RECT *rect);
	int app_set_cb_rgb_data(int stream_index, CB_RGB_DATA cb);

	int app_set_cb_enc_data(int stream_index, CB_ENC_DATA cb);

	/********************** 取得 流 相关参数 *********/
	int app_get_resolution(int stream_index, int *w, int *h, int *fps);
	// 这个函数取得camid， 为isp 函数提供关键参数。
	int app_get_camid(int stream_index);

	int app_get_camera_hardware_para(CAM_HARD_PARA **cams);
	int app_get_camera_num(void);

	int app_get_resol_from_wh(int w, int h);
	bool app_get_wh_from_resol(int res, int *w, int *h);

	/********************** rtsp audio 相关 api *******************************************/
	typedef void (*CB_ENC_AUDIODATA)(int stream_index, av_frame_t *frame);

	int app_set_audio_input(int stream_index, int samplerate, int channels, int fps);
	int app_set_audio_enc(int stream_index, AUDIO_CODEC_TYPE type, int bitrate);
	void app_set_audio_cb(int stream_index, CB_ENC_AUDIODATA cb);
	int app_start_audio(int stream_index);
	int app_stop_release_audio(int stream_index);
	bool app_is_audio_running(void);

/************************** aduio play  api ****************************************/
#include "ao_c_if.h"

	/***************** ai 处理 函数 *********************************/
	typedef int (*CB_AI_DATA)(int stream_index, void *frame_data, void *puser);

	// int app_set_cb_ai_data(int stream_index, CB_AI_DATA cb, void * puser);
	int app_set_ai_process_para(int stream_index, CB_AI_DATA cb, int cb_ratio, void *puser);
	int app_start_ai_process(void);
	int app_set_nn_obj(int stream_index, nn_object_array_t *nn_obj_p);

	int app_get_main_index_from_ai(int ai_index);
	int app_get_sub_index_from_ai(int ai_index);

	int app_covert_nn_coordinate(int src, int dst, nn_object_array_t *nn_obj_p);
	int app_covert_coordinate(int src, int dst, int *x, int *y);

	/**************** ircut 处理 ************************************/
	typedef int (*ircut_poll_callback)(int cam_id, IRCUT_OUTPUT_TYPE type);

	int app_ircut_init(int stream_index, int open_io, int close_io, int adc_num, IRCUT_OUTPUT_TYPE type, ircut_poll_callback cb);
	int app_ircut_refresh(int stream_index); // 对于 有adc 硬件的来感光 的， 可以调用此函数，定时控制 ircut。
	int app_ircut_set_open(int stream_index, int isopen);
	int app_ircut_get_adc(int stream_index, int *adc);
	int app_ircut_get_cds(int stream_index, int *cds);
	int app_ircut_get_luma(int stream_index, int *luma);
	int app_ircut_set_graymode(int stream_index, int bGrayFlag);
	int app_ircut_get_graymode(int stream_index);

	/******************* 以下操作 属于图形转换 之类 ， 晚点再弄 ***************/
	// int app_convert_rgb_ex(unsigned char * data, MB_IMAGE_INFO_S *info, CROP_RECT *crop, int width_dst, int height_dst,unsigned char * outbuf, bool is_output_rgb888);
	// int app_convert_rgb(unsigned char * data, MB_IMAGE_INFO_S *info, CROP_RECT *crop, unsigned char * outbuf, bool is_output_rgb888);
	/*
	void app_set_face_snap_wh(int nWidth,int nHeigh);
	int comm_snap_face_encode(unsigned char *pSrc,int nSrcWidth,int nSrcHeigh,int PixFmt,nn_rect_t *pRect);
	int comm_snap_face_get(char *pBuff,int *iBuffLen);
	int comm_snap_custom_encode(unsigned char *pSrc,int nSrcWidth,int nSrcHeigh,int PixFmt);
	int comm_snap_custom_get(char *pBuff,int *iBuffLen);
	void app_set_custom_snap_wh(int width,int heigh);

	int app_get_picSize(int res,int *nWidth,int *nHeigh);
	*/

#ifdef __cplusplus
};
#endif

#endif
