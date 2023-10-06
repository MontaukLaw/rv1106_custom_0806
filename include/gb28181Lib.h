#ifndef __GBTLIB_H__
#define __GBTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#define		GBT_CODE_SIZE						32			//编码数组长度
#define 	GBT_PASSWD_SIZE					32			//密码数组长度
#define 	GBT_USERNAME_SIZE					32			//用户名长度
#define 	GBT_IP_SIZE						32			//IP数组长度
#define 	GBT_DOMAIN_SIZE					16			//域数组长度
#define 	GBT_FILE_PATH_LEN					256			//文件名长度
#define 	GBT_MAX_RECORD_COUNT				256			//一次查询录像文件或者抓拍图片最大数量
#define		GBT_STRING_LEN						32			//字符长
#define		GBT_DESCRIPTION_LEN				128			//描述字符串长度
#define		GBT_MAX_CAMERA_COUNT				64			//最大视频通道数量
#define		GBT_MAX_ALARMIN_COUNT				8			//最大告警输入数量
#define		GBT_MAX_ALARMOUT_COUNT			8			//最大告警输出数量
#define 	GBT_MAX_CATLOGITEM			    (GBT_MAX_CAMERA_COUNT + GBT_MAX_ALARMIN_COUNT + GBT_MAX_ALARMOUT_COUNT)	//设备目录最大条数
#define		GBT_RECORD_CLIENT_COUNT			128
#define		GBT_BASIC_NAME_SIZE				128
#define		GBT_ALARMTYPE_SIZE					8
#define 	GBT_MAX_NETCARDNO_SIZE			32
#define		GBT_MAX_NETCARDTYPE_SIZE			12
#define		GBT_MAX_NETCNT					3
#define		GBT_MAX_PRESET_SIZE		256
#define		GBT_MAX_AREA_SIZE			64
#define		GBT_MAX_AREA_POINT_SIZE	64
#define		GBT_DIGEST_SIZE			64

#define ALARM_AI_UPLOAD_PIC			0x01
#define ALARM_AI_UPLOAD_VIDEO			0x02
#define ALARM_AI_UPLOAD_ALL			0x03

/*****************************************************
		regState value  注册的3种状态值
******************************************************/

#define 	NON_REGISTER 				0x00	//没有注册
#define		NOW_REGISTER				0x01	//正在注册			
#define 	DONE_REGISTER 				0x02	//注册成功
#define		REQUEST_LOGOUT				0x04	//请求注销
#define		NOW_LOGOUT					0x08	//发送第一次注销成功
#define		DONE_LOGOUT				0x10	//注销成功
#define 	READY_REGISTER				0x20	//每次准备注册前，要先注销下设备
#define 	RUNNING_REGISTER			0x40	//正常操作状态
#define 	REQUEST_READY_REGISTER	0x80
/*****************************************************
			Guard value  注册的2种状态值
******************************************************/
typedef enum
{
	GUARD_RESET = 0,						//撤防
	GUARD_SET = 1							//布防
}GUARD_T;

/*****************************************************
		Hand Record value  注册的2种状态值
******************************************************/
typedef enum
{
	HAND_RECORD_STOP = 0,							//手动录像开始
	HAND_RECORD_START = 1							//手动录像结束
}HAND_RECORD_T;


typedef struct _GBT_ChnInfo_T_				//通道信息，包括视频通道，报警输入通道，报警输出通道
{
	int 	chnType;						//通道类型
	int     chnNo;							//通道号			//从1开始
	char    chnID[GBT_CODE_SIZE];			//通道ID
	char 	chnName[GBT_STRING_LEN];		//通道名称
}GBT_ChnInfo;

typedef struct _GBT_NetTrafic_T_
{
	char CardNo[GBT_MAX_NETCARDNO_SIZE];
	char CardType[GBT_MAX_NETCARDTYPE_SIZE];
	int CardStatus;
	int	LastMonthTotal;
	int	TotalUsed;
}GBT_NetTrafic_T;

