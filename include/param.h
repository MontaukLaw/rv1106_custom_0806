#ifndef __COMM_PARAM_H
#define __COMM_PARAM_H

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MAIN_VER "1.0"
#define MINOR_VER "0.0"
#define HARDWARE_VER "1.0"
#define COMM_VERSION(x, y, z) (((x) << 16) | ((y) << 8) | (z))

	COMM_API void comm_param_set_debug_level(int level);
	COMM_API OS_RESULT_t comm_param_init(char *main_ver, char *minor_ver, char *hardware_ver);
	COMM_API OS_RESULT_t comm_param_get(char *name, char *value, int size);
	COMM_API OS_RESULT_t comm_param_get_int(char *name, int *value);
	COMM_API OS_RESULT_t comm_param_set(char *name, char *value);
	COMM_API OS_RESULT_t comm_param_param_set_int(char *name, int value);
	COMM_API OS_RESULT_t comm_param_save();
	COMM_API OS_RESULT_t comm_param_reset_factory();
	COMM_API OS_RESULT_t comm_param_set_backup(char *back_file);
	COMM_API OS_RESULT_t comm_param_set_default();

	////////////////////////////////////////////////////////////////////////////////////////////

#define COMM_VERSION_LEN 24
#define COMM_MAX_WEEK_DAY 7			/* 一星期最多天数 */
#define COMM_MAX_TIME_SEG_NUM 4		/* 最多时间段数 */
#define COMM_MAX_TITLE_LEN 8		/* 通道标题最大长度 */
#define COMM_MAX_ADDR_SIZE 4		/*IP地址最大长度*/
#define COMM_MAX_MAC_SIZE 20		/*IP地址最大长度*/
#define COMM_MAX_OVERLAY_NUM 16		/*视频遮挡最大数*/
#define COMM_MAX_CHANNEL_NUM 4		/*设备名最大长度*/
#define COMM_MAX_DEVICE_NAME_LEN 24 /*设备名最大长度*/
#define COMM_MAX_USER_NUM 24		/* 最多用户数 */
#define COMM_MAX_USER_NAME_LEN 24	/* 用户名的最大长度,实际使用长度为16.*/
#define COMM_MAX_PASSWD_LEN 24		/* 用户密码最大长度,实际使用长度为8.*/
#define COMM_MAX_CHANN_NUM 64		/* 最大64个通道的*/

#define DEFAULT_AD_SHARPNESS 0x80
#define DEFAULT_AD_BRIGHTNESS 0x80
#define DEFAULT_AD_CONTRAST 0x80
#define DEFAULT_AD_HUE 0x80
#define DEFAULT_AD_SATURATION 0x80
#define DEFAULT_ENCODE_FORMATE 0			 // PAL
#define DEFAULT_ENCODE_RESOLUTION RES_HD1080 // D1
#define DEFAULT_SLAVE_ENCODE_RESOLUTION RES_D1
#define DEFAULT_ENCODE_BITRATE_TYPE 0	 // CBR
#define DEFAULT_ENCODE_LEVEL 512		 // 2000	//kbps
#define DEFAULT_ENCODE_LEVEL_720P 1000	 // 4000	//kbps
#define DEFAULT_ENCODE_LEVEL_1080P 2000	 // kbps
#define DEFAULT_ENCODE_FRAME_RATE 25	 // fs
#define DEFAULT_PROFILE 1				 // main
#define DEFAULT_ENCODE_FRAME_INTERVAL 50 // 50
#define DEFAULT_ENCODE_PREFER_FRAME 1	 // unused
#define DEFAULT_ENCODE_MAX_QP 32		 // unused
#define DEFAULT_ENCODE_TYPE 0			 // 0:音视频流1:视频流2:音频流
#define DEFAULT_AUDIO_SAMPLE_RATE 0		 // 8K
#define DEFAULT_AUDIO_BIT_WIDTH 1		 // 16bit
#define DEFAULT_AUDIO_ENCODE_TYPE 2		 // g711u
#define DEFAULT_AMR_MODE 0				 // MIME
#define DEFAULT_AMR_FORMAT 0			 // 4.75K

#define STANDRD_W 1920
#define STANDRD_H 1080

