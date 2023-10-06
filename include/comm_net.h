#ifndef __COMM_NET_H
#define __COMM_NET_H

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_COMM_DATA_LEN 512
#define MAX_COMM_SERIAL_NO_LEN 40
#define MAX_COMM_SN_LEN 128

#define MSG_LOGIN 0xA001 // login

#define MSG_START_PREVIEW 0xA002 // 预览视频
#define MSG_STOP_PREVIEW 0xA003	 // 预览停止

#define MSG_START_JPEG 0xA004 // 开始jpeg传输
#define MSG_STOP_JPEG 0xA005  // 停止jpeg传输

#define MSG_STREAM_DATA 0xB001 // 流媒体数据

#define MSG_CONTROL_DATA 0xA801 // 控制数据

#define MSG_TYPE_HEART 0xA802

#define MSG_TYPE_GET_WIFI_CFG 0xA006
#define MSG_TYPE_SET_WIFI_CFG 0xA007
#define MSG_TYPE_REBOOT 0xA008

#define MSG_START_PUSH_JPEG 0xA009
#define MSG_TYPE_KEY_PAD 0xA00A
#define MSG_TYPE_REQ_IFRAME 0xA00B
#define MSG_START_UPDATE_FILE 0xA00C

#define MSG_TYPE_UPDATE_LOCAL_INFO 0xA00D // 搜索设备
#define MSG_TYPE_GET_VERSION 0xA00E
#define MSG_TYPE_SET_VIDEO_INFO 0xA00F
#define MSG_TYPE_GET_VIDEO_INFO 0xA010
#define MSG_TYPE_SET_DEFAULT_VIDEO_INFO 0xA011

#define MSG_TYPE_SET_RTMP_INFO 0xA012
#define MSG_TYPE_GET_RTMP_INFO 0xA013

#define MSG_TYPE_GET_LIVE_STATUS 0xA014
#define MSG_TYPE_SET_LIVE_STATUS 0xA015

#define MSG_TYPE_GET_MVRNET_INFO 0xA016
#define MSG_TYPE_SET_MVRNET_INFO 0xA017

#define MSG_TYPE_SET_OSDCONF_INFO 0xA018

#define MSG_TYPE_PTZ_CONTROL 0xA019

#define MSG_TYPE_SET_SYS_CTRL 0xA01A // Add by 2021-21-28;
#define MSG_TYPE_GET_SYS_CTRL 0xA01B

#define MSG_TYPE_SET_NETWORK 0xA01C
#define MSG_TYPE_GET_NETWORK 0xA01D

#define MSG_TYPE_SET_SYSTIME 0xA01E
#define MSG_TYPE_GET_SYSTIME 0xA01F

#define MSG_TYPE_SET_ROI 0xA020
#define MSG_TYPE_GET_ROI 0xA021

#define MSG_TYPE_SET_ISP_CONFIG 0xA022
#define MSG_TYPE_GET_ISP_CONFIG 0xA023

#define SYS_CTRL_TYPE_FLIP 0x01 // Add by 2021-12-28
#define SYS_CTRL_TYPE_WSN 0x02
#define SYS_CTRL_TYPE_IRCUT 0x03
#define SYS_CTRL_TYPE_WLED 0x04
#define SYS_CTRL_TYPE_AUTH 0x05
#define SYS_CTRL_TYPE_MAC 0x06
#define SYS_CTRL_TYPE_INFO 0x07

#define ISP_TYPE_EXPOSURE_CONFIG 0x01
#define ISP_TYPE_DAYNIGHT_CONFIG 0x02
#define ISP_TYPE_BLC_CONFIG 0x03
#define ISP_TYPE_WB_CONFIG 0x04
#define ISP_TYPE_NR_CONFIG 0x05
#define ISP_TYPE_FORG_CONFIG 0x06
#define ISP_TYPE_ABC_CONFIG 0x07
#define ISP_TYPE_MIRROR_CONFIG 0x08