typedef struct _GBT_Platform_T_				//平台参数
{
	int			GBT_enable;						//0:不使能 1:使能   平台使能标志
	int  		connectType;						//连接协议    0:tcp  1:udp
	
	char 		platformIP[GBT_IP_SIZE];    		//平台IP地址
	int  		platformPort;						//平台端口    5060
	char 		platformCode[GBT_CODE_SIZE];   	//平台编码

	char 		DevDomainID[GBT_DOMAIN_SIZE];		//设备域ID
	char 		DevCode[GBT_CODE_SIZE];  			//设备编码或者设备ID
	char		DevAlarmCode[GBT_CODE_SIZE];		//设备告警业务编码
	char 		password[GBT_PASSWD_SIZE];  		//设备密码,用于平台验证
	char 		userName[GBT_USERNAME_SIZE];
	int			expiresTime;						//注册的有效期限
	int			keepAliveTIme;						//心跳保活周期

	int 		cameraCount;						//视频通道数量
	GBT_ChnInfo cameraInfo[GBT_MAX_CAMERA_COUNT];
	
	int 		alarmInCount;							//告警输入通道数量
	GBT_ChnInfo alarmInInfo[GBT_MAX_ALARMIN_COUNT];	
		
	int 		alarmOutCount;							//告警输出通道数量
	GBT_ChnInfo alarmOutInfo[GBT_MAX_ALARMOUT_COUNT];

	int			audioIn_enable;
	GBT_ChnInfo audioInInfo;

	int			audioOut_enable;
	GBT_ChnInfo audioOutInfo;	
}GBT_Platform_T;

typedef struct _GBT_Time_T_		
{
	int 	year;   				//年
	int 	month;					//月
	int 	day;					//日
	int 	hour;					//时
	int 	minute;					//分
	int 	second;					//秒
}GBT_Time_T;

typedef struct _GBT_RecordFile_T_					//录像文件信息结构体
{
	unsigned int 	fileStartTime;						//绝对时间
	unsigned int 	fileEndTime;
	GBT_Time_T 		timeStart;							//年月日时分秒 时间
	GBT_Time_T 		timeEnd;
	int 			fileSize;
	int 			recordType;     					//012位依次是手动、定时、告警
	char 			filePath[GBT_FILE_PATH_LEN];		//文件路径
}GBT_RecordFile_T;

typedef struct _GBT_RecordList_T_					//录像文件或者抓拍图片列表结构体
{
	int 				fileType;						//文件种类 0: 录像文件 1: 图片
	int 				recordType;    				//0:手动 1:定时 2:告警
	unsigned int 		beginTime;						//查询开始时间
	unsigned int 		endTime;						//查询结束时间
	int 				fileNum;						//文件数量
	GBT_RecordFile_T 	recordFile[GBT_MAX_RECORD_COUNT];
}GBT_RecordList_T;


typedef struct _GBT_AlarmMsg_T_
{
	int 		alarmChannel;											//报警通道
	int 		alarmPriority;											//报警级别
	int 		alarmMethod;											//报警方式
	GBT_Time_T	alarmTime;												//报警时间
	int			sn;
	int 		UploadType;
	char		alarmType[GBT_ALARMTYPE_SIZE];							//AI 报警类型
	char 		alarmDescription[GBT_DESCRIPTION_LEN];				//报警描述
}GBT_AlarmMsg_T;


typedef struct _GBT_UploadFile_T_
{
	int 		UploadType;
	int 		UploadResult;
	int			sn;
	char		ChannCode[GBT_CODE_SIZE];	
	char 		PicUploadUrl[GBT_FILE_PATH_LEN];
	char		VideoUploadUrl[GBT_FILE_PATH_LEN];
}GBT_UploadFile_T;