#define MAX_3G_MODULE 4 // 最大3G运营商数,联通，移动，电信，其他
#define MAX_SERIAL_NUM 4
#define MAX_PROBER_NUM 2
#define MAX_CUSTOM_OSD_NUM 6
	typedef struct __comm_version_info_t
	{
		char version_number[COMM_VERSION_LEN]; /* 版本号 */
		char device_type[COMM_VERSION_LEN];	   /* 产品型号 */
		char version_date[COMM_VERSION_LEN];   /* 版本日期 */
		unsigned int server_type;
		unsigned char is_support_wifi; /* 无线 0: 不支持, 1:支持*/
		char sub_type;
		char ircut_type; // 0：正向 1：反向
		char unusd;
		char device_serial_no[32];				   /* 产品序列号 */
		char leave_factory_date[COMM_VERSION_LEN]; /* 出厂日期 */
		char kernel_version[COMM_VERSION_LEN];
	} comm_version_info_t;

	typedef struct __comm_sys_config_t
	{
		unsigned int dev_type; /* 设备类型 ,IPC,NVR,DVR,AIBOX,DUALIPC*/
		unsigned int sub_type; /* 设备子类型 用于配置工具配置,球机，枪机，半球 */

		unsigned short maxWidth; /* 解码最大宽*/
		unsigned short maxHeigh; /*解码最大高*/

		unsigned char channel_num; /*产品的总通道数*/
		unsigned char prober_num;  /*报警输入探头数*/
		unsigned char output_num;  /*输出总数*/
		unsigned char serial_num;  /*串口总数*/

		unsigned char hd_num;		 /* hard disk number */
		unsigned char ptz_serial_no; /* 云台控制使用的固定串口号 */
		unsigned char wifi_enable;
		unsigned char enable_4G;

		unsigned char ad_type[4]; /* imx347,imx415,imx327 ...... */

		unsigned char flat_type;	 /*接入的平台*/
		unsigned char encode_format; /*编码格式	0：PAL  1：N*/
		unsigned char language;		 /*语言  0：英文  1：中文*/

		unsigned char sys_analogNum; // 可以接入的模拟通道数量
		unsigned char sys_digitNum;	 // 可以接入的数字通道数量
		unsigned char ircut_type;	 // 0 : 正向 1:反向
		// 0:LIGHT_MODE_IR = 0,	LIGHT_MODE_WLED,LIGHT_MODE_IR_WLED,LIGHT_MODE_AUTO
		unsigned char led_type; // 出厂默认值 ？
		unsigned char unused0;
		unsigned int resolv_capable_m[COMM_MAX_CHANNEL_NUM];
		/*	设备各通道能力级 主码流
		 *  bit0:QCIF bit1:CIF bit2:HD1 bit3:D1 bit4:720p
		 *  bit5:QVGA bit6:VGA bit7:HI_SXGA bit8:XGA bit9:1080i*/
		unsigned int resolv_capable_s[COMM_MAX_CHANNEL_NUM];
		/*	设备各通道能力级 从码流
		 *  bit0:QCIF bit1:CIF bit2:HD1 bit3:D1 bit4:720p
		 *  bit5:QVGA bit6:VGA bit7:HI_SXGA bit8:XGA bit9:1080i*/
		unsigned char projectID[8];
		unsigned char unused1[7];
	} comm_sys_config_t;

	////////////////////// 以下为老的设备搜索用的
	typedef struct __sys_config_t
	{
		unsigned int dev_type;		 /*设备类型 */
		unsigned int sub_type;		 /*设备子类型*/
		uchar_t chip_num;			 /*设备芯片数*/
		uchar_t channel_num;		 /*设备通道数*/
		uchar_t prober_num;			 /*设备报警输入数*/
		uchar_t output_num;			 /*设备报警输出数*/
		uchar_t serial_num;			 /*设备串口数*/
		uchar_t ccd_enable : 1;		 /*unused*/
		uchar_t ptz_serial_type : 1; /*unused*/
		uchar_t ptz_serial_no : 6;	 /*unused*/
		uchar_t adchip_type;
		uchar_t flat_type;			  /*unused*/
									  // 设备各通道能力级
		uint32_t resolv_capable_m[4]; // 主通道能力级
		uint32_t resolv_capable_s[4]; // 从通道能力级
		uchar_t ds_enable_flag : 1;	  /*双码流使能*/
		uchar_t D1_type : 1;		  /*unused*/
		uchar_t product_type : 6;	  /*unused*/
		uchar_t unused0[16];		  // 主通道扩展能力级
		uchar_t unused1[16];		  // 从通道扩展能力级
		uchar_t cmos_enable;		  /*unused*/
		uchar_t encode_format;		  /*unused*/
		uchar_t language;			  /*unused*/
		uchar_t unused2[4];
		uchar_t projectID[8]; /* 项目唯一标示 */
	} sys_config_t;

	typedef struct __comm_system_status_t
	{
		int cpu_percent; /* CPU利用率 */
		int run_hour;	 /* 运行时间:小时 */
		int run_minute;	 /* 运行时间:分钟*/
	} comm_system_status_t;
	/* 日志信息参数 */

	/* 录像时间段结构 */
	typedef struct __comm_rec_time_seg_t
	{
		uchar_t start_hour;
		uchar_t start_minute;
		uchar_t start_second;
		uchar_t end_hour;
		uchar_t end_minute;
		uchar_t end_second;
		uchar_t day_mask;
		uchar_t unused[1];
	} comm_rec_time_seg_t;

	/* 探头报警设置 */
	typedef struct __comm_prober_day_t
	{
		comm_rec_time_seg_t time_seg[COMM_MAX_TIME_SEG_NUM];
		uchar_t prober_guard_flag;	 /* 1 :布防, 0 :撤防 */
		uchar_t prober_enable;		 /* 1:enable, 0: enable */
		uchar_t alarm_interval_time; /* 侦察间隔时间*/
		uchar_t joint_record_time;	 /* 检测到报警信息后持续录像的时间*/
		uchar_t joint_output_time;	 /* 检测到报警信息后持续输出的时间*/
		uchar_t beep_output_time;	 /* 检测到报警信息后持续锋鸣器发声的时间*/
		uchar_t beep_output_enable;	 /* 锋鸣器输出使能 */
		uchar_t capture_num;		 /* 联动抓拍张数 */
		uchar_t capture_link;		 /* 抓拍图片处理bit0:EMAIL 1:FTP*/
		uchar_t unused[3];
		int joint_snap_enable_ch;	/* 联动抓拍使能 */
		int joint_record_enable_ch; /* 联动录像使能,1: enable, 0: disable */
		int joint_output_enable_ch; /* 联动输出使能,1: enable, 0: disable */
	} comm_prober_day_t;

	typedef struct __comm_prober_alarm_t
	{
		uchar_t prober_type; /* 探头类型 0:常开	1:常闭 */
		uchar_t preset_no;
		uchar_t unused;
		uchar_t sub_prober_type; /* 探头子类型	0:normal  1:巡更 */
		comm_prober_day_t prober_day[COMM_MAX_WEEK_DAY];
	} comm_prober_alarm_t;

	/* 视频移动设置 */
	typedef struct __comm_motion_area_t
	{
		unsigned short area[12];
	} comm_motion_area_t;

	typedef struct __comm_video_day_t
	{
		comm_rec_time_seg_t time_seg[COMM_MAX_TIME_SEG_NUM]; /* 时间段 */
		uchar_t move_guard_flag;							 /* 1 :布防, 0 :撤防 */
		uchar_t move_enable;								 /* 报警使能, 1: enable, 0: disable */
		uchar_t sensitive_level;							 /* 灵敏度,1:1级,...,9:9级 */
		uchar_t alarm_interval_time;						 /* 侦察间隔时间 */
		uchar_t joint_record_time;							 /* 检测到报警信息后持续录像的时间*/
		uchar_t joint_output_time;							 /* 检测到报警信息后持续输出的时间*/
		uchar_t beep_output_time;							 /* 检测到报警信息后持续锋鸣器发声的时间*/
		uchar_t beep_output_enable;							 /* 锋鸣器输出使能 */
		int joint_snap_enable_ch;							 /* 联动抓拍使能 */
		int joint_record_enable_ch;							 /* 联动录像使能,1: enable, 0: disable */
		int joint_output_enable_ch;							 /* 联动输出使能,1: enable, 0: disable */
		uchar_t capture_num;								 /* 联动抓拍张数*/
		uchar_t capture_link;								 /*抓拍图片处理bit0:EMAIL 1:FTP*/
		uchar_t unused[2];
		comm_motion_area_t motion_area;
	} comm_video_day_t;

	typedef struct __comm_video_move_t
	{
		comm_video_day_t video_day[COMM_MAX_WEEK_DAY];
	} comm_video_move_t;

	/* 视频设置 */
	typedef struct __comm_video_config_t__
	{
		uchar_t dis_title[COMM_MAX_TITLE_LEN];
		uchar_t brightness; /* 亮度值,范围:0-255 */
		uchar_t contrast;	/* 对比度值,范围:0-255 */
		uchar_t sharpness;	/*      用于 锐度调节 0-255*/
		uchar_t saturation; /* 饱和度值,范围:0-255 */
							/* 色调值,范围:0-255  */
	} comm_video_config_t;

	/* 编码设置 */
	typedef struct __comm_video_encode_t
	{
		int video_format;		/* 制式选择 0:PAL	1:NTSC */
		int resolution;			/* 分辨率,0:QCIF,1:CIF,2:HD1,3:D1, 5:QVGA, 6:VGA */
		int bitrate_type;		/* 位率类型,0:CBR,1:VBR */
		int level;				/* 画质,200kbit3000kbit */
		int frame_rate;			/* 每秒侦数 */
		int Iframe_interval;	/* I侦间隔 */
		char prefer_frame;		/* 侦率优先*/
		char profile;			// profile类型 0:baseline 1:main 2:high
		char profile_level;		// 保留
		char video_encode_type; // 0:h264 1:h265 2:mjpeg
		int Qp;					/* 量化系数 */
		int encode_type;		/* 0:音视频流, 1:视频流, 2:音频流*/
	} comm_video_encode_t;

	/*音频编码参数*/
	typedef struct __comm_audio_encode_t
	{
		char sample_rate; /* 采样率 0:8k, 1:11.025k, 2:16K, 3:22.050K, 4:24K, 5:32K, 6:44.1K, 7:48K */
		char bit_width;	  /* 位宽 0: 8bit, 1: 16bit */
		char encode_type; /* 编码方式 0:原始码流, 1:g711a, 2:g711u, 3:ADPCM, 4:g726_16K, 5: g726_24K,
				  6: g726_32K,7 : g726_40K, 8:AMR, 9:AMRDTX, 10:AAC */
		char amr_mode;	  /* bit rate type 0:4.75k,1:5.15k,2:5.90K,3:6.70K,4:7.40k,5:7.95k,6:10.20k,7:12.20k */
		char amr_format;  /* package type 0:MIME(file saving), 1:IF1(wireless), 2:IF2(wireless) */
		char unused[7];
	} comm_audio_encode_t;

	/* 录像参数 */
	typedef struct __comm_record_param_t
	{
		uchar_t cover_type;		  /* 覆盖方式,0:自动覆盖,1:报警提示.*/
		uchar_t prerec_status;	  /* 预录状态,0:停止,1:开启 */
		uchar_t switch_file_time; /* 录像文件切换时间 10~30分钟*/
		uchar_t ds_enable_flag;	  /* 双码流(double stream)使能标志 1: enable, 0: disable*/
		uchar_t ds_record_ch;	  /* 双码流(double stream)录像通道 0: master, 1: slave */
		uchar_t ds_net_ch;		  /* 双码流(double stream)网传通道 0: master, 1: slave*/
		uchar_t manual_record_time;
		uchar_t unused[1];
		int delete_space; /* 硬盘录像空间报警阈值*/
		int delete_unit;  /* 删除单元大小 */
	} comm_record_param_t;

	/* 串口参数 */
	typedef struct __comm_serial_info_t
	{
		uchar_t data_bit; /* 数据位,值:5/6/7/8 默认8*/
		uchar_t stop_bit; /* 停止位,值:0/1/2 默认1 */
		uchar_t reserve[2];
		int verify_rule; /* 验证规则,-1:无,0:偶校验,1:奇校验 默认-1*/
		int baut_rate;	 /* 波特率,协议特定 默认9600*/
	} comm_serial_info_t;

	/* 云台设置 */
	typedef struct __comm__ptz_config_t
	{
		uchar_t ptz_addr;  /* 云台地址0 ~ 255 */
		uchar_t ptz_type;  /* 云台类型索引.PELCO_D-1,PELCO_P-2,松下-3,YAAN-4,santachi-5,santachi_in-6，custom_01-0xA*/
		uchar_t serial_no; /* 串口号,值默认为0*/
		uchar_t reserve;
		comm_serial_info_t serial_info;
	} comm_ptz_config_t;

	/*透明传输设置*/
	typedef struct __comm_transparence_info_t
	{
		uchar_t serial_no; /* 串口号,值默认为0 */
		uchar_t up_flag;   /* 上传数据标志	0 不上传, 1 上传*/
		uchar_t down_flag; /* 下载数据标志	0 不处理下载数据, 1 处理*/
		uchar_t reserve;
	} comm_transparence_info_t;

	/* 网络设置 */
	typedef struct __comm_network_config_t
	{
		ushort_t port_num; /* 端口号,范围:8000-50000 */
		uchar_t protocal_type;
		uchar_t dhcp_flag; /* dhcp 使能标志,0: 禁止, 1, 使能 */
		uchar_t ip_addr[COMM_MAX_ADDR_SIZE];
		uchar_t net_mask[COMM_MAX_ADDR_SIZE];
		uchar_t def_gateway[COMM_MAX_ADDR_SIZE];
		uchar_t broad_addr[COMM_MAX_ADDR_SIZE];
		uchar_t mac_addr[COMM_MAX_MAC_SIZE];
		uchar_t first_dns_addr[COMM_MAX_ADDR_SIZE];	  /* 主DNS */
		uchar_t second_dns_addr[COMM_MAX_ADDR_SIZE];  /* 备用DNS */
		ushort_t dns_manu_flag;						  /* dns 获得标志,0: 自动获得, 1: 手动输入*/
		ushort_t web_port;							  /* web 端口配置 */
		ushort_t rtsp_port;							  /* RTSP 端口配置 */
		uchar_t used_gateway;						  /* 0:使用eth0的网关   1:使用eth2的网关*/
		uchar_t is_enable_wireless;					  /* 是否使能无线网络  1: 使能	0:禁用 */
		uchar_t ip_addr_eth2[COMM_MAX_ADDR_SIZE];	  /* 无线网卡IP */
		uchar_t net_mask_eth2[COMM_MAX_ADDR_SIZE];	  /* 无线mask */
		uchar_t def_gateway_eth2[COMM_MAX_ADDR_SIZE]; /* 无线网关 */
		uchar_t broad_addr_eth2[COMM_MAX_ADDR_SIZE];  /* 无线多播 */
		uchar_t mac_addr_eth2[COMM_MAX_MAC_SIZE];	  /* 无线mac */
	} comm_network_config_t;

	/* 定时录像设置 */
	typedef struct __comm_time_seg_t
	{
		uchar_t start_hour;
		uchar_t start_minute;
		uchar_t start_second;
		uchar_t unused;
	} comm_time_seg_t;

	typedef struct __comm_timer_week_t
	{
		comm_rec_time_seg_t time_seg[COMM_MAX_TIME_SEG_NUM];
		char enable_flag; /* 1: enable, 0: disable */
		char unused[3];
	} comm_timer_week_t;

	typedef struct __comm_timer_record_t
	{
		comm_timer_week_t timer_week[COMM_MAX_WEEK_DAY];
	} comm_timer_record_t;

	typedef struct __comm_timer_color2gray_t
	{
		comm_timer_week_t timer_week[COMM_MAX_WEEK_DAY];
	} comm_timer_color2gray_t;

	// auto color to gray
	typedef struct __comm_auto_color2gray_t
	{
		int color2gray_mod; // 0:auto, 1: color, 2: gray.
		int color2gray_threshold;
		int gray2color_threshold;
	} comm_auto_color2gray_t;

	/* 外接USB设备类型 */
	typedef struct __comm_usb_type_t
	{
		int type;		 /* 0:中兴模块  1:华为模块 */
		int sub_type;	 /* 0-0:AC8710  1-0:EC2610 暂时不用*/
		char unused[32]; /* 保留字节*/
	} comm_usb_type_t;

	typedef struct __comm_module_config_t
	{
		char ppp_apn[64];
		char sm_apn[64];
		char tel_num[32];
		char use_id[64];
		char password[64];
		char unused[64];
	} comm_module_config_t;

	/* 网络对时参数 */
	typedef struct __comm_net_chect_time_t
	{
		int enable;		 /* 使能 */
		int time;		 /* 对时间隔时间 （秒）*/
		int zone;		 /* 共24区 0 -25 */
		char addr[64];	 /* 可以为域名 */
		char unused[32]; /* 保留字节 */
	} comm_net_chect_time_t;

	typedef struct __comm_flat_chect_time_t
	{
		int enable;		 /* 使能 */
		int type;		 /* 平台类型 */
		char unused[32]; /* 保留字节 */
	} comm_flat_chect_time_t;

	typedef struct __comm_check_time_t
	{
		comm_net_chect_time_t net;	 /* 网络对时 */
		comm_flat_chect_time_t flat; /* 平台对时 */
		int unused[64];				 /* 保留字节 */
	} comm_check_time_t;

	typedef struct __comm_capture_week_t
	{
		comm_rec_time_seg_t time_seg[COMM_MAX_TIME_SEG_NUM];
		unsigned char enable_flag; /* 1有效， 0无效*/
		unsigned char ftp_flag;	   /* 1有效，0无效 */
		unsigned char http_flag;   /* 1有效，0无效 */
		unsigned char unused[3];
		unsigned short inter_time; /* 抓拍发送间隔	*/
	} comm_capture_week_t;

	typedef struct __comm_capture_sched_t
	{
		comm_capture_week_t timer_week[COMM_MAX_WEEK_DAY];
	} comm_timer_capture_t;

	/*FTP配置*/
	typedef struct __comm_ftp_info_t
	{
		char ftp_enable;
		char unused[3];
		char userName[64];
		char passWord[64];
		char directory[256]; /* ftp://主机名:端口/路径/ */

	} comm_ftp_info_t;

	typedef struct __comm_ftp_upload_info_t
	{
		char base_name[32]; // 上传的基准名字
		int file_suffix;	// 文件后缀名; 1：只用基准名	 2：在名字后面跟上日期时间	 3：跟上数字上限，循环使用
		int num;			// 数字上限，如果file_suffix为3时使用
		char unused[16];	// 保留字节
	} comm_ftp_upload_info_t;

	typedef struct __comm_instant_capture_t
	{
		int enable;		/* 0 使能; 1 不使能 */
		int inter_time; /*抓拍间隔时间,最短为1秒 */
	} comm_instant_capture_t;

	typedef struct __comm_snap_jpeg_config_t
	{
		char qp; /* 抓拍图片QP 值 [0,4] */
		char nused[3];
	} comm_snap_jpeg_config_t;

	typedef struct __comm_wireless_hostap_config_t
	{
		unsigned char connType;
		unsigned char res[3];
		unsigned char ap_ssid[64]; // 用于设置hostap模式下的ssid
		unsigned char ap_pwd[64];  // 用于设置hostap模式下的pwd
	} comm_wireless_hostap_config_t;

	/*OSD LOGO设置*/
	typedef struct __comm_osd_logo_t
	{
		unsigned char enable;	 /* 1: enable, 0: disable */
		unsigned char color_red; /* 0: white, 1: red, 2: blue */
		unsigned char color_green;
		unsigned char color_blue;
		unsigned short x_pos;
		unsigned short y_pos;
		char logo[48];
		// unsigned char font_size;
	} comm_osd_logo_t;

	/*OSD TIME设置*/
	typedef struct __comm_osd_time_t
	{
		unsigned char enable;	 /* 1: enable, 0: disable */
		unsigned char color_red; /* 0: white, 1: red, 2: blue */
		unsigned char color_green;
		unsigned char color_blue;
		unsigned short x_pos;
		unsigned short y_pos;
		// unsigned char font_size;
	} comm_osd_time_t;

	/*OSD INFO设置*/
	typedef struct __comm_osd_info_t
	{
		comm_osd_time_t osd_time;
		comm_osd_logo_t osd_logo;
	} comm_osd_info_t;

	typedef struct __comm_osd_custom_t
	{
		comm_osd_logo_t osd_custom[MAX_CUSTOM_OSD_NUM];
	} comm_osd_custom_t;

	typedef struct __comm_overlay_index_t
	{
		unsigned int color;
		unsigned short x_start;
		unsigned short y_start;
		unsigned short width;
		unsigned short height;
	} comm_overlay_index_t;

	typedef struct __comm_overlay_info_t
	{
		unsigned char enable;
		unsigned char num;
		unsigned char reserve[2];
		comm_overlay_index_t overlay[COMM_MAX_OVERLAY_NUM];
	} comm_overlay_info_t;

	/*wireless*/
	typedef struct __comm_wireless_wep_password_t /* LSL */
	{
		uchar_t safety_option; /* 安全选项 */
		uchar_t passwd_format; /* 密钥格式 */
		uchar_t key_numb;	   /* 密钥选择 */
		uchar_t unused;
		uchar_t password[4][32]; /* 密码 */
		uchar_t key_type[4];	 /* 密钥类型 */
	} comm_wireless_wep_password_t;

	typedef struct __comm_wireless_wpa_password_t /* LSL */
	{
		uchar_t safety_option; /* 安全选项 */
		uchar_t passwd_format; /* 密钥格式 */
		uchar_t unused[2];
		uchar_t server_ip[COMM_MAX_ADDR_SIZE];
		int port;
		uchar_t username[48]; /* 密码 */
		uchar_t password[48]; /* 密码 */
		int update_cycle;
	} comm_wireless_wpa_password_t;

	typedef struct __comm_wireless_wpapsk_password_t /* LSL */
	{
		uchar_t safety_option; /* 安全选项 */
		uchar_t passwd_format; /* 密钥格式 */
		uchar_t unused[2];
		uchar_t username[48]; /* 密码 */
		uchar_t password[48]; /* 密码 */
		int update_cycle;
	} comm_wireless_psk_password_t;

	typedef struct __comm_wireless_scanning_t
	{
		int current_used; /* 是否使用  1:使用	 0:未使用*/
		char essid[64];
		int signalStrength; /* 信号强度 */
		int isEncrypt;		/* 是否加密  1:加密  0:未加密*/
		int safety_type;	/* 1:WEP   3:WPA   2:WPA-PSK */
		int safety_option;	/* 安全选项  1:自动选择	2:开放系统	 3:共享密钥*/
		int key_numb;
		uchar_t username[48]; /* 密码 */
		uchar_t password[48]; /* 密码 */
	} comm_wireless_scanning_t;

	typedef struct __comm_wireless_config_t /* LSL */
	{
		uchar_t safety_type;	/* 安全类型 1:wep	2:psk	3:wpa */
		uchar_t channel;		/* 频道 */
		uchar_t is_enable_safe; /* 是否开启安全选项	 1:是	 0:否*/
		uchar_t unused;
		uchar_t essid[64]; /* 路由器名称 */
		comm_wireless_wep_password_t wep_key;
		comm_wireless_psk_password_t psk_key;
		comm_wireless_wpa_password_t wpa_key;
	} comm_wireless_config_t;

	/*设备名称*/
	typedef struct __comm_device_info_t
	{
		char device_name[COMM_MAX_DEVICE_NAME_LEN];
		char channel_name[COMM_MAX_CHANNEL_NUM][COMM_MAX_DEVICE_NAME_LEN];
		char unused[32];
	} comm_device_info_t;

	typedef struct __comm_user_info_t
	{
		uchar_t user_name[COMM_MAX_USER_NAME_LEN]; /* 用户名 */
		uchar_t user_passwd[COMM_MAX_PASSWD_LEN];  /* 用户密码的字符型表示*/
		ushort_t local_right;					   /* 从第一位开始,分别表示本地的录像权限,放像权限,参数设置权限,硬盘管理,数据删除,云台控制,数据备份权限*/
		ushort_t remote_right;					   /* 从第一位开始,分别表示远程的录像权限,放像权限,参数设置权限,硬盘管理,数据删除,云台控制,数据备份,报警输出,语音对讲权限*/
	} comm_user_info_t;

	/* 手动录像设置 */
	typedef struct __comm_hand_record_t
	{
		uchar_t record_hour_len;
		uchar_t record_minute_len;
		uchar_t record_status;
		uchar_t unused;
	} comm_hand_record_t;

	typedef struct __comm_netcom_config_t
	{
		char puId[64];
		char puName[64];
		char puPassword[64];
		int controlPort;
		int sipPort;
		int rtspPort;
		char dns[16];

		int registerServerPort;
		char registerServerIp[64];

		int alarmServerPort;
		char alarmServerIp[16];
		char alarmURL[128];

		int enableHeartbeat;
		int heartbeatPeriod;
		int heartbeatServerPort;
		char heartbeatServerIp[64];

		int enableSipRegister;
		int sipRegisterServerPort;
		char sipRegisterServerIp[64];

		int enableSipHeartbeat;
		int sipHeartbeatPeriod;
		int sipHeartbeatServerPort;
		char sipHeartbeatServerIp[64];
		char enableFlag;	   /* 0:不启用;  1:启用. */
		char conType;		   /* 1:tcp 0:udp */
		char isOnLine;		   /* 1:在线  0:不在线 */
		char smoothFlag;	   /* 平滑标记  0:不平滑  1:平滑 */
		unsigned char ptzTime; /*云台控制有效期*/
		char videoConfCh;	   /* 视频会议视频通道，取值0x00 ~ MaxChannel -1*/
		char audioConfCh;	   /* 视频会议音频通道，取值0x00 ~ MaxChannel ，取值为MaxChannel表示对讲通道*/
		char unused[1];
		unsigned char offRecord; /* 设备掉线录像 0:不启用;	1:启用. */
		unsigned char offUpload; /* 设备掉线录像上传 0:不启用;  1:启用. */
	} comm_netcom_config_t;

	typedef struct __gb_channel_t
	{
		char deviceCode[32];	  // 设备编码
		char name[32];			  // 设备名称
		char manufacturer[32];	  // 生产商家
		char model[32];			  // 设备型号
		char owner[32];			  // 设备归属
		char civilCode[32];		  // 行政区域
		char block[32];			  // 警区
		char address[32];		  // 安装地址
		int parental;			  // 是否有子设备 0:没有 1:有
		char parentCode[32];	  // 父设备ID
		int safetyWay;			  // 信令安全模式
		int registerWay;		  // 注册方式
		char certNum[32];		  // 证书序列号
		int certifiable;		  // 证书有效标识
		int errCode;			  // 无效原因码
		comm_time_info_t endTime; // 证书终止有效期
		int secrecy;			  // 保密属性
		char deviceIP[32];		  // 设备IP
		int port;				  // 设备端口
		char password[32];		  // 设备口令
		int status;				  // 设备状态
		double logitude;		  // 经度
		double latitude;		  // 纬度
	} gb_channel_t;

	typedef struct __gb_platform_t // 平台参数
	{
		int GBT_enable;		   // 0:不使能 1:使能   平台使能标志
		int connectType;	   // 连接协议    0:tcp  1:udp
		char platformIP[32];   // 平台IP地址
		int platformPort;	   // 平台端口    5060
		char platformCode[32]; // 平台编码
		char DevDomainID[32];  // 设备域ID
		char DevCode[32];	   // 设备编码或者设备ID
		char userName[32];	   // 登录平台的用户名
		char password[32];	   // 设备密码,用于平台验证
		int expiresTime;	   // 注册的有效期限
		int keepAliveTIme;	   // 心跳保活周期
		int max_camera;
		char max_alarmIn;
		char max_alarmOut;
		char audioIn_enable;
		char audioOut_enable;
		char unused[4]; //
		gb_channel_t cameraInfo[2];
		gb_channel_t audioInInfo;
		gb_channel_t audioOutInfo;
		gb_channel_t alarmInInfo[COMM_MAX_CHANNEL_NUM];
		gb_channel_t alarmOutInfo[COMM_MAX_CHANNEL_NUM];
	} gb_platform_t;

	typedef struct __comm_isp_exposure_t
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
	} comm_isp_exposure_t;

	typedef struct __comm_isp_dayNight_t
	{
		int iMode; // auto  day night spec
		int iSens; // 0....7
		comm_time_seg_t start;
		comm_time_seg_t end;
		int iFilterTime;	// 3...10
		int iLedMode;		// red, white/red    white
		int iLedLightValue; // 0...10
	} comm_isp_dayNight_t;

	typedef struct __comm_isp_blc_t
	{
		int iHdrMode;	  // normal   hdr2
		int iHdrLevel;	  // 0,1,2,3
		int iBlcMode;	  // close,open
		int iBlcValue;	  // 0-100
		int iHlcMode;	  // close open
		int iHlcLevel;	  // 0-100
		int iHlcEncValue; // 0-100
	} comm_isp_blc_t;
	typedef struct __comm_isp_wbgain_t
	{
		int iRGainValue;  // 0 -100
		int iGRGainValue; // 0-100
		int iGBGainValue; // 0-100
		int iBGainValue;  // 0-100
	} comm_isp_wbgain_t;

	typedef struct __comm_isp_wbscene_t
	{
		int iSeneMode;
		// RK_AIQ_WBCT_INCANDESCENT = 0,
		// RK_AIQ_WBCT_FLUORESCENT,
		// RK_AIQ_WBCT_WARM_FLUORESCENT,
		// RK_AIQ_WBCT_DAYLIGHT,
		// RK_AIQ_WBCT_CLOUDY_DAYLIGHT,
		// RK_AIQ_WBCT_TWILIGHT,
		// RK_AIQ_WBCT_SHADE
	} comm_isp_wbscene_t;

	typedef struct __comm_isp_wbcct_t
	{
		int cct;
		int ccri; //?? what mean
	} comm_isp_wbcct_t;

	typedef struct __comm_isp_wb_t
	{
		int iWbMode; // auto,manul-wbgain,manul-wbscene,manul-wbcct
		comm_isp_wbgain_t wbgain;
		comm_isp_wbscene_t wbscene;
		comm_isp_wbcct_t wbcct;
	} comm_isp_wb_t;

	typedef struct __comm_isp_nr_t
	{
		int iNrMode;	  // 0 default ,1 timeNR 2 spaceNR, 3 time and space
		int iTimeFilter;  // 0-100
		int iSpaceFilter; // 0-100
	} comm_isp_nr_t;

	typedef struct __comm_isp_forg_t
	{
		int iForgMode; // close open auto
		int iForgLevel;
	} comm_isp_forg_t;

	typedef struct __comm_isp_abc_t
	{
		int iAbcMode;
		int iAbcLevel;
	} comm_isp_abc_t; // Aberration correction
	typedef struct __comm_isp_mirror_t
	{
		int iMirrorMode;
	} comm_isp_mirror_t;
	typedef struct __comm_isp_config_t
	{
		comm_isp_exposure_t exposure;
		comm_isp_dayNight_t dayNight;
		comm_isp_blc_t blc;
		comm_isp_wb_t wb;
		comm_isp_nr_t nr;
		comm_isp_forg_t forg;
		comm_isp_abc_t abc;
		comm_isp_mirror_t mirror;
	} comm_isp_config_t;

	/* unused */
	typedef struct __online_info_t
	{
		char online_status; /* 1 :上线 */
		char unused[3];
		char online_name[64];
	} online_info_t;

	/* unused */
	typedef struct __online_status_t
	{
		char online_type; /* device:1, channel:2, device && channel:3 */
		char channel_num;
		char unused[2];
		online_info_t online_device;
		online_info_t online_channel[COMM_MAX_CHANNEL_NUM];
	} online_status_t;

	/*PPPoE配置*/
	typedef struct __nt200h_pppoe_info_t
	{
		char account[64];
		char password[24];
		char ip[16];
		char enable;
		char status;
		unsigned char ppp_interface : 2; /* eth0 :0,  eth1 :1 */
		unsigned char dns_server : 6;	 /* qdns 0,qdns-static 1, dyndns 2, dyndns-static 3 */
		char dns_enable;				 /* 1: enable, 0: disable */
		char mydomain[256];
		char dns_user[128];
		char dns_password[128];
	} pppoe_info_t;

	typedef struct __module_config_t
	{
		char ppp_apn[64];
		char sm_apn[64];
		char tel_num[32];
		char use_id[64];
		char password[64];
		char unused[64];
	} module_config_t;

	/* 外接USB设备类型 */
	typedef struct _usb_type_t
	{
		int type;		 /* 0:联通，1:移动 ,2:电信        3:其他*/
		int sub_type;	 /* 暂时不用*/
		char unused[32]; /* 保留字节*/
	} usb_type_t;