#define MSG_TYPE_ALARM 0x01
///////////////// 媒体流类型////////////////////////////
#define TYPE_I_FRAME 0x01 // I帧
#define TYPE_P_FRAME 0x02 // P 帧

#define TYPE_H264_DATA 0x01

#define TYPE_JPEG_DATA 0x02 // jpeg frame

#define TYPE_UPDATE_DATA 0x03 // update frame

///////////////数据分割  ///////////////////////
#define DATA_UNIT_FLAG 0x00	 // 中间数据
#define DATA_START_FLAG 0x01 // 开始标示
#define DATA_STOP_FLAG 0x02	 // 结束标示

	//////////////协议类型/////////////////////////

#define MSG_TYPE_BINARY 0x01
#define MSG_TYPE_JSON 0x02

	struct __stream_ext_t__
	{
		unsigned char data_flag; // 当为视频数据时，指示数据的开始，结束，中间
		unsigned char data_type; // 数据类型， jpeg，h264 ?
		//	uint16_t channel; 			//发送媒体流时，指示通道号
		unsigned char channel;
		unsigned char stream_index;
	};

	typedef struct __stream_ext_t__ stream_ext_t;

	struct __ctrl_ext_t__
	{
		unsigned char total_packet;
		unsigned char curr_packet;
		uint16_t msg_type; // 子类型
	};
	typedef struct __ctrl_ext_t__ ctrl_ext_t;

	struct _msg_head_ex_t_
	{
		unsigned char headFlag; // 协议标示头，1字节，固定 0xff
		unsigned char version;	// 版本号，当前为 1
		uint16_t ack;			// 控制命令时，回应消息
		uint32_t sessionId;		// sessionid，登陆设备后，设备返回唯一的sessionid
		unsigned int seqnum;	// seq号，启动后从0开始加，到达最大后从0开始,同一命令的seq不变，两边各自使用各自的seq
		uint16_t msgId;			// 消息码
		uint16_t result;		// 消息结果
		union
		{
			stream_ext_t stream_ext;
			ctrl_ext_t ctrl_ext;
		} ext;
		uint32_t data_len; // 消息数据长度，不包含消息头的
		int32_t key_type;
		uint32_t data_type; // 当为命令时，后面是二进制数据，还是json数据
	};
	typedef struct _msg_head_ex_t_ msg_head_ex_t;

	struct __frame_head_ex_t
	{
		uint32_t frame_size;
		uint32_t frame_no; // jpeg frame_no is zero
		unsigned char video_resolution;
		unsigned char frame_type; // I,P
		unsigned char frame_rate;
		unsigned char video_standard;
		uint32_t hi_pts; // usec
		uint32_t lo_pts; // usec
	};
	typedef struct __frame_head_ex_t frame_head_ex_t;

	struct __jpeg_head_t__
	{
		int len;		// 文件大小
		char name[128]; // 文件名称
	};
	typedef struct __jpeg_head_t__ jpeg_head_t;

	////////////////////分辨率 /////////////////////////

#define ERRCODE_NO_CHANNEL 0x0001
#define ERRCODE_NO_LIVESTATUS 0x0002

#define MAX_CHANNEL_CONNECT 8 // 每次登录最多有4个媒体通道可以用， 省去通道管理
#define MAX_CLIENT_CONNECT 20 // 最多可以允许4个用户登录
#define MAX_OSD_CONTEXT_LEN 256

	struct __client_addr_t__
	{
		int sockfd;
		struct sockaddr_in sockaddr;
	};
	typedef struct __client_addr_t__ client_addr_t;