typedef struct _GBT_AlarmStatus_T_
{
	char 	alarmCode[GBT_CODE_SIZE];					//报警编码
	int 	status;										//报警状态    0:撤防 1:布防 2:报警
}GBT_AlarmStatus_T;

typedef struct _GBT_AlarmAIStatus_T_
{
	char 	ChannCode[GBT_CODE_SIZE];					//报警编码
	int 	status;										//报警状态    0:已经上报服务器  1: 正在上传pic, 2:上传完成
}GBT_AlarmAIStatus_T;

typedef struct _GBT_DeviceStatus_T_					//设备状态查询
{
	int 				onlineStatus;								//是否在线				1:在线     0:不在线
	int 				workStatus;								//是否正常工作			1:正常工作 0:不正常工作
	char 				reason[GBT_DESCRIPTION_LEN];				//不正常工作原因		
	int					encodeStatus;								//是否编码				1:正在编码 0:没有编码
	int     			recordStatus;								//录像状态				1:正在录像 0:没有录像
	GBT_Time_T			timeNow;									//设备当前时间			
	int 				alarmCount;								//报警通道数量
	GBT_AlarmStatus_T   alarmStatus[GBT_MAX_ALARMIN_COUNT];		//报警通道状态
	int 				netCardCount;
	GBT_NetTrafic_T		netCardInfo[GBT_MAX_NETCNT];
}GBT_DeviceStatus_T;

typedef struct _GBT_SwitchStatus_T_
{
	char 	switchCode[GBT_CODE_SIZE];					//报警编码
	char	switchNo[GBT_STRING_LEN];
	int 	status;										//报警状态    0: off 1:on
}GBT_SwitchStatus_T;

typedef struct _GBT_DeviceSwitchStatus_T_					//设备状态查询
{
	GBT_Time_T			timeNow;									//设备当前时间			
	int 				switchCount;								//开关量状态查询
	GBT_SwitchStatus_T   alarmStatus[GBT_MAX_ALARMIN_COUNT];		//报警通道状态
}GBT_DeviceSwitchStatus_T;


typedef struct _GBT_DeviceInfo_T_						//设备信息查询
{
	char 			manufacturer[GBT_STRING_LEN];			//设备生产商
	char 			deviceType[GBT_STRING_LEN];			//设备类型
	char			model[GBT_STRING_LEN];					//设备型号
	char 			firmware[GBT_STRING_LEN];				//软件版本
	int 			channel;								//输入通道数
	int 			maxCamera;								//最大输入通道
	int 			maxAlarm;								//最大告警通道数
}GBT_DeviceInfo_T;

typedef struct _GBT_UpdateGradeInfo_T_
{
	char 	szFirmware[GBT_STRING_LEN];
	char	szFileURL[GBT_FILE_PATH_LEN];
	char	szManufacturer[GBT_STRING_LEN];
	char	szDigest[GBT_DIGEST_SIZE];
}GBT_UpdateGradeInfo_T;

typedef struct _GBT_CatlogItem_T_
{
	char 			deviceCode[GBT_CODE_SIZE];				//设备编码
	char			name[GBT_STRING_LEN];					//设备名称
	char 			manufacturer[GBT_STRING_LEN];			//生产商家
	char			model[GBT_STRING_LEN];					//设备型号
	char			owner[GBT_STRING_LEN];					//设备归属
	char 			civilCode[GBT_STRING_LEN];				//行政区域
	char 			block[GBT_STRING_LEN];					//警区
	char			address[GBT_STRING_LEN];				//安装地址
	int				parental;								//是否有子设备 0:没有 1:有
	char			parentCode[GBT_CODE_SIZE];				//父设备ID
	int				safetyWay;								//信令安全模式
	int				registerWay;							//注册方式
	char			certNum[GBT_CODE_SIZE];					//证书序列号
	int				certifiable;							//证书有效标识
	int 			errCode;								//无效原因码
	GBT_Time_T		endTime;								//证书终止有效期
	int 			secrecy;								//保密属性
	char 			deviceIP[GBT_IP_SIZE];					//设备IP
	int 			port;									//设备端口
	char 			password[GBT_PASSWD_SIZE];				//设备口令
	int 			status;									//设备状态
	double			logitude;								//经度
	double			latitude;								//纬度
}GBT_CatlogItem_T;