#define COMM_ROI_TYPE_LINE 0x01
#define COMM_ROI_TYPE_RECT 0x02
#define COMM_ROI_TYPE_POLYGON 0x03
#define COMM_ROI_RECT_NUM 4
#define COMM_ROI_LINE_NUM 4
#define COMM_ROI_POLYGON_NUM 4
#define COMM_ROI_MAX_VERT 10 // 多边形最多10个顶点
	typedef enum
	{
		COMM_DIR_INNER = 0, // 框内有效
		COMM_DIR_OUT,		// 框外有效
		COMM_DIR_UP,		//
		COMM_DIR_DOWN,
		COMM_DIR_LEFT,
		COMM_DIR_RIGHT,
		COMM_DIR_LEFT_UP,
		COMM_DIR_LEFT_DOWN,
		COMM_DIR_RIGHT_UP,
		COMM_DIR_RIGHT_DOWN
	} COMM_DIR_E;

	struct __comm_roi_rect_t__
	{
		int xpoint;
		int ypoint;
		int nwidth;
		int nHeigh;
		int nDirection; // DIR_INNER，DIR_OUT
	};
	typedef struct __comm_roi_rect_t__ comm_roi_rect_t;

	struct __comm_roi_point_t__
	{
		int xPoint;
		int yPoint;
	};
	typedef struct __comm_roi_point_t__ comm_roi_point_t;

	struct __comm_roi_line_t__
	{
		int nStartXpoint;
		int nStartYPoint;
		int nEndXPoint;
		int nEndYPoint;
		int nDirection; // DIR_UP,DIR_DOWN,DIR_LEFT,DIR_RIGHT,DIR_LEFT_UP,DIR_LEFT_DOWN,DIR_RIGHT_UP,DIR_RIGHT_DOWN
	};
	typedef struct __comm_roi_line_t__ comm_roi_line_t;
	struct __comm_roi_polygon_t__
	{
		int point_num;
		int nDirection; // DIR_INNER，DIR_OUT
		comm_roi_point_t comm_point[COMM_ROI_MAX_VERT];
	};
	typedef struct __comm_roi_polygon_t__ comm_roi_polygon_t;

	struct __comm_video_roi_aera_t__
	{
		int rect_num;	 // 矩形个数
		int line_num;	 // 线条数
		int polygon_num; // 多边形
		comm_roi_rect_t rect[COMM_ROI_RECT_NUM];
		comm_roi_line_t line[COMM_ROI_LINE_NUM];
		comm_roi_polygon_t polygon[COMM_ROI_POLYGON_NUM];
	};
	typedef struct __comm_video_roi_aera_t__ comm_video_roi_aera_t;

	struct __comm_roi_config_t__
	{
		comm_video_roi_aera_t roi_aera;
	};

	typedef struct __comm_roi_config_t__ comm_roi_config_t;

