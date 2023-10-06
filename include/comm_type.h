#ifndef __COMM_TYPE_H
#define __COMM_TYPE_H

// define all struct  type fuction  ......
//
//
#include "rk_type.h"

#define MAX_CAPTURE_LEN 5 * 1024 * 1024
#define COMM_ADDRSIZE 20
#define MAX_CONFILE_PATH 256

#define my_malloc malloc
#define my_free free
#define my_calloc calloc

enum app_exposure_mode
{
	APP_AUTO_EXP_MODE = 0,
	APP_MANUAL_EXP_MODE = 1,
	APP_SPEC_EXP_MODE = 2
};
enum app_ircut_mode
{
	APP_IRCUT_MODE_MANUAL = 0,
	APP_IRCUT_MODE_AUTO
};
enum app_daynight_mode
{
	APP_DAYNIGHT_AUTO_MODE = 0,
	APP_DAYNIGHT_DAY_MODE = 1,
	APP_DAYNIGHT_NIGHT_MODE = 2,
	APP_DAYNIGHT_SPEC_MODE = 3
};
enum app_led_mode
{
	APP_LED_IR_MODE = 0,
	APP_LED_WHITE_MODE = 1,
	APP_LED_DAUL_MODE = 2,
	APP_LED_NONE = 3
};
enum app_hdr_mode
{
	APP_HDR_NORMAL_MODE = 0,
	APP_HDR_HDR2_MODE = 1,
	APP_HDR_HDR3_MODE = 2
};
enum app_blc_mode
{
	APP_BLC_CLOSE_MODE = 0,
	APP_BLC_OPEN_MODE = 1
};
enum app_hlc_mode
{
	APP_HLC_CLOSE_MODE = 0,
	APP_HLC_OPEN_MODE = 1
};
enum app_wb_mode
{
	APP_WB_AUTO_MODE = 0,
	APP_WB_MANUAL_GAIN_MODE = 1,
	APP_WB_MANUAL_SCENE_MODE = 2,
	APP_WB_MANUAL_CT_MODE = 3,
};
enum app_nr_mode
{
	APP_NR_DEFAULT_MODE = 0,
	APP_NR_TF_MODE = 1,
	APP_NR_SF_MODE = 2,
	APP_NR_TSF_MODE = 3
};
enum app_forg_mode
{
	APP_FORG_CLOSE_MODE = 0,
	APP_FORG_OPEN_MODE = 1
};
enum app_abc_mode
{
	APP_ABC_LDHC_MODE = 0,
	APP_ABC_FEC_MODE = 1,
	APP_ABC_CLOSE_MODE = 2
};
enum app_mirror_mode
{
	APP_MIRROR_NORMAL_MODE = 0,
	APP_MIRROR_LR_MODE = 1,
	APP_MIRROR_UD_MODE = 2,
	APP_MIRROR_R90_MODE = 3,
	APP_MIRROR_R270_MODE = 3
};

/*------------------------frame struct------------------------------*/
#define CODEC_H264_FRAME_FLAG 0x0
#define CODEC_H265_FRAME_FLAG 0x80

#define CODEC_I_FRAME_TYPE 1   /*I Frame*/
#define CODEC_P_FRAME_TYPE 2   /*P Frame*/
#define CODEC_A_FRAME_TYPE 3   /*Audio Frame*/
#define CODEC_OSD_FRAME_TYPE 4 /* Osd Frame*/

#define CODEC_AI_FRAME_TYPE 6  // alarm info frame
#define CODEC_GPS_FRAME_TYPE 7 // GPS Frame
#define CODEC_PH_FRAME_TYPE 8  // ��ƽ��֡

#define AVFRAME_BLOCK_SIZE 1024 * 1024 // ���֡��

#define NN_CALLBACK_FLAG_RGB 0x01
#define NN_CALLBACK_FLAG_SCALE 0x01 << 1
#define NN_CALLBACK_FLAG_SYNC 0x01 << 2

#define NN_ALGO_TYPE_HIGHALTITUDEPARABOLIC 0x01
#define NN_ALGO_TYPE_FACE_DETECT (0x01 << 1)

#define FRAME_TYPE(type) ((type)&0x7F)
#define CODEC_TYPE(type) ((type)&0x80)

// RockChip,from0x54532000; 2022-09-13;
// 1126 and 1109 is same，show 1126;
#define DEV_TYPE_RK1126_IPC 0x54532000
#define DEV_TYPE_RK1126_DUALIPC 0x54532100
#define DEV_TYPE_RK1126_AIBOX 0x54532200
#define DEV_TYPE_RK1126_NVR 0x54532300
#define DEV_TYPE_RK1126_DVR 0x54532400