typedef struct _GBT_DeviceCatalog_T_				//设备目录查询
{
	int 				catalogCount;						//目录条数
	GBT_CatlogItem_T	catalogItem[GBT_MAX_CATLOGITEM];	//目录数组
}GBT_DeviceCatalog_T;


typedef struct _GBT_AlgoCfg_T_
{
	int 	nChann;
	char 	DeviceID[GBT_CODE_SIZE];
	char 	szDeviceName[GBT_BASIC_NAME_SIZE];
	char 	ChannID[GBT_CODE_SIZE];
	char 	szChannName[GBT_BASIC_NAME_SIZE];
	char 	szAlgoID[GBT_BASIC_NAME_SIZE];
	int 	nOptType;
	int		nExtractionType;
	double	fConfidenceThresh;
	int		nAlarmInterval;
	char	szModelConfig[GBT_BASIC_NAME_SIZE];
	int 	nPresetNum;
	struct
	{
		double	fPan;
		double	fTilt;
		double	fZoom;
		int 	nPresetID;
		char 	szPresetName[GBT_BASIC_NAME_SIZE];
	}preset[GBT_MAX_PRESET_SIZE];
	int nMonitorAreaNum;
	struct
	{
		char szAreaId[GBT_BASIC_NAME_SIZE];
		int nPointNum;
		struct
		{
			int nPointId;
			double	fX;
			double	fY;
		}point[GBT_MAX_AREA_POINT_SIZE];
	}Area[GBT_MAX_AREA_SIZE];	
}GBT_AlgoCfg_T;


typedef struct __GBT_HomePosion_T
{
	int nChann;
	int flag;
	int ResetTime;
	int PresetIndex;

}GBT_HomePosion_T;

typedef struct __GBT_PresetInfo_T
{
	int nChann;
	int iCount;
	struct
	{
		int 	nPresetID;
		char 	szPresetName[GBT_BASIC_NAME_SIZE];
	}preset[GBT_MAX_PRESET_SIZE];	
}GBT_PresetInfo_T;

/******************************************************************************************
* 控球命令
*******************************************************************************************/

typedef enum
{
	CMD_STOP_PTZ	= 0,								//停止扫描 巡航 PTZ
	CMD_PTZ 		= 1,								//云台控制指令
	CMD_FI		 	= 2,								//聚焦 光圈
	CMD_PRESET 	= 3,								//预置位指令
	CMD_CRUISE 	= 4,								//巡航指令
	CMD_SCAN 		= 5, 								//扫描指令	
	CMD_SWITCH_ON	 =6,
	CMD_SWITCH_OFF	 =7
	
}CMD_METHOD;

typedef enum											//水平方向
{		
	PAN_NULL		= 0, 								//水平方向不动
	PAN_LEFT 		= 1,								//向左
	PAN_RIGHT		= 2									//向右
}SUBCMD_PAN;

typedef enum											//垂直方向
{	
	TILT_NULL		= 0,
	TILT_UP 		= 1,								//向上		
	TILT_DOWN		= 2									//向下
}SUBCMD_TILT;

typedef enum											//放大缩小
{	
	ZOOM_NULL		= 0,
	ZOOM_IN 		= 1,								//放大		
	ZOOM_OUT		= 2									//缩小
}SUBCMD_ZOOM;

typedef enum											//聚焦
{	
	FOCUS_NULL		= 0,
	FOCUS_ADD 		= 1,								//聚焦加		
	FOCUS_SUB		= 2									//聚焦减
}SUBCMD_FOCUS;