#define SLOT_UNUSED 0
#define SLOT_USED 1
#define MAX_SEND_BUFF_LEN 1400
#define MAX_BUFF_LEN 1500
#define MAX_JPEG_FILE_PATH 256

	struct __net_channel_info_t__
	{
		int channel_no;
		int stream_index;
		int session_id; // 对应相同的session id 代表相同的用户登录过来的操作哦
		int data_type;	// 发送数据的类型， 实时流，jpeg，等等
		int send_Iframe_flag;
		int open_flag;
		int sockfd;
		int slot_flag; // 是否此slot 可以被使用
		int send_len;  // 发送buff大小
		unsigned int seqnum;
		net_frame_t frame;
		char *send_buff;
		client_addr_t *client_addr;
	};
	typedef struct __net_channel_info_t__ net_channel_info_t;

	struct __recv_jpeg_info_t
	{
		int open_flag;
		int session_id; // 对应相同的session id 代表相同的用户登录过来的操作哦
		int iJpegTotalLen;
		int iJpegRecvLen;
		int ffd;
		int sockfd;
		int slot_flag; // 是否此slot 可以被使用
		char szJpegName[MAX_JPEG_FILE_PATH];
		char recv_buff[MAX_BUFF_LEN];
		client_addr_t *client_addr;
	};
	typedef struct __recv_jpeg_info_t recv_jpeg_info_t;

	struct __net_client_connect_t__
	{
		int slot_flag; // 有没有没使用呢
		int session_id;
		int msg_flag;
		int sockfd;
		pthread_t msg_thread_tid;
		int alarm_open_flag;
		int alarm_status;
		int seq_num;
		pthread_t alarm_thread_tid;
		int heartbeat_lost;
		client_addr_t *client_addr;
		net_channel_info_t channel_list[MAX_CHANNEL_CONNECT]; // 每个登陆客户所有的通道都要挂载在这里的
		recv_jpeg_info_t jpeg_info_list[MAX_CHANNEL_CONNECT];
	};
	typedef struct __net_client_connect_t__ net_client_connect_t;

	struct __comm_net_server_t__
	{
		int server_port;
		int listen_flag;
		int listen_sockfd;
		pthread_t listen_thread_tid;
		pthread_mutex_t mutex;
		int client_cnt; // 有多少个客户登录了
		int chann_cnt;
		net_client_connect_t client_list[MAX_CLIENT_CONNECT]; // 所有登陆的客户都要挂在这里的
	};
	typedef struct __comm_net_server_t__ comm_net_server_t;