// 3568,3566 show 356X;
#define DEV_TYPE_RK356X_IPC 0x54533000
#define DEV_TYPE_RK356X_AIBOX 0x54533100
#define DEV_TYPE_RK356X_NVR 0x54533200
#define DEV_TYPE_RK356X_DVR 0x54533300

// 1106and 1103 is same，show1106;
#define DEV_TYPE_RK1106_IPC 0x54534000
#define DEV_TYPE_RK1106_DUALIPC 0x54534100

typedef enum __COMM_SENSOR_TYPE
{
	IMX327 = 0,	 // 200W
	IMX335 = 1,	 // 500W
	IMX347 = 2,	 // 400W
	IMX415 = 3,	 // 800W
	SC530AI = 4, // 500W
	SC230AI = 5, // 200W
} COMM_SENSOR_TYPE;
#define DEFAULT_ADCHIP_TYPE IMX327
#define DEFAULT_SUB_TYPE 0 // reserve
#define DEFAULT_DEV_TYPE DEV_TYPE_RK1106_IPC

typedef enum SENSOR_TYPE_S
{
	SENSOR_TYPE_IMX334,
	SENSOR_TYPE_IMX415,
	SENSOR_TYPE_IMX335,
	SENSOR_TYPE_IMX290,
	SENSOR_TYPE_IMX347,
	SENSOR_TYPE_IMX327,
	INPUT_AHD_720P,
	INPUT_AHD_1080P,
} SENSOR_TYPE_E;

typedef enum AVPIXELFMT
{
	AV_IMAGE_TYPE_UNKNOW = 0,
	AV_IMAGE_TYPE_GRAY8,
	AV_IMAGE_TYPE_GRAY16,
	AV_IMAGE_TYPE_YUV420P,
	AV_IMAGE_TYPE_NV12,
	AV_IMAGE_TYPE_NV21,
	AV_IMAGE_TYPE_YV12,
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
	AV_IMAGE_TYPE_RGBA8888,
	AV_IMAGE_TYPE_BGRA8888,
	AV_IMAGE_TYPE_JPEG,
	AV_IMAGE_TYPE_BUTT
} AVPIXELFMT_E;

typedef enum _SNAP_TYPE
{
	SNAP_TYPE_CUSTOM = 0,
	SNAP_TYPE_FACE = 1,
	SNAP_TYPE_BUTT
} SNAP_TYPE_E;

// ��������
typedef enum _VIDEO_ENCODING_TYPE
{
	VE_TYPE_H264 = 0,
	VE_TYPE_H265 = 1,
	VE_TYPE_MJPEG = 2,
	VE_TYPE_BUTT
} VE_TYPE_E;

typedef enum _VIDEO_INPUT_FMT_TYPE
{
	VF_TYPE_PAL = 0,
	VF_TYPE_NTSC = 1,
	VF_TYPE_BUTT
} VF_TYPE_E;
typedef enum ENCODE_RESOLUTION_
{
	RES_QCIF = 0,	/*0: QCIF*/
	RES_CIF,		/*1: CIF */
	RES_2CIF,		/*2: HD1 */
	RES_D1,			/*3: D1  */
	RES_QVGA,		/*4: 320*240 */
	RES_VGA,		/*5: 640*480  */
	RES_HD960,		/*6: 1280*960P */
	RES_HD720,		/*7: 1280*720P  */
	RES_HD1080 = 8, /*8: 1920*1080P */
	RES_XGA,		/*9: 1024*768  */
	RES_QXGA,		/*10: 2048*1536 */
	RES_2304x1296,	/*11:3M 2034*1296*/
	RES_2688x1520,	/*12:4M 2688*1520*/
	RES_5M,			/*13:5M 2592*1944*/
	RES_UHD4K,		/*14:4K 3840*2160*/
	RES_HFD1080P,	/*15 960*540*/
	RES_QHD1080P,	/*16 480*270*/
	RES_2880x1616,	/*17 4.6M */
	RES_4M,			/*18 2560*1440*/
	RES_BUTT
} ENCODE_RESOLUTION_E;

/* ����Ķ����� RK rga�� IM_USAGE ������ͬ����Ҫ�޸�ֵ���� */
typedef enum ROTATE_t
{
	ROT_0,
	ROT_90 = (1 << 0),
	ROT_180 = (1 << 1),
	ROT_270 = (1 << 2),

	FLIP_H = (1 << 3),
	FLIP_V = (1 << 4),
	FLIP_H_V = (1 << 5)

} ROTATE_ANGLE;