typedef enum											//光圈
{	
	IRIS_NULL		= 0,
	IRIS_ADD		= 1,								//光圈+
	IRIS_SUB		= 2									//光圈-
}SUBCMD_IRIS;

typedef enum 
{
	PRESET_SET		= 1,								//设置预置位
	PRESET_DEL		= 2,								//删除预置位
	PRESET_USE		= 3									//调用预置位
}SUBCMD_PRESET_METHOD;

typedef enum
{
	CRUISE_START   = 1,								//巡航开始
	CRUISE_ADD		= 2,								//增加巡航点
	CRUISE_DEL		= 3,								//删除巡航点
	CRUISE_SPEED	= 4,								//设置巡航速度
	CRUISE_TIME	= 5									//设置巡航时间
}SUBCMD_CRUISE_METHOD;

typedef enum
{
	SCAN_START				= 1,						//扫描开始
	SCAN_LEFTBOUNDARY		= 2,						//扫描左边界
	SCAN_RIGHTBOUNDARY	= 3,						//扫描右边界
	SCAN_SPEED				= 4							//设置扫描速度
}SUBCMD_SCAN_METHOD;

typedef struct _SUBCMD_PTZ_						//云台控制子命令
{
	int 		pan;								//水平方向				向左 向右
	int 		pan_speed;							//水平速度				
	int			tilt;								//垂直方向			    向上 向下
	int 		tilt_speed;						//垂直速度
	int 		zoom;								//放大缩小				放大 缩小
	int 		zoom_speed;						//放大缩小速度
}SUBCMD_PTZ;

typedef struct _SUBCMD_FI_						//聚焦控制子命令
{
	int 		focus;								//聚焦					聚焦放大 聚焦缩小
	int 		focus_speed;						//聚焦速度
	int 		iris;								//光圈					光圈放大 光圈缩小
	int 		iris_speed;						//光圈速度
}SUBCMD_FI;

typedef struct _SUBCMD_PRESET_					//预置位子类型
{
	int 		preset;								//预置位命令	设置预置位 删除预置位 调用预置位
	int 		presetNO;							//预置位号
}SUBCMD_PRESET;

typedef struct _SUBCMD_CRUISE_					//巡航子类型
{
	int 		cruise;								//巡航指令		开始巡航 加入巡航点 删除巡航点 巡航速度 巡航停留时间
	int 		cruiseNO;							//巡航号
	int 		presetNO;							//预置位号
	int 		cruise_speed;						//巡航速度
	int 		cruise_stayTime;					//巡航停留时间
}SUBCMD_CRUISE;

typedef struct _SUBCMD_SCAN_						//扫描子类型
{
	int 		scan;								//扫描指令		开始扫描  左边界 右边界 扫描速度
	int 		scanNO;								//扫描号
	int 		scan_speed;						//扫描速度
}SUBCMD_SCAN;

typedef struct _SUBCMD_SWITCH_						//扫描子类型
{
	int 		switchCmd;								//扫描指令		开始扫描  左边界 右边界 扫描速度
	int 		switchNo;					
}SUBCMD_SWITCH;

/******************************************************************************************
*										定义回调类型
*******************************************************************************************/

typedef int(* GBT_GetPlatformInfo_CB)(void *p_platform);
typedef int(* GBT_SetPlatformInfo_CB)(void *p_platform);