#define MAX_NET_USER_LEN 32
#define MAX_NET_PASSWD_LEN 32
#define RTMP_URL_LEN 128
#define RTMP_STREAMNAME_LEN 128

	typedef struct __login_info_t_
	{
		char szUserID[MAX_NET_USER_LEN];
		char szPassword[MAX_NET_PASSWD_LEN];
	} login_info_t;

	typedef struct __rtmp_info_t_
	{
		int enable;
		char szUrl[RTMP_URL_LEN];
		char szStreamName[RTMP_STREAMNAME_LEN];
	} rtmp_info_t;

	typedef struct __mvr_net_info_t_
	{
		char szUrl[RTMP_URL_LEN];
		char szName[MAX_NET_USER_LEN];
		char szPassword[MAX_NET_PASSWD_LEN];
		int nPort;
	} mvr_net_info_t;

	typedef struct __live_info_t_
	{
		char status;
		char wifi_flag;
		char stream_cnt; // MVR NET 观看人数
		char res;
	} live_info_t;

	struct __local_info_t__
	{
		char local_ip[COMM_ADDRSIZE]; // 连接的本机ip地址,网络字节序
		int local_port;				  // 连接的本机端口
		char mac_addr[COMM_ADDRSIZE]; // 20
		char serial_info[MAX_COMM_SERIAL_NO_LEN];
		int device_type; // 0x
		char auth_flag;	 //
		char led_type;
		char cam_count;
		char res;
	};
	typedef struct __local_info_t__ local_info_t;

	struct __network_info_t__
	{
		int dhcp_flag;
		int wifi_flag;
		char ipaddr[COMM_ADDRSIZE];
		char netmask[COMM_ADDRSIZE];
		char gateway[COMM_ADDRSIZE];
		char first_dns[COMM_ADDRSIZE];
		char second_dns[COMM_ADDRSIZE];
	};
	typedef struct __network_info_t__ network_info_t;

	typedef struct __net_sys_ctrl_t_
	{
		char ctrl_type; //
		char ctrl_val;	//
		char res[3];
		int data_len;
		char data[MAX_COMM_DATA_LEN];
	} net_sys_ctrl_t;

	typedef struct __net_sys_ctrl_flip_t
	{
		int flip;
	} net_sys_ctrl_flip_t;

	typedef struct __net_sys_ctrl_wsn_t
	{
		char sn[MAX_COMM_SN_LEN];
	} net_sys_ctrl_wsn_t;

	typedef struct __net_sys_ctrl_ircut_t
	{
		int reverse; // a--->b or b--->a
	} net_sys_ctrl_ircut_t;

	typedef struct __net_sys_ctrl_led_t
	{
		int led_type;	   // 0: red_led 1:white_led 2:double_led
		int switch_type;   // 0: cds  1:lum
		int ircut_reverse; // a--->b or b--->a
	} net_sys_ctrl_led_t;

	typedef struct __net_sys_ctrl_auth_t
	{
		int auth; //
	} net_sys_ctrl_auth_t;
	typedef struct __comm_net_ctrl_mac_t
	{
		char device_serial_no[32];	  // serial
		char mac_addr[COMM_ADDRSIZE]; // 20
	} net_sys_ctrl_mac_t;

	typedef struct __net_sys_ctrl_info_t
	{
		unsigned int dev_type; /* 设备类型 ,IPC,NVR,DVR,AIBOX,DUALIPC*/
		unsigned int sub_type; /* 设备子类型 用于配置工具配置,球机，枪机，半球 */

		unsigned char channel_num; /*产品的总通道数*/
		unsigned char prober_num;  /*报警输入探头数*/
		unsigned char output_num;  /*输出总数*/
		unsigned char serial_num;  /*串口总数*/

		unsigned char hd_num;		 /* hard disk number */
		unsigned char ptz_serial_no; /* 云台控制使用的固定串口号 */
		unsigned char wifi_enable;
		unsigned char enable_4G;
		unsigned char ad_type[4]; /* imx347,imx415,imx327 ...... */
		unsigned int resolv_capable_m[COMM_MAX_CHANNEL_NUM];
		/*	设备各通道能力级 主码流
		 *  bit0:QCIF bit1:CIF bit2:HD1 bit3:D1 bit4:720p
		 *  bit5:QVGA bit6:VGA bit7:HI_SXGA bit8:XGA bit9:1080i*/
		unsigned int resolv_capable_s[COMM_MAX_CHANNEL_NUM];

		unsigned char sys_analogNum; // 可以接入的模拟通道数量
		unsigned char sys_digitNum;	 // 可以接入的数字通道数量，用于解码器和nvr
		unsigned char ircut_type;	 // 0 : 正向 1:反向
		unsigned char led_type;

		unsigned short maxWidth; /* 解码最大宽*/
		unsigned short maxHeigh; /*解码最大高*/

		char device_serial_no[32];				   /* 产品序列号 */
		char product_type[COMM_VERSION_LEN];	   /* 产品型号 */
		char mac_addr[COMM_ADDRSIZE];			   // 20
		char leave_factory_date[COMM_VERSION_LEN]; /* 出厂日期 */
	} net_sys_ctrl_info_t;

	typedef enum
	{
		PTZ_CMD_STOP = 0,
		PTZ_CMD_MENU_UP,
		PTZ_CMD_MENU_DOWN,
		PTZ_CMD_MENU_LEFT,
		PTZ_CMD_MENU_RIGHT,
		PTZ_CMD_MENU_ENTER, // 5
		PTZ_CMD_MENU_IRIS_PLUS,
		PTZ_CMD_MENU_IRIS_MINUS,
		PTZ_CMD_MENU_FOCUS_PLUS,  // +
		PTZ_CMD_MENU_FOCUS_MINUS, // -
		PTZ_CMD_MENU_ZOOM_PLUS,	  //  10 +
		PTZ_CMD_MENU_ZOOM_MINUS,  // -
		PTZ_CMD_PRESET_SET,
		PTZ_CMD_PRESET_CALL,
		PTZ_CMD_PRESET_DEL,
		PTZ_CMD_NULL // 15
	} PTZ_CMD_E;

	typedef struct __PtzConf_t
	{
		int chann;
		int cmd;
		int param1;
		int param2;
		int speed;
		int reserve[4];
	} PtzConf_t;

	//////// osd  //////
	struct _mvr_osd_info_t__
	{
		int iPointX;
		int iPointY;
		int iContextLen;
		char szContext[MAX_OSD_CONTEXT_LEN];
	};

	typedef struct _mvr_osd_info_t__ mvr_osd_info_t;

	struct _mvr_osd_attr_t__
	{
		int iFontColor;
		int iFontSize;
		int iBgColor;
		int iTrans;
	};

	typedef struct _mvr_osd_attr_t__ mvr_osd_attr_t;

	struct _mvr_osd_config_t
	{
		int iIndex; // 第几个index
		int iOsdEnable;
		int iEnableTime; // 是否使能时间戳
		mvr_osd_attr_t stOsdAttr;
		mvr_osd_info_t stOsdInfo;
	};
	typedef struct _mvr_osd_config_t mvr_osd_config_t;

	struct _net_time_t_
	{
		int year;
		int mon;
		int day;
		int hour;
		int minute;
		int sec;
		int timezone; //    +/- 24*60*60 second
	};
	typedef struct _net_time_t_ net_time_t;

