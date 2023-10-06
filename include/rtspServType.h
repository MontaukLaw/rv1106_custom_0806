#ifndef __RTSPSERVTYPE_H
#define __RTSPSERVTYPE_H

#define AVFRAME_BLOCK_SIZE 1024*1024 //最大帧长

#define DEV_TYPE_RV11XX	 0x54531400
#define DEV_TYPE DEV_TYPE_RV11XX

/*------------------------frame struct------------------------------*/
#define CODEC_H264_FRAME_FLAG		0x0
#define CODEC_H265_FRAME_FLAG		0x80

#define CODEC_I_FRAME_TYPE 1 /*I Frame*/
#define CODEC_P_FRAME_TYPE 2 /*P Frame*/
#define CODEC_A_FRAME_TYPE 3 /*Audio Frame*/
#define CODEC_OSD_FRAME_TYPE 4 /* Osd Frame*/

#define CODEC_AI_FRAME_TYPE 6// alarm info frame
#define CODEC_GPS_FRAME_TYPE 7//GPS Frame
#define CODEC_PH_FRAME_TYPE 8//流平滑帧

typedef enum AVPIXELFMT
{
	AV_IMAGE_TYPE_UNKNOW = 0,
	AV_IMAGE_TYPE_GRAY8,
	AV_IMAGE_TYPE_GRAY16,
	AV_IMAGE_TYPE_YUV420P,
	AV_IMAGE_TYPE_NV12,
	AV_IMAGE_TYPE_NV21,
	AV_IMAGE_TYPE_YV12,
	AV_IMAGE_TYPE_FBC2,
	AV_IMAGE_TYPE_FBC0,
	AV_IMAGE_TYPE_YUV422P,
	AV_IMAGE_TYPE_NV16,
	AV_IMAGE_TYPE_NV61,
	AV_IMAGE_TYPE_YV16,
	AV_IMAGE_TYPE_YUYV422,
	AV_IMAGE_TYPE_UYVY422,
	AV_IMAGE_TYPE_RGB332,
	AV_IMAGE_TYPE_RGB565,
	AV_IMAGE_TYPE_BGR565,
	AV_IMAGE_TYPE_RGB888,
	AV_IMAGE_TYPE_BGR888,
	AV_IMAGE_TYPE_ARGB8888,
	AV_IMAGE_TYPE_ABGR8888,
	AV_IMAGE_TYPE_JPEG,
	AV_IMAGE_TYPE_BUTT
}AVPIXELFMT_E;

typedef enum _SNAP_TYPE{
	SNAP_TYPE_CUSTOM = 0,
	SNAP_TYPE_FACE = 1,
	SNAP_TYPE_BUTT
}SNAP_TYPE_E;

//编码类型
typedef enum _VIDEO_ENCODING_TYPE{
	VE_TYPE_H264 = 0,
	VE_TYPE_H265 = 1,
	VE_TYPE_MJPEG = 2,
	VE_TYPE_BUTT
}VE_TYPE_E;

typedef enum _VIDEO_INPUT_FMT_TYPE{
	VF_TYPE_PAL = 0,
	VF_TYPE_NTSC = 1,
	VF_TYPE_BUTT
}VF_TYPE_E;
typedef enum ENCODE_RESOLUTION_
{
	RES_QCIF = 0,  	/*0: QCIF*/
	RES_CIF,  		/*1: CIF */
	RES_2CIF,  		/*2: HD1 */
	RES_D1,  		/*3: D1  */
	RES_QVGA,		/*4: 320*240 */
	RES_VGA,		/*5: 640*480  */
	RES_HD960,		/*6: 1280*960P */
	RES_HD720, 		/*7: 1280*720P  */
	RES_HD1080 = 8,/*8: 1920*1080P */
	RES_XGA, 		/*9: 1024*768  */
	RES_QXGA,		/*10: 2048*1536 */
	RES_2304x1296, /*11:3M 2034*1296*/
	RES_2688x1520,	/*13:4M 2560*1440*/
	RES_5M,			/*14:5M 2592*1944*/
	RES_UHD4K,		/*15:4K 3840*2160*/
	RES_HFD1080P,	/*960*540*/
	RES_QHD1080P,	/*480*270*/
	RES_BUTT
}ENCODE_RESOLUTION_E;

typedef uint8_t  OS_UINT8_t;
typedef uint16_t OS_UINT16_t;
typedef uint32_t OS_UINT32_t;
typedef uint64_t OS_UINT64_t;

typedef int8_t	 OS_INT8_t;
typedef int16_t  OS_INT16_t;
typedef int32_t  OS_INT32_t;
typedef int64_t  OS_INT64_t;
typedef unsigned char uchar_t ;
typedef unsigned short ushort_t;


typedef struct __av_frame_head_t
{
	OS_UINT32_t device_type;
	OS_UINT32_t frame_size;
	OS_UINT32_t frame_no;
	uchar_t video_resolution;
	uchar_t frame_type;     
	uchar_t frame_rate;  
	uchar_t video_standard;         
	OS_UINT32_t sec;
	OS_UINT32_t usec;
	OS_UINT64_t pts;
}av_frame_head_t;

typedef struct _av_frame_t
{
	av_frame_head_t frame_head;
	uchar_t *frame_data;
}av_frame_t;

#endif