typedef enum
{
	AUTOWB_MODE = 0,
	MWB_GAIN_MODE,
	MWB_SCENE_MODE,
	MWB_CCT_MODE
} WBMODE_E;

typedef enum
{
	IRCUT_MODE_NORMAL = 0,
	IRCUT_MODE_RESERVE
} IRCUT_MODE_E;
typedef enum
{
	LIGHT_MODE_IR = 0,
	LIGHT_MODE_WLED,
	LIGHT_MODE_IR_WLED,
	LIGHT_MODE_AUTO
} LIGHT_MODE_E;

typedef enum
{
	DAYNIGHT_MODE_AUTO = 0,
	DAYNIGHT_MODE_DAY,
	DAYNIGHT_MODE_NIGHT,
	DAYNIGHT_MODE_SPEC
} DAYNIGHT_MODE_E;

typedef enum
{
	////////////// master channel
	VENC_CHANN_M1,
	VENC_CHANN_M2,
	VENC_CHANN_M3,
	VENC_CHANN_M4,
	////////////////////slave channel
	VENC_CHANN_S1,
	VENC_CHANN_S2,
	VENC_CHANN_S3,
	VENC_CHANN_S4,
	////////////////////////////capture channel
	VENC_CHANN_C1, //
	VENC_CHANN_C2, //
	VENC_CHANN_C3, //
	VENC_CHANN_C4, //
				   ////////////////////////////// VDA  channel
	VENC_CHANN_VDA1,
	VENC_CHANN_VDA2,
	VENC_CHANN_VDA3,
	VENC_CHANN_VDA4,
	//////////////////////////////////////////
	VENC_CHANN_BUTT, //
} VENC_CHANN_E;

#define MAX_TALK_FRAME_SIZE 1024 // 最大对讲帧�?
#define MAX_TALK_FRAME_COUNT 50	 // 最大对讲帧缓冲�?

#define MAX_ALARM_INFO_COUNT 30
#define ALARM_INFO_LEN 48
#define ALARM_ALL 0
#define ALARM_PROBER 1
#define ALARM_VIDEO_LOSE 2
#define ALARM_VIDEO_MOTION 3
#define ALARM_OSD 4
#define ALARM_DATE 5
#define ALARM_VIDEO_LOSE_STOP 6
#define ALARM_VIDEO_MOTION_STOP 7
#define ALARM_PROBER_STOP 8
#define ALARM_MEM_LIMIT 9
#define ALARM_CPU_LIMIT 10
#define ALARM_ON_LINE 11
#define ALARM_DISK_FULL 12
#define ALARM_DISK_ERR 13
#define ALARM_DISK_ERR_STOP 14
#define ALARM_DISK_THRESHOLD 15
#define ALARM_HD_USAGE 16
#define HANDLE_SNAPJPG 17
#define INSTANT_SNAPJPG 17
#define ALARM_GPS_INFO 20
#define ALARM_RF 21
#define ALARM_RF_STOP 22
#define ALARM_VIDEO_COVER 23
#define ALARM_VIDEO_COVER_STOP 24
// wsx plat error no
#define ALARM_MDUID_ERROR 14
#define ALARM_REG_PUID_ERROR 15
#define ALARM_REG_CMU_ERROR 16
#define ALARM_REG_PUID_USED 17
#define ALARM_RECV_CSGINFO_ERROR 18
#define ALARM_REG_CSG_SUCCESS 19
#define ALARM_REG_MDU_SUCCESS 20
#define ALARM_PPP_INFO 23
#define ALARM_PPP_STATUS 24

#define ENC_TO_NET 0
#define NET_TO_ENC 1

#define MAX_CHANN_NUM 4	 // ipc ���ͨ������Ϊ1
#define MAX_STREAM_NUM 4 // ֧���������� 2��

#define MASTER_STREAM_INDEX 0 // ����������
#define SLAVE_STREAM_INDEX 1  // ����������
#define THIRD_STREAM_INDEX 2

#define CAPTURE_STREAM_INDEX 3 // ��ai������

#define MAX_NAL_NUM 12

#define MAX_NET_FRAME_COUNT 60

#define MAX_OSD_LEN 64

// #define MAX_OSD_NUM			4

#define OSD_INDEX_0 0
#define OSD_INDEX_1 1
#define OSD_INDEX_2 2
#define OSD_INDEX_3 3
#define OSD_INDEX_4 4

#define OSD_TIME_INDEX OSD_INDEX_0	// ��һ��Ϊʱ��osd
#define OSD_CHANN_INDEX OSD_INDEX_1 // �ڶ���Ϊͨ������osd
#define MAX_OBJNUM 15
typedef struct __read_pos_t
{
	int read_begin;
	int read_end;
	int old_frame_no;
	int drop_flag;
	int lock_pos;
} read_pos_t;