typedef int(* GBT_PTZControl_CB)(int chn, int cmdType, void *p_ptzInfo, int reserve);
typedef int(* GBT_SetGuardStat_CB)(int chn, int stat, int reserve);   //stat 0:撤防  1:布防
typedef int(* GBT_HandRecord_CB)(int chn, int flag, int reserve);		//flag 0:手动录像结束  1:手动录像开始
typedef int(* GBT_ResetAlarm_CB)(int chn, int reserve);
typedef int(* GBT_TeleBoot_CB)(int chn, int reserve);				//通道号负数为本机
typedef int(* GBT_GetRecordFileList_CB)(int chn, void *p_fileList, int reserve);				//录像文件检索
typedef int(* GBT_GetDeviceStatus_CB)(int chn, void *p_devStatus, int reserve);				//设备状态
typedef int(* GBT_SetDeviceSwitchStatus_CB)(int chn, void *p_devStatus, int reserve); 			//设备状态
typedef int(* GBT_GetDeviceSwitchStatus_CB)(int chn, void *p_devSwitchStatus, int reserve);				//设备状态

typedef int(* GBT_UpdateGrade_CB)(void *p_devUpdateInfo, int reserve); 			//升级

typedef int(* GBT_UploadFile_CB)(void *p_uploadFileINfo, int reserve); 			//升级

typedef int(* GBT_GetDeviceInfo_CB)(int chn, void *p_devInfo, int reserve);					//设备信息
typedef int(* GBT_GetDeviceCatalog_CB)(int chn, void *p_devCatalogHead, int reserve);			//设备目录查询

typedef int(* GBT_OpenRealTimeChannel_CB)(int chn, int *p_handle, int reserve);
typedef int(* GBT_CloseRealTimeChannel_CB)(int handle);
typedef int(* GBT_GetRealTimeFrame_CB)(int handle, char *p_buff, int *p_len);

typedef int(* GBT_OpenRecordFile_CB)(int ch, unsigned int startTime, unsigned int endTime, int mode, int *p_handle, int reserve);
//ch 通道号 startTime 开始时间 endTime结束时间	mode 0:回放打开 1:下载打开
typedef int(* GBT_CloseRecordFile_CB)(int handle);
typedef int(* GBT_LseekRecordFile_CB)(int handle, unsigned int timeSeek);  //timeSeek 绝对时间
typedef int(* GBT_SetPlaySpeed_CB)(int handle, int speed);
typedef int(* GBT_PauseRecordFile_CB)(int handle, int flag);						//0 播放 1 暂停
typedef int(* GBT_GetHistoryFrame_CB)(int handle, char *p_buff, int *p_len);
typedef int(* GBT_GetRecordFrame_CB)(int handle, char *p_buff, int *p_len);

typedef int(* GBT_OpenAlarmChannel_CB)(void);									//打开告警通道
typedef int(* GBT_CloseAlarmChannel_CB)(void);									//关闭告警通道
typedef int(* GBT_GetAlarmMessage_CB)(void *p_alarmMsg);							//获取告警信息

typedef int(* GBT_SetDevTime_CB)(void *p_time);

typedef int(* GBT_Open_AudioChannel)(int chn, int *p_handle, int reserve);			//打开语音
typedef int(* GBT_GetAudioFrame_CB)(int handle, char *p_buff, int *p_len);			//获取语音
typedef int(* GBT_SendAudioFrame_CB)(int handle, char *p_buff, int *p_len);			//发送语音
typedef int(* GBT_Close_AudioChannel)(int handle);								//关闭语音

typedef int(* GBT_GetNetCardInfo_CB)(GBT_NetTrafic_T *pNetCard);
typedef int(* GBT_AlgoCfgSet_CB)(GBT_AlgoCfg_T *pAlgoCfg);
typedef int(* GBT_SetHomePosionInfo_CB)(GBT_HomePosion_T *pHomePosionInfo);
typedef int(* GBT_QueryPreset_CB)(GBT_PresetInfo_T *pPresetInfo);

/*********************************************************************************************
								    GBT库启动接口
**********************************************************************************************/
int ST_GBT_Start(); 
int ST_GBT_RegState();
int ST_GBT_UploadMsg(GBT_AlarmMsg_T alarmMsg);
int ST_GBT_NotifyUploadResult(GBT_UploadFile_T uploadMsg);