//////////////
#define MAX_NET_RTMP_URL_LEN 256
#define MAX_NET_RTMP_STREAM_NAME_LEN 256
#define MAX_NET_USER_NAME_LEN 32
#define MAX_NET_PASSWD_LEN 32

	typedef struct server_config_s server_config_t;
	struct server_config_s
	{
		int local_port;	 // local 通信端口
		int debug_level; // 调试等级
		int rtmp_enable; // 控制是否rtmp 启动开启
		char rtmp_url[MAX_NET_RTMP_URL_LEN];
		char rtmp_stream_name[MAX_NET_RTMP_STREAM_NAME_LEN];
		char net_user[MAX_NET_USER_NAME_LEN];
		char net_passwd[MAX_NET_PASSWD_LEN];
		int dhcp_flag;
		char ipaddr[COMM_ADDRSIZE];
		char netmask[COMM_ADDRSIZE];
		char gateway[COMM_ADDRSIZE];
		char first_dns[COMM_ADDRSIZE];
		char second_dns[COMM_ADDRSIZE];
		int multiples;		 // 球机显示倍数
		int multiples_flag;	 // 是否显示倍率
		int multiples_point; // 显示位置
		int multiples_times; // 显示时间
		int ptz_track_flag;	 // 是否自动跟踪
		int shift_af;		 // af值需要右移多少，默认 14
		int rtsp_enable;
		int onvif_enable;
		int gb28181_enable;
		int gat1400_enable;
		unsigned int version;
		int dial_enable; // 4G 拨号
		char apn[32];
		char apn_user[32];
		char apn_passwd[32];
		int wdt_enable; // 开启看门狗
		int ai_product; // ai 厂家  :   0--> non   1-->meg   2-->minivision  3 rockchip
	};

	//////////////
	void comm_param_startParamService(void);
	void comm_param_initParamLib();
	void comm_param_startNetwork(void);

	int comm_param_GetSysTime(comm_time_info_t *time_info);
	int comm_param_SetSysTime(comm_time_info_t *time_info);
	void comm_param_initRtcTime();
	void comm_param_getPppoeInfoStruct(pppoe_info_t *temp);
	void comm_param_setPppoeInfoStruct(pppoe_info_t temp);
	void comm_param_get3GConfigStruct(int num, module_config_t *temp);
	void comm_param_set3GConfigStruct(int num, module_config_t temp);
	void comm_param_getUsbTypeStruct(usb_type_t *temp);
	void comm_param_setUsbTypeStruct(usb_type_t temp);
	void comm_param_getVideoEncodeParam(int channel_no, comm_video_encode_t *temp);
	void comm_param_setVideoEncodeParam(int channel_no, comm_video_encode_t temp);
	void comm_param_setRoiConfig(int chann, comm_roi_config_t temp);
	void comm_param_getRoiConfig(int chann, comm_roi_config_t *temp);
	void comm_param_getDeviceInfo(comm_device_info_t *temp);
	void comm_param_getSysConfig(comm_sys_config_t *temp);
	void comm_param_getSerialStruct(int serial_no, comm_serial_info_t *temp);
	void comm_param_setSerialStruct(int serial_no, comm_serial_info_t temp);
	void comm_param_getTransparenceStruct(comm_transparence_info_t *temp);
	void comm_param_setTransparenceStruct(comm_transparence_info_t temp);
	void comm_param_getTerminateStruct(int channel_no, comm_ptz_config_t *temp);
	void comm_param_setTerminateStruct(int channel_no, comm_ptz_config_t temp);
	void comm_param_getHandRecordStruct(int channel_no, comm_hand_record_t *temp);
	void comm_param_setHandRecordStruct(int channel_no, comm_hand_record_t temp);
	void comm_param_getSensorConfig(int camid, comm_isp_config_t *temp);
	void comm_param_setSensorConfig(int camid, comm_isp_config_t temp);
	void comm_param_getProberStruct(int prober_no, comm_prober_alarm_t *temp);
	void comm_param_setProberStruct(int prober_no, comm_prober_alarm_t temp);
	void comm_param_getTimerStruct(int channel_no, int week_day, comm_timer_week_t *temp);
	void comm_param_setTimerStruct(int channel_no, int week_day, comm_timer_week_t temp);
	void comm_param_getVideoMoveStruct(int channel_no, int week_day, comm_video_day_t *temp);
	void comm_param_setVideoMoveStruct(int channel_no, int week_day, comm_video_day_t temp);
	void comm_param_getVideoConfigParam(int channel_no, comm_video_config_t *temp);
	void comm_param_setVideoConfigParam(int channel_no, comm_video_config_t temp);
	void comm_param_initVideoConfigParam(int max_channel_num);
	void comm_param_getOsdLogoStruct(int channel_no, comm_osd_logo_t *temp);
	void comm_param_setOsdLogoStruct(int channel_no, comm_osd_logo_t temp);
	void comm_param_getOsdTimeStruct(int channel_no, comm_osd_time_t *temp);
	void comm_param_setOsdTimeStruct(int channel_no, comm_osd_time_t temp);
	void comm_param_getOsdCustomStruct(int channel_no, comm_osd_custom_t *temp);
	void comm_param_setOsdCustomStruct(int channel_no, comm_osd_custom_t temp);
	void comm_param_getOverlayStruct(int channel_no, comm_overlay_info_t *temp);
	void comm_param_setOverlayStruct(int channel_no, comm_overlay_info_t temp);
	void comm_param_getRecParamStruct(comm_record_param_t *temp);
	void comm_param_setRecParamStruct(comm_record_param_t temp);
	void comm_param_getClientFtpStruct(comm_ftp_info_t *temp);
	void comm_param_setClientFtpStruct(comm_ftp_info_t temp);
	void comm_param_getTimerColor2grayStruct(int week_day, comm_timer_week_t *temp);
	void comm_param_setTimerColor2grayStruct(int week_day, comm_timer_week_t temp);
	void comm_param_getTimerCaptureStruct(int channel_no, int week_day, comm_capture_week_t *temp);
	void comm_param_setTimerCaptureStruct(int channel_no, int week_day, comm_capture_week_t temp);
	void comm_param_getClientFtpUploadStruct(int channel, comm_ftp_upload_info_t *temp);
	void comm_param_setClientFtpUploadStruct(int channel, comm_ftp_upload_info_t temp);

	void comm_param_setWsxStatus(int status);
	void comm_param_getNetcomConfig(comm_netcom_config_t *temp);
	void comm_param_setNetcomConfig(comm_netcom_config_t temp);
	void comm_param_setSnapconfigStruct(int channel, comm_snap_jpeg_config_t temp);
	void comm_param_getSnapconfigStruct(int channel, comm_snap_jpeg_config_t *temp);
	void comm_param_getGBConfig(gb_platform_t *temp);
	void comm_param_setGBConfig(gb_platform_t temp);

	void comm_param_getNetworkStruct(comm_network_config_t *nc);
	void comm_param_setNetworkStruct(comm_network_config_t nc);

	void comm_param_setSysConfig(comm_sys_config_t temp);
	void comm_param_getSysConfig(comm_sys_config_t *temp);

	void comm_param_setSlaveEncodeParam(int channel_no, comm_video_encode_t temp);
	void comm_param_getSlaveEncodeParam(int channel_no, comm_video_encode_t *temp);

	void comm_param_setAudioEncodeParam(comm_audio_encode_t temp);
	void comm_param_getAudioEncodeParam(comm_audio_encode_t *temp);

	void comm_param_getOnlineStatus(online_status_t *temp);
	void comm_param_setOnlineStatus(online_status_t temp);
	void comm_param_setTimeCheckConfig(comm_check_time_t temp);
	void comm_param_getTimeCheckConfig(comm_check_time_t *temp);
	uchar_t comm_param_getOutputNum(void);
	uchar_t comm_param_getProberNum(void);
	uchar_t comm_param_getAdchipType(void);
	uchar_t comm_param_getSerialNum(void);
	uint32_t comm_param_getSubType(void);
	uint32_t comm_param_getPtzSerialNo(void);
	void comm_param_getWirelessStruct(comm_wireless_config_t *temp);
	void comm_param_setWirelessStruct(comm_wireless_config_t temp);
	void comm_param_getSlaveEncodeParam(int channel_no, comm_video_encode_t *temp);
	void comm_param_setSlaveEncodeParam(int channel_no, comm_video_encode_t temp);

	void comm_param_getUserInfoStruct(int cur_user_num, comm_user_info_t *temp);
	int comm_param_getUserNum(void);
	void comm_param_deleteUser(int index);
	void comm_param_addUser(comm_user_info_t *user);
	void comm_param_getVersionInfo(comm_version_info_t *temp);
	void comm_param_setVersionInfo(comm_version_info_t temp);

	void comm_app_getDeviceVersionInfo(char *version);
	void comm_param_resetFactoryParam(void);
	void comm_param_resetAllStruct(void);
	int comm_param_resetDefaultParam(void);
	unsigned int comm_param_getSupportResolution(unsigned int chn, unsigned int chn_type);
	void comm_convertCommSysConfig2SysConfig(const comm_sys_config_t *pCommSysConfig, sys_config_t *pConfig);
	void comm_convertSysConfig2CommSysConfig(const sys_config_t *pConfig, comm_sys_config_t *pCommSysConfig);

	uchar_t comm_param_getChannelNum(void);
	uchar_t comm_param_getProberNum(void);
	uchar_t comm_param_getOutputNum(void);
	uchar_t comm_param_getAdchipType(void);
	uchar_t comm_param_getSerialNum(void);
	uint32_t comm_param_getSubType(void);
	uint32_t comm_param_getPtzSerialNo(void);

	void comm_config_lock();
	void comm_config_unlock();
	void update_server_config();
	server_config_t *get_server_config();
	int comm_config_read();
	int comm_config_write();

#ifdef __cplusplus
}
#endif

#endif