#define ROI_TYPE_LINE 0x01
#define ROI_TYPE_RECT 0x02
#define ROI_TYPE_POLYGON 0x03
#define ROI_RECT_NUM 4
#define ROI_LINE_NUM 4
#define ROI_POLYGON_NUM 4
#define ROI_MAX_VERT 10 // 多边形最多10个顶点
	typedef enum
	{
		DIR_INNER = 0, // 框内有效
		DIR_OUT,	   // 框外有效
		DIR_UP,		   //
		DIR_DOWN,
		DIR_LEFT,
		DIR_RIGHT,
		DIR_LEFT_UP,
		DIR_LEFT_DOWN,
		DIR_RIGHT_UP,
		DIR_RIGHT_DOWN
	} DIR_E;

	struct __roi_rect_t__
	{
		int xpoint;
		int ypoint;
		int nwidth;
		int nHeigh;
		int nDirection; // DIR_INNER，DIR_OUT
	};
	typedef struct __roi_rect_t__ roi_rect_t;

	struct __roi_point_t__
	{
		int xPoint;
		int yPoint;
	};
	typedef struct __roi_point_t__ roi_point_t;

	struct __roi_line_t__
	{
		int nStartXpoint;
		int nStartYPoint;
		int nEndXPoint;
		int nEndYPoint;
		int nDirection; // DIR_UP,DIR_DOWN,DIR_LEFT,DIR_RIGHT,DIR_LEFT_UP,DIR_LEFT_DOWN,DIR_RIGHT_UP,DIR_RIGHT_DOWN
	};
	typedef struct __roi_line_t__ roi_line_t;
	struct __roi_polygon_t__
	{
		int point_num;
		int nDirection; // DIR_INNER，DIR_OUT
		roi_point_t point[ROI_MAX_VERT];
	};
	typedef struct __roi_polygon_t__ roi_polygon_t;

	struct __video_roi_aera_t__
	{
		int rect_num;	 // 矩形个数
		int line_num;	 // 线条数
		int polygon_num; // 多边形
		roi_rect_t rect[ROI_RECT_NUM];
		roi_line_t line[ROI_LINE_NUM];
		roi_polygon_t polygon[ROI_POLYGON_NUM];
	};
	typedef struct __video_roi_aera_t__ video_roi_aera_t;

	struct __video_roi_config_t__
	{
		int chann;
		video_roi_aera_t roi_aera;
	};

	typedef struct __video_roi_config_t__ video_roi_config_t;

	typedef struct __net_time_seg_t
	{
		uchar_t start_hour;
		uchar_t start_minute;
		uchar_t start_second;
		uchar_t unused;
	} net_time_seg_t;

	typedef struct _net_isp_exposure_t
	{
		int iExpMode; // auto   manual   spec
		int iExpvalue;
		int iExpMaxVal;
		int iExpMinVal;
		int iGainMode;
		int iGainValue;
		///////////////////
		int iAeSpeed;
		int iAeSetPoint;	// day target ae
		int iAeSetDayPoint; // night target ae
		int iAeSetPointEn;	// auto target ae
	} net_isp_exposure_t;

	typedef struct _net_isp_dayNight_t
	{
		int iMode; // auto  day night spec
		int iSens; // 0....7
		net_time_seg_t start;
		net_time_seg_t end;
		int iFilterTime;	// 3...10
		int iLedMode;		// red, white/red    white
		int iLedLightValue; // 0...10
	} net_isp_dayNight_t;

	typedef struct __net_isp_blc_t
	{
		int iHdrMode;	  // normal   hdr2
		int iHdrLevel;	  // 0,1,2,3
		int iBlcMode;	  // close,open
		int iBlcValue;	  // 0-100
		int iHlcMode;	  // close open
		int iHlcLevel;	  // 0-100
		int iHlcEncValue; // 0-100
	} net_isp_blc_t;

	typedef struct __net_isp_wbgain_t
	{
		int iRGainValue;  // 0 -100
		int iGRGainValue; // 0-100
		int iGBGainValue; // 0-100
		int iBGainValue;  // 0-100
	} net_isp_wbgain_t;

	typedef struct __net_isp_wbscene_t
	{
		int iSeneMode;
		// RK_AIQ_WBCT_INCANDESCENT = 0,
		// RK_AIQ_WBCT_FLUORESCENT,
		// RK_AIQ_WBCT_WARM_FLUORESCENT,
		// RK_AIQ_WBCT_DAYLIGHT,
		// RK_AIQ_WBCT_CLOUDY_DAYLIGHT,
		// RK_AIQ_WBCT_TWILIGHT,
		// RK_AIQ_WBCT_SHADE
	} net_isp_wbscene_t;

	typedef struct __net_isp_wbcct_t
	{
		int cct;
		int ccri; //?? what mean
	} net_isp_wbcct_t;

	typedef struct __net_isp_wb_t
	{
		int iWbMode; // auto,manul-wbgain,manul-wbscene,manul-wbcct
		net_isp_wbgain_t wbgain;
		net_isp_wbscene_t wbscene;
		net_isp_wbcct_t wbcct;
	} net_isp_wb_t;

	typedef struct __net_isp_nr_t
	{
		int iNrMode;	  // 0 default ,1 timeNR 2 spaceNR, 3 time and space
		int iTimeFilter;  // 0-100
		int iSpaceFilter; // 0-100
	} net_isp_nr_t;

	typedef struct __net_isp_forg_t
	{
		int iForgMode; // close open auto
		int iForgLevel;
	} net_isp_forg_t;

	typedef struct __net_isp_abc_t
	{
		int iAbcMode;
		int iAbcLevel;
	} net_isp_abc_t; // Aberration correction
	typedef struct __net_isp_mirror_t
	{
		int iMirrorMode;
	} net_isp_mirror_t;
	typedef struct __net_isp_config_t
	{
		int chann_no;
		int isp_type; //
		union
		{
			net_isp_exposure_t exposure;
			net_isp_dayNight_t dayNight;
			net_isp_blc_t blc;
			net_isp_wb_t wb;
			net_isp_nr_t nr;
			net_isp_forg_t forg;
			net_isp_abc_t abc;
			net_isp_mirror_t mirror;
		} isp_config;
	} net_isp_config_t;

#define MAX_THREADPOOL_NUM 8
#define MIN_THREADPOOL_NUM 3
#define MAX_THREADPOOL_TASK_NUM 20

#ifdef __cplusplus
}
#endif

#endif