// typedef struct __av_frame_head_t
// {
// 	OS_UINT32_t device_type;
// 	OS_UINT32_t frame_size;
// 	OS_UINT32_t frame_no;
// 	uchar_t video_resolution;
// 	uchar_t frame_type;
// 	uchar_t frame_rate;
// 	uchar_t video_standard;
// 	OS_UINT32_t sec;
// 	OS_UINT32_t usec;
// 	OS_UINT64_t pts;
// }av_frame_head_t;

typedef struct __av_frame_head_t
{
	RK_U32 device_type;
	RK_U32 frame_size;
	RK_U32 frame_no;
	RK_U8 video_resolution;
	RK_U8 frame_type;
	RK_U8 frame_rate;
	RK_U8 video_standard;
	RK_U8 sec;
	RK_U8 usec;
	RK_U64 pts;
} av_frame_head_t;

typedef struct _av_frame_t
{
	av_frame_head_t frame_head;
	RK_U8 *frame_data;
} av_frame_t;

typedef struct __net_frame_t
{
	av_frame_head_t frame_head;
	RK_U8 *frame_data;
	int write_flag;
} net_frame_t;

typedef struct __net_stream_t
{
	int open_flag[MAX_STREAM_NUM];
	int write_end[MAX_STREAM_NUM];
	net_frame_t *frame_pool[MAX_STREAM_NUM][MAX_NET_FRAME_COUNT];

	pthread_mutex_t frame_pool_mutex[MAX_STREAM_NUM][MAX_NET_FRAME_COUNT];
	pthread_cond_t cond_var[MAX_STREAM_NUM];
	pthread_mutex_t cond_var_mutex[MAX_STREAM_NUM];
} net_stream_t;

typedef struct __frame_block_data_t
{
	unsigned char *frame_data;
	int pos;
	int ipos;
	int iipos;
	unsigned int bend_time;
	int switchflag;
} frame_block_data_t;

typedef struct _mem_block_t
{
	frame_block_data_t fb;
	struct _mem_block_t *next;
} mem_block_t;

typedef struct __talk_frame_t
{
	av_frame_head_t frame_head;
	unsigned char frame_data[MAX_TALK_FRAME_SIZE];
} talk_frame_t;

typedef struct __talk_stream_t
{
	int open_flag;
	int write_end;
	talk_frame_t *frame_pool[MAX_TALK_FRAME_COUNT];
	pthread_cond_t cond_var;
	pthread_mutex_t cond_var_mutex;
} talk_stream_t;

typedef struct __comm_time_info_t
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
} comm_time_info_t;

typedef struct __alarm_attr_t
{
	int type; // 0:binary  1:json   1:xml
	int len;
	void *attr;
} alarm_attr_t;

typedef struct __alarm_info_t
{
	unsigned int alarm_ack;
	comm_time_info_t tm;
	unsigned int alarm_id;
	unsigned int alarm_channel;
	unsigned int alarm_type;
	char ip[16];
	char alarm_info[ALARM_INFO_LEN];
	alarm_attr_t attr;
} alarm_info_t;

typedef struct __alarm_stream_t
{
	int open_flag;
	int write_end;
	alarm_info_t *alarm_info_pool[MAX_ALARM_INFO_COUNT];
	pthread_mutex_t alarm_pool_mutex[MAX_ALARM_INFO_COUNT];

	pthread_cond_t cond_var;
	pthread_mutex_t cond_var_mutex;
} alarm_stream_t;

/* �������� */
typedef struct __avcodec_video_encode_t
{
	int video_format;		/* ��ʽѡ�� 0:PAL   1:NTSC */
	int resolution;			/* �ֱ���,0:QCIF,1:CIF,2:HD1,3:D1, 5:QVGA, 6:VGA */
	int bitrate_type;		/* λ������,0:CBR,1:VBR */
	int level;				/* ����,200kbit3000kbit */
	int frame_rate;			/* ÿ������ */
	int Iframe_interval;	/* I���� */
	char prefer_frame;		/* ��������*/
	char profile;			// profile���� 0:baseline 1:main 2:high
	char profile_level;		// ����
	char video_encode_type; // 0:h264 1:h265 2:mjpeg
	int Qp;					/* ����ϵ�� */
	int encode_type;
} avcodec_video_encode_t;