/*********************************************************************************************
								    平台参数回调注册接口
**********************************************************************************************/
int ST_GBT_RegisterPlatformParamCallBack(GBT_GetPlatformInfo_CB);		  	//获取平台参数
int ST_GBT_RegisterSetPlatformParamCallBack(GBT_SetPlatformInfo_CB);		//设置平台参数

int ST_GBT_RegisterGetFTPParamCallBack();									//获取ftp路径
int ST_GBT_RegisterSetFTPParamCallBack();									//设置ftp路径
int ST_GBT_RegisterSnapshotCallBack();										//抓拍

int ST_GBT_RegisterSetSerialParamCallBack();									//设置串口参数
int ST_GBT_RegisterGetSerialParamCallBack();									//获取串口参数

int ST_GBT_RegisterSetTimeNowCallBack();										//设置当前时间
int ST_GBT_RegisterGetTimeNowCallBack();									//后去当前时间

int ST_GBT_RegisterSetTimeConfigCallBack();									//设置时间配置
int ST_GBT_RegisterGetTimeConfigCallBack();									//获取时间配置

int ST_GBT_RegisterSetNetworkParamCallBack();								//设置网络参数
int ST_GBT_RegisterGetNetworkParamCallBack();								//获取网络参数

int ST_GBT_RegisterSetDiskAlarmParamCallBack();								//设置硬盘告警参数
int ST_GBT_RegisterGetDiskAlarmParamCallBack();								//查询硬盘告警参数

int ST_GBT_RegisterSetVideoLostAlarmParamCallBack();						//设置视频丢失参数
int ST_GBT_RegisterGetVideoLostAlarmParamCallBack();						//查询视频丢失参数

int ST_GBT_RegisterSetVideoCoveredParamCallBack();						//设置视频遮挡参数
int ST_GBT_RegisterGetVideoCoveredParamCallBack();						//查询视频遮挡参数

int ST_GBT_RegisterSetMotionDectionParamCallBack();						//设置移动侦测参数
int ST_GBT_RegisterGetMotionDectionParamCallBack();						//查询移动侦测参数

int ST_GBT_RegisterSetDisplayParamCallBack();								//设置显示参数
int ST_GBT_RegisterGetDisplayParamCallBack();								//查询显示参数

int ST_GBT_RegisterSetOSDParamCallBack();									//设置字符叠加参数
int ST_GBT_RegisterGetOSDParamCallBack();									//查询字符叠加参数

int ST_GBT_RegisterSetStorageSchemaParamCallBack();						//设置存储策略
int ST_GBT_RegisterGetStorageSchemaParamCallBack();						//查询存储策略

int ST_GBT_RegisterSetVideoEncoderParamCallBack();						//设置视频编码参数
int ST_GBT_RegisterGetVideoEncoderParamCallBack();						//查询视频编码参数


/*********************************************************************************************
									设备控制和查询回调注册接口
**********************************************************************************************/
int ST_GBT_RegisterPTZControlCallBack(GBT_PTZControl_CB);				  	//云台控制
int ST_GBT_RegisterSetGuardStatCallBack(GBT_SetGuardStat_CB);			  	//布防撤防
int ST_GBT_RegisterHandRecordCallBack(GBT_HandRecord_CB);				  	//手动录像
int ST_GBT_RegisterResetAlarmCallBack(GBT_ResetAlarm_CB);				  	//告警复位
int ST_GBT_RegisterTeleBootCallBack(GBT_TeleBoot_CB);						  	//远程重启
int ST_GBT_RegisterGetRecordFileListCallBack(GBT_GetRecordFileList_CB);	//获取文件列表

/******************************************************************************************************
									设备信息查询接口
******************************************************************************************************/
int ST_GBT_RegisterGetDeviceCatalogCallBack(GBT_GetDeviceCatalog_CB);
int ST_GBT_RegisterGetDeviceStatusCallBack(GBT_GetDeviceStatus_CB);
int ST_GBT_RegisterGetDeviceInfoCallBack(GBT_GetDeviceInfo_CB);

/******************************************************************************************************
									实时流回调注册接口
******************************************************************************************************/
int ST_GBT_RegisterOpenRealTimeChannelCallBack(GBT_OpenRealTimeChannel_CB);
int ST_GBT_RegisterGetRealTimeFrameCallBack(GBT_GetRealTimeFrame_CB);  	//实时流回调注册接口
int ST_GBT_RegisterCloseRealTimeChannelCallBack(GBT_CloseRealTimeChannel_CB);

/******************************************************************************************************
									历史回放和下载回调注册接口
******************************************************************************************************/
int ST_GBT_RegisterOpenRecordFileCallBack(GBT_OpenRecordFile_CB);		//打开录像文件的注册回调接口
int ST_GBT_RegisterCloseRecordFileCallBack(GBT_CloseRecordFile_CB);	//关闭录像文件的注册回调接口
int ST_GBT_RegisterLseekRecordFileCallBack(GBT_LseekRecordFile_CB);	//控制读写指针
int ST_GBT_RegisterSetPlaySpeedCallBack(GBT_SetPlaySpeed_CB);
int ST_GBT_RegisterPauseRecordFileCallBack(GBT_PauseRecordFile_CB);
int ST_GBT_RegisterGetRecordFrameCallBack(GBT_GetRecordFrame_CB);	//读取录像文件一帧的内容
int ST_GBT_RegisterGetHistoryFrameCallBack(GBT_GetHistoryFrame_CB);	//历史流注册回调接口

/******************************************************************************************************
									告警通道回调注册接口
******************************************************************************************************/
int ST_GBT_RegisterOpenAlarmChannelCallBack(GBT_OpenAlarmChannel_CB);
int ST_GBT_RegisterGetAlarmMessageCallBack(GBT_GetAlarmMessage_CB);
int ST_GBT_RegisterCloseAlarmChannelCallBack(GBT_CloseAlarmChannel_CB);

/********************************************************************************************************
									校时回调注册接口
*********************************************************************************************************/
int ST_GBT_RegisterSetDevTimeCallBack(GBT_SetDevTime_CB);

/*******************************************************************************************************
									对讲通道
********************************************************************************************************/
int ST_GBT_RegisterOpenAudioChannelCallBack(GBT_Open_AudioChannel);
int ST_GBT_RegisterCloseAudioChannelCallBack(GBT_Close_AudioChannel);
int ST_GBT_RegisterGetAudioFrameCallBack(GBT_GetAudioFrame_CB);
int ST_GBT_RegisterSendAudioFrameCallBack(GBT_SendAudioFrame_CB);

int ST_GBT_RegisterGetDeviceSwitchStatusCallBack(GBT_GetDeviceSwitchStatus_CB get_deviceSwitchStatus);
int ST_GBT_RegisterSetDeviceSwitchStatusCallBack(GBT_SetDeviceSwitchStatus_CB set_deviceSwitchStatus);
int ST_GBT_RegisterUpdateGradeCallBack(GBT_UpdateGrade_CB UpdateGrade);
int ST_GBT_RegisterUploadFileInfo(GBT_UploadFile_CB UploadFile);

int ST_GBT_RegisterNetCardInfo(GBT_GetNetCardInfo_CB GetNetCardInfo_CB);

int ST_GBT_RegisterAlgoCfgSet(GBT_AlgoCfgSet_CB  AlgoCfgSet_CB);

int ST_GBT_UpdateChannStatus(int nChann,int iStatus);	//更新通道在线状态

int ST_GBT_SetHomePosionInfoCallBack(GBT_SetHomePosionInfo_CB SetHomePosionInfo_CB);
int ST_GBT_RegisterQueryPresetInfoCallBack(GBT_QueryPreset_CB QueryPreset_CB);

#ifdef __cplusplus
}
#endif

#endif