/*��Ƶ�������*/
typedef struct __avcodec_audio_encode_t
{
	char sample_rate; /* ������ 0:8k, 1:11.025k, 2:16K, 3:22.050K, 4:24K, 5:32K, 6:44.1K, 7:48K */
	char bit_width;	  /* λ�� 0: 8bit, 1: 16bit */
	char encode_type; /* ���뷽ʽ 0:ԭʼ����, 1:g711a, 2:g711u, 3:ADPCM, 4:g726_16K, 5: g726_24K,
						  6: g726_32K,7 : g726_40K, 8:AMR, 9:AMRDTX, 10:AAC */
	char amr_mode;	  /* bit rate type 0:4.75k,1:5.15k,2:5.90K,3:6.70K,4:7.40k,5:7.95k,6:10.20k,7:12.20k */
	char amr_format;  /* package type 0:MIME(file saving), 1:IF1(wireless), 2:IF2(wireless) */
	char unused[7];
} avcodec_audio_encode_t;

typedef struct avcodec_md_config_s
{
	unsigned char sensitivity; // ������
	unsigned char enable;
	unsigned char guard_flag;
	unsigned short area[12]; // ������ǰ�İ���12x16������
} avcodec_md_config_t;

typedef struct avcodec_md_state_s
{
	int state;
} avcodec_md_state_t;

typedef struct avcodec_color_s
{
	RK_U8 aa;
	RK_U8 rr;
	RK_U8 gg;
	RK_U8 bb;
} avcodec_color_t;

typedef struct avcodec_osd_config_s
{
	int index; // range: 0 -7
	// int fmt;	// only used for time   not used
	int font_size;
	int bg_color_index;
	unsigned int osd_color; // rr gg bb , rr is  at higher bit.
} avcodec_osd_config_t;

typedef struct avcodec_osd_context_s
{
	int index; // range: 0 -7
	int enable;
	int xpoint;
	int ypoint;
	char data[MAX_OSD_LEN];
} avcodec_osd_context_t;

typedef struct avcodec_osd_rect_s
{
	int index; // range: 0-7
	int enable;
	int x;
	int y;
	int w;
	int h;
	int thick;
	unsigned int color;

} avcodec_osd_rect_t;

typedef struct avcodec_osd_line_s
{
	int index; // rk osd range: 0 -7, softosd 0 - MAX_SW_OSD_NUM(30)
	int enable;
	int x;
	int y;
	int x2;
	int y2;
	short line_type; // 0 ---- common , 1, begin arrow, 2, end arrow
	short thick;
	unsigned int color;

} avcodec_osd_line_t;

typedef struct avcodec_osd_graph_s
{
	int index; // range: 0 -7
	int enable;
	int x;
	int y;

	char *file_dir;

} avcodec_osd_graph_t;

typedef struct avcodec_osd_bitmap_s
{
	int index; // range: 0 -30
	int enable;
	unsigned int x;
	unsigned int y;
	unsigned int w; // ע�⣺ ���� ��Ҫ8 ���ض��룬����ײ��ͼ��
	unsigned int h;
	unsigned int color; // dot color

	unsigned char *data; // ����ͼƬ����->��, �ϵ��µ�˳������ݣ� ÿ������һ��bit��0 --- ����ʾ�� 1 --- ��ʾdot_color��

} avcodec_osd_bitmap_t;

typedef struct _data_frame_t__
{
	int nsize;
	int pixfmt;
	int u32Width;
	int u32Height;
	int u32HorStride;
	int u32VerStride;
	RK_U8 *data;
} data_frame_t;

typedef struct __nn_frame_t
{
	int64_t pts_us;
	int frame_no;
	data_frame_t yuv_frame;
} nn_frame_t;

typedef struct comm_rect_t
{
	int left;	///< Most left coordinate
	int top;	///< Most top coordinate
	int right;	///< Most right coordinate
	int bottom; ///< Most bottom coordinate
} comm_rect_t;

typedef struct nn_object_t
{
	int id; ///< Track id  �� 1 ��ʼ�� 0 ������ʾ��
	// int cls_idx;            ///< Class index
	unsigned int cls_idx : 8;
	unsigned int color : 24;
	comm_rect_t box; ///< Object Region
	float score;	 ///< Object confidence score
	void *dots_data; // ��� ��ʾ�ַ����ĵ��� �ɵײ���䴦��
	char name[16];
} nn_object_t;

typedef struct nn_object_array_t
{
	int count;						///< Array Count(0 <= count < 256)
	nn_object_t object[MAX_OBJNUM]; ///< Objects
} nn_object_array_t;

typedef int (*net_stream_cb_fun_t)(int ch, int sub_ch, int codecType, void *pFrame, void *user_data);
typedef int (*nn_stream_cb_fun_t)(int ch, void *pFrame, void *nn_obj, void *user_data);

#endif
