#ifndef __GBTLIB_H__
#define __GBTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#define		GBT_CODE_SIZE						32			//�������鳤��
#define 	GBT_PASSWD_SIZE					32			//�������鳤��
#define 	GBT_USERNAME_SIZE					32			//�û�������
#define 	GBT_IP_SIZE						32			//IP���鳤��
#define 	GBT_DOMAIN_SIZE					16			//�����鳤��
#define 	GBT_FILE_PATH_LEN					256			//�ļ�������
#define 	GBT_MAX_RECORD_COUNT				256			//һ�β�ѯ¼���ļ�����ץ��ͼƬ�������
#define		GBT_STRING_LEN						32			//�ַ���
#define		GBT_DESCRIPTION_LEN				128			//�����ַ�������
#define		GBT_MAX_CAMERA_COUNT				64			//�����Ƶͨ������
#define		GBT_MAX_ALARMIN_COUNT				8			//���澯��������
#define		GBT_MAX_ALARMOUT_COUNT			8			//���澯�������
#define 	GBT_MAX_CATLOGITEM			    (GBT_MAX_CAMERA_COUNT + GBT_MAX_ALARMIN_COUNT + GBT_MAX_ALARMOUT_COUNT)	//�豸Ŀ¼�������
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
		regState value  ע���3��״ֵ̬
******************************************************/

#define 	NON_REGISTER 				0x00	//û��ע��
#define		NOW_REGISTER				0x01	//����ע��			
#define 	DONE_REGISTER 				0x02	//ע��ɹ�
#define		REQUEST_LOGOUT				0x04	//����ע��
#define		NOW_LOGOUT					0x08	//���͵�һ��ע���ɹ�
#define		DONE_LOGOUT				0x10	//ע���ɹ�
#define 	READY_REGISTER				0x20	//ÿ��׼��ע��ǰ��Ҫ��ע�����豸
#define 	RUNNING_REGISTER			0x40	//��������״̬
#define 	REQUEST_READY_REGISTER	0x80
/*****************************************************
			Guard value  ע���2��״ֵ̬
******************************************************/
typedef enum
{
	GUARD_RESET = 0,						//����
	GUARD_SET = 1							//����
}GUARD_T;

/*****************************************************
		Hand Record value  ע���2��״ֵ̬
******************************************************/
typedef enum
{
	HAND_RECORD_STOP = 0,							//�ֶ�¼��ʼ
	HAND_RECORD_START = 1							//�ֶ�¼�����
}HAND_RECORD_T;


typedef struct _GBT_ChnInfo_T_				//ͨ����Ϣ��������Ƶͨ������������ͨ�����������ͨ��
{
	int 	chnType;						//ͨ������
	int     chnNo;							//ͨ����			//��1��ʼ
	char    chnID[GBT_CODE_SIZE];			//ͨ��ID
	char 	chnName[GBT_STRING_LEN];		//ͨ������
}GBT_ChnInfo;

typedef struct _GBT_NetTrafic_T_
{
	char CardNo[GBT_MAX_NETCARDNO_SIZE];
	char CardType[GBT_MAX_NETCARDTYPE_SIZE];
	int CardStatus;
	int	LastMonthTotal;
	int	TotalUsed;
}GBT_NetTrafic_T;

typedef struct _GBT_Platform_T_				//ƽ̨����
{
	int			GBT_enable;						//0:��ʹ�� 1:ʹ��   ƽ̨ʹ�ܱ�־
	int  		connectType;						//����Э��    0:tcp  1:udp
	
	char 		platformIP[GBT_IP_SIZE];    		//ƽ̨IP��ַ
	int  		platformPort;						//ƽ̨�˿�    5060
	char 		platformCode[GBT_CODE_SIZE];   	//ƽ̨����

	char 		DevDomainID[GBT_DOMAIN_SIZE];		//�豸��ID
	char 		DevCode[GBT_CODE_SIZE];  			//�豸��������豸ID
	char		DevAlarmCode[GBT_CODE_SIZE];		//�豸�澯ҵ�����
	char 		password[GBT_PASSWD_SIZE];  		//�豸����,����ƽ̨��֤
	char 		userName[GBT_USERNAME_SIZE];
	int			expiresTime;						//ע�����Ч����
	int			keepAliveTIme;						//������������

	int 		cameraCount;						//��Ƶͨ������
	GBT_ChnInfo cameraInfo[GBT_MAX_CAMERA_COUNT];
	
	int 		alarmInCount;							//�澯����ͨ������
	GBT_ChnInfo alarmInInfo[GBT_MAX_ALARMIN_COUNT];	
		
	int 		alarmOutCount;							//�澯���ͨ������
	GBT_ChnInfo alarmOutInfo[GBT_MAX_ALARMOUT_COUNT];

	int			audioIn_enable;
	GBT_ChnInfo audioInInfo;

	int			audioOut_enable;
	GBT_ChnInfo audioOutInfo;	
}GBT_Platform_T;

typedef struct _GBT_Time_T_		
{
	int 	year;   				//��
	int 	month;					//��
	int 	day;					//��
	int 	hour;					//ʱ
	int 	minute;					//��
	int 	second;					//��
}GBT_Time_T;

typedef struct _GBT_RecordFile_T_					//¼���ļ���Ϣ�ṹ��
{
	unsigned int 	fileStartTime;						//����ʱ��
	unsigned int 	fileEndTime;
	GBT_Time_T 		timeStart;							//������ʱ���� ʱ��
	GBT_Time_T 		timeEnd;
	int 			fileSize;
	int 			recordType;     					//012λ�������ֶ�����ʱ���澯
	char 			filePath[GBT_FILE_PATH_LEN];		//�ļ�·��
}GBT_RecordFile_T;

typedef struct _GBT_RecordList_T_					//¼���ļ�����ץ��ͼƬ�б�ṹ��
{
	int 				fileType;						//�ļ����� 0: ¼���ļ� 1: ͼƬ
	int 				recordType;    				//0:�ֶ� 1:��ʱ 2:�澯
	unsigned int 		beginTime;						//��ѯ��ʼʱ��
	unsigned int 		endTime;						//��ѯ����ʱ��
	int 				fileNum;						//�ļ�����
	GBT_RecordFile_T 	recordFile[GBT_MAX_RECORD_COUNT];
}GBT_RecordList_T;


typedef struct _GBT_AlarmMsg_T_
{
	int 		alarmChannel;											//����ͨ��
	int 		alarmPriority;											//��������
	int 		alarmMethod;											//������ʽ
	GBT_Time_T	alarmTime;												//����ʱ��
	int			sn;
	int 		UploadType;
	char		alarmType[GBT_ALARMTYPE_SIZE];							//AI ��������
	char 		alarmDescription[GBT_DESCRIPTION_LEN];				//��������
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
	char 	alarmCode[GBT_CODE_SIZE];					//��������
	int 	status;										//����״̬    0:���� 1:���� 2:����
}GBT_AlarmStatus_T;

typedef struct _GBT_AlarmAIStatus_T_
{
	char 	ChannCode[GBT_CODE_SIZE];					//��������
	int 	status;										//����״̬    0:�Ѿ��ϱ�������  1: �����ϴ�pic, 2:�ϴ����
}GBT_AlarmAIStatus_T;

typedef struct _GBT_DeviceStatus_T_					//�豸״̬��ѯ
{
	int 				onlineStatus;								//�Ƿ�����				1:����     0:������
	int 				workStatus;								//�Ƿ���������			1:�������� 0:����������
	char 				reason[GBT_DESCRIPTION_LEN];				//����������ԭ��		
	int					encodeStatus;								//�Ƿ����				1:���ڱ��� 0:û�б���
	int     			recordStatus;								//¼��״̬				1:����¼�� 0:û��¼��
	GBT_Time_T			timeNow;									//�豸��ǰʱ��			
	int 				alarmCount;								//����ͨ������
	GBT_AlarmStatus_T   alarmStatus[GBT_MAX_ALARMIN_COUNT];		//����ͨ��״̬
	int 				netCardCount;
	GBT_NetTrafic_T		netCardInfo[GBT_MAX_NETCNT];
}GBT_DeviceStatus_T;

typedef struct _GBT_SwitchStatus_T_
{
	char 	switchCode[GBT_CODE_SIZE];					//��������
	char	switchNo[GBT_STRING_LEN];
	int 	status;										//����״̬    0: off 1:on
}GBT_SwitchStatus_T;

typedef struct _GBT_DeviceSwitchStatus_T_					//�豸״̬��ѯ
{
	GBT_Time_T			timeNow;									//�豸��ǰʱ��			
	int 				switchCount;								//������״̬��ѯ
	GBT_SwitchStatus_T   alarmStatus[GBT_MAX_ALARMIN_COUNT];		//����ͨ��״̬
}GBT_DeviceSwitchStatus_T;


typedef struct _GBT_DeviceInfo_T_						//�豸��Ϣ��ѯ
{
	char 			manufacturer[GBT_STRING_LEN];			//�豸������
	char 			deviceType[GBT_STRING_LEN];			//�豸����
	char			model[GBT_STRING_LEN];					//�豸�ͺ�
	char 			firmware[GBT_STRING_LEN];				//����汾
	int 			channel;								//����ͨ����
	int 			maxCamera;								//�������ͨ��
	int 			maxAlarm;								//���澯ͨ����
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
	char 			deviceCode[GBT_CODE_SIZE];				//�豸����
	char			name[GBT_STRING_LEN];					//�豸����
	char 			manufacturer[GBT_STRING_LEN];			//�����̼�
	char			model[GBT_STRING_LEN];					//�豸�ͺ�
	char			owner[GBT_STRING_LEN];					//�豸����
	char 			civilCode[GBT_STRING_LEN];				//��������
	char 			block[GBT_STRING_LEN];					//����
	char			address[GBT_STRING_LEN];				//��װ��ַ
	int				parental;								//�Ƿ������豸 0:û�� 1:��
	char			parentCode[GBT_CODE_SIZE];				//���豸ID
	int				safetyWay;								//���ȫģʽ
	int				registerWay;							//ע�᷽ʽ
	char			certNum[GBT_CODE_SIZE];					//֤�����к�
	int				certifiable;							//֤����Ч��ʶ
	int 			errCode;								//��Чԭ����
	GBT_Time_T		endTime;								//֤����ֹ��Ч��
	int 			secrecy;								//��������
	char 			deviceIP[GBT_IP_SIZE];					//�豸IP
	int 			port;									//�豸�˿�
	char 			password[GBT_PASSWD_SIZE];				//�豸����
	int 			status;									//�豸״̬
	double			logitude;								//����
	double			latitude;								//γ��
}GBT_CatlogItem_T;

typedef struct _GBT_DeviceCatalog_T_				//�豸Ŀ¼��ѯ
{
	int 				catalogCount;						//Ŀ¼����
	GBT_CatlogItem_T	catalogItem[GBT_MAX_CATLOGITEM];	//Ŀ¼����
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
* ��������
*******************************************************************************************/

typedef enum
{
	CMD_STOP_PTZ	= 0,								//ֹͣɨ�� Ѳ�� PTZ
	CMD_PTZ 		= 1,								//��̨����ָ��
	CMD_FI		 	= 2,								//�۽� ��Ȧ
	CMD_PRESET 	= 3,								//Ԥ��λָ��
	CMD_CRUISE 	= 4,								//Ѳ��ָ��
	CMD_SCAN 		= 5, 								//ɨ��ָ��	
	CMD_SWITCH_ON	 =6,
	CMD_SWITCH_OFF	 =7
	
}CMD_METHOD;

typedef enum											//ˮƽ����
{		
	PAN_NULL		= 0, 								//ˮƽ���򲻶�
	PAN_LEFT 		= 1,								//����
	PAN_RIGHT		= 2									//����
}SUBCMD_PAN;

typedef enum											//��ֱ����
{	
	TILT_NULL		= 0,
	TILT_UP 		= 1,								//����		
	TILT_DOWN		= 2									//����
}SUBCMD_TILT;

typedef enum											//�Ŵ���С
{	
	ZOOM_NULL		= 0,
	ZOOM_IN 		= 1,								//�Ŵ�		
	ZOOM_OUT		= 2									//��С
}SUBCMD_ZOOM;

typedef enum											//�۽�
{	
	FOCUS_NULL		= 0,
	FOCUS_ADD 		= 1,								//�۽���		
	FOCUS_SUB		= 2									//�۽���
}SUBCMD_FOCUS;

typedef enum											//��Ȧ
{	
	IRIS_NULL		= 0,
	IRIS_ADD		= 1,								//��Ȧ+
	IRIS_SUB		= 2									//��Ȧ-
}SUBCMD_IRIS;

typedef enum 
{
	PRESET_SET		= 1,								//����Ԥ��λ
	PRESET_DEL		= 2,								//ɾ��Ԥ��λ
	PRESET_USE		= 3									//����Ԥ��λ
}SUBCMD_PRESET_METHOD;

typedef enum
{
	CRUISE_START   = 1,								//Ѳ����ʼ
	CRUISE_ADD		= 2,								//����Ѳ����
	CRUISE_DEL		= 3,								//ɾ��Ѳ����
	CRUISE_SPEED	= 4,								//����Ѳ���ٶ�
	CRUISE_TIME	= 5									//����Ѳ��ʱ��
}SUBCMD_CRUISE_METHOD;

typedef enum
{
	SCAN_START				= 1,						//ɨ�迪ʼ
	SCAN_LEFTBOUNDARY		= 2,						//ɨ����߽�
	SCAN_RIGHTBOUNDARY	= 3,						//ɨ���ұ߽�
	SCAN_SPEED				= 4							//����ɨ���ٶ�
}SUBCMD_SCAN_METHOD;

typedef struct _SUBCMD_PTZ_						//��̨����������
{
	int 		pan;								//ˮƽ����				���� ����
	int 		pan_speed;							//ˮƽ�ٶ�				
	int			tilt;								//��ֱ����			    ���� ����
	int 		tilt_speed;						//��ֱ�ٶ�
	int 		zoom;								//�Ŵ���С				�Ŵ� ��С
	int 		zoom_speed;						//�Ŵ���С�ٶ�
}SUBCMD_PTZ;

typedef struct _SUBCMD_FI_						//�۽�����������
{
	int 		focus;								//�۽�					�۽��Ŵ� �۽���С
	int 		focus_speed;						//�۽��ٶ�
	int 		iris;								//��Ȧ					��Ȧ�Ŵ� ��Ȧ��С
	int 		iris_speed;						//��Ȧ�ٶ�
}SUBCMD_FI;

typedef struct _SUBCMD_PRESET_					//Ԥ��λ������
{
	int 		preset;								//Ԥ��λ����	����Ԥ��λ ɾ��Ԥ��λ ����Ԥ��λ
	int 		presetNO;							//Ԥ��λ��
}SUBCMD_PRESET;

typedef struct _SUBCMD_CRUISE_					//Ѳ��������
{
	int 		cruise;								//Ѳ��ָ��		��ʼѲ�� ����Ѳ���� ɾ��Ѳ���� Ѳ���ٶ� Ѳ��ͣ��ʱ��
	int 		cruiseNO;							//Ѳ����
	int 		presetNO;							//Ԥ��λ��
	int 		cruise_speed;						//Ѳ���ٶ�
	int 		cruise_stayTime;					//Ѳ��ͣ��ʱ��
}SUBCMD_CRUISE;

typedef struct _SUBCMD_SCAN_						//ɨ��������
{
	int 		scan;								//ɨ��ָ��		��ʼɨ��  ��߽� �ұ߽� ɨ���ٶ�
	int 		scanNO;								//ɨ���
	int 		scan_speed;						//ɨ���ٶ�
}SUBCMD_SCAN;

typedef struct _SUBCMD_SWITCH_						//ɨ��������
{
	int 		switchCmd;								//ɨ��ָ��		��ʼɨ��  ��߽� �ұ߽� ɨ���ٶ�
	int 		switchNo;					
}SUBCMD_SWITCH;

/******************************************************************************************
*										����ص�����
*******************************************************************************************/

typedef int(* GBT_GetPlatformInfo_CB)(void *p_platform);
typedef int(* GBT_SetPlatformInfo_CB)(void *p_platform);

typedef int(* GBT_PTZControl_CB)(int chn, int cmdType, void *p_ptzInfo, int reserve);
typedef int(* GBT_SetGuardStat_CB)(int chn, int stat, int reserve);   //stat 0:����  1:����
typedef int(* GBT_HandRecord_CB)(int chn, int flag, int reserve);		//flag 0:�ֶ�¼�����  1:�ֶ�¼��ʼ
typedef int(* GBT_ResetAlarm_CB)(int chn, int reserve);
typedef int(* GBT_TeleBoot_CB)(int chn, int reserve);				//ͨ���Ÿ���Ϊ����
typedef int(* GBT_GetRecordFileList_CB)(int chn, void *p_fileList, int reserve);				//¼���ļ�����
typedef int(* GBT_GetDeviceStatus_CB)(int chn, void *p_devStatus, int reserve);				//�豸״̬
typedef int(* GBT_SetDeviceSwitchStatus_CB)(int chn, void *p_devStatus, int reserve); 			//�豸״̬
typedef int(* GBT_GetDeviceSwitchStatus_CB)(int chn, void *p_devSwitchStatus, int reserve);				//�豸״̬

typedef int(* GBT_UpdateGrade_CB)(void *p_devUpdateInfo, int reserve); 			//����

typedef int(* GBT_UploadFile_CB)(void *p_uploadFileINfo, int reserve); 			//����

typedef int(* GBT_GetDeviceInfo_CB)(int chn, void *p_devInfo, int reserve);					//�豸��Ϣ
typedef int(* GBT_GetDeviceCatalog_CB)(int chn, void *p_devCatalogHead, int reserve);			//�豸Ŀ¼��ѯ

typedef int(* GBT_OpenRealTimeChannel_CB)(int chn, int *p_handle, int reserve);
typedef int(* GBT_CloseRealTimeChannel_CB)(int handle);
typedef int(* GBT_GetRealTimeFrame_CB)(int handle, char *p_buff, int *p_len);

typedef int(* GBT_OpenRecordFile_CB)(int ch, unsigned int startTime, unsigned int endTime, int mode, int *p_handle, int reserve);
//ch ͨ���� startTime ��ʼʱ�� endTime����ʱ��	mode 0:�طŴ� 1:���ش�
typedef int(* GBT_CloseRecordFile_CB)(int handle);
typedef int(* GBT_LseekRecordFile_CB)(int handle, unsigned int timeSeek);  //timeSeek ����ʱ��
typedef int(* GBT_SetPlaySpeed_CB)(int handle, int speed);
typedef int(* GBT_PauseRecordFile_CB)(int handle, int flag);						//0 ���� 1 ��ͣ
typedef int(* GBT_GetHistoryFrame_CB)(int handle, char *p_buff, int *p_len);
typedef int(* GBT_GetRecordFrame_CB)(int handle, char *p_buff, int *p_len);

typedef int(* GBT_OpenAlarmChannel_CB)(void);									//�򿪸澯ͨ��
typedef int(* GBT_CloseAlarmChannel_CB)(void);									//�رո澯ͨ��
typedef int(* GBT_GetAlarmMessage_CB)(void *p_alarmMsg);							//��ȡ�澯��Ϣ

typedef int(* GBT_SetDevTime_CB)(void *p_time);

typedef int(* GBT_Open_AudioChannel)(int chn, int *p_handle, int reserve);			//������
typedef int(* GBT_GetAudioFrame_CB)(int handle, char *p_buff, int *p_len);			//��ȡ����
typedef int(* GBT_SendAudioFrame_CB)(int handle, char *p_buff, int *p_len);			//��������
typedef int(* GBT_Close_AudioChannel)(int handle);								//�ر�����

typedef int(* GBT_GetNetCardInfo_CB)(GBT_NetTrafic_T *pNetCard);
typedef int(* GBT_AlgoCfgSet_CB)(GBT_AlgoCfg_T *pAlgoCfg);
typedef int(* GBT_SetHomePosionInfo_CB)(GBT_HomePosion_T *pHomePosionInfo);
typedef int(* GBT_QueryPreset_CB)(GBT_PresetInfo_T *pPresetInfo);

/*********************************************************************************************
								    GBT�������ӿ�
**********************************************************************************************/
int ST_GBT_Start(); 
int ST_GBT_RegState();
int ST_GBT_UploadMsg(GBT_AlarmMsg_T alarmMsg);
int ST_GBT_NotifyUploadResult(GBT_UploadFile_T uploadMsg);

/*********************************************************************************************
								    ƽ̨�����ص�ע��ӿ�
**********************************************************************************************/
int ST_GBT_RegisterPlatformParamCallBack(GBT_GetPlatformInfo_CB);		  	//��ȡƽ̨����
int ST_GBT_RegisterSetPlatformParamCallBack(GBT_SetPlatformInfo_CB);		//����ƽ̨����

int ST_GBT_RegisterGetFTPParamCallBack();									//��ȡftp·��
int ST_GBT_RegisterSetFTPParamCallBack();									//����ftp·��
int ST_GBT_RegisterSnapshotCallBack();										//ץ��

int ST_GBT_RegisterSetSerialParamCallBack();									//���ô��ڲ���
int ST_GBT_RegisterGetSerialParamCallBack();									//��ȡ���ڲ���

int ST_GBT_RegisterSetTimeNowCallBack();										//���õ�ǰʱ��
int ST_GBT_RegisterGetTimeNowCallBack();									//��ȥ��ǰʱ��

int ST_GBT_RegisterSetTimeConfigCallBack();									//����ʱ������
int ST_GBT_RegisterGetTimeConfigCallBack();									//��ȡʱ������

int ST_GBT_RegisterSetNetworkParamCallBack();								//�����������
int ST_GBT_RegisterGetNetworkParamCallBack();								//��ȡ�������

int ST_GBT_RegisterSetDiskAlarmParamCallBack();								//����Ӳ�̸澯����
int ST_GBT_RegisterGetDiskAlarmParamCallBack();								//��ѯӲ�̸澯����

int ST_GBT_RegisterSetVideoLostAlarmParamCallBack();						//������Ƶ��ʧ����
int ST_GBT_RegisterGetVideoLostAlarmParamCallBack();						//��ѯ��Ƶ��ʧ����

int ST_GBT_RegisterSetVideoCoveredParamCallBack();						//������Ƶ�ڵ�����
int ST_GBT_RegisterGetVideoCoveredParamCallBack();						//��ѯ��Ƶ�ڵ�����

int ST_GBT_RegisterSetMotionDectionParamCallBack();						//�����ƶ�������
int ST_GBT_RegisterGetMotionDectionParamCallBack();						//��ѯ�ƶ�������

int ST_GBT_RegisterSetDisplayParamCallBack();								//������ʾ����
int ST_GBT_RegisterGetDisplayParamCallBack();								//��ѯ��ʾ����

int ST_GBT_RegisterSetOSDParamCallBack();									//�����ַ����Ӳ���
int ST_GBT_RegisterGetOSDParamCallBack();									//��ѯ�ַ����Ӳ���

int ST_GBT_RegisterSetStorageSchemaParamCallBack();						//���ô洢����
int ST_GBT_RegisterGetStorageSchemaParamCallBack();						//��ѯ�洢����

int ST_GBT_RegisterSetVideoEncoderParamCallBack();						//������Ƶ�������
int ST_GBT_RegisterGetVideoEncoderParamCallBack();						//��ѯ��Ƶ�������


/*********************************************************************************************
									�豸���ƺͲ�ѯ�ص�ע��ӿ�
**********************************************************************************************/
int ST_GBT_RegisterPTZControlCallBack(GBT_PTZControl_CB);				  	//��̨����
int ST_GBT_RegisterSetGuardStatCallBack(GBT_SetGuardStat_CB);			  	//��������
int ST_GBT_RegisterHandRecordCallBack(GBT_HandRecord_CB);				  	//�ֶ�¼��
int ST_GBT_RegisterResetAlarmCallBack(GBT_ResetAlarm_CB);				  	//�澯��λ
int ST_GBT_RegisterTeleBootCallBack(GBT_TeleBoot_CB);						  	//Զ������
int ST_GBT_RegisterGetRecordFileListCallBack(GBT_GetRecordFileList_CB);	//��ȡ�ļ��б�

/******************************************************************************************************
									�豸��Ϣ��ѯ�ӿ�
******************************************************************************************************/
int ST_GBT_RegisterGetDeviceCatalogCallBack(GBT_GetDeviceCatalog_CB);
int ST_GBT_RegisterGetDeviceStatusCallBack(GBT_GetDeviceStatus_CB);
int ST_GBT_RegisterGetDeviceInfoCallBack(GBT_GetDeviceInfo_CB);

/******************************************************************************************************
									ʵʱ���ص�ע��ӿ�
******************************************************************************************************/
int ST_GBT_RegisterOpenRealTimeChannelCallBack(GBT_OpenRealTimeChannel_CB);
int ST_GBT_RegisterGetRealTimeFrameCallBack(GBT_GetRealTimeFrame_CB);  	//ʵʱ���ص�ע��ӿ�
int ST_GBT_RegisterCloseRealTimeChannelCallBack(GBT_CloseRealTimeChannel_CB);

/******************************************************************************************************
									��ʷ�طź����ػص�ע��ӿ�
******************************************************************************************************/
int ST_GBT_RegisterOpenRecordFileCallBack(GBT_OpenRecordFile_CB);		//��¼���ļ���ע��ص��ӿ�
int ST_GBT_RegisterCloseRecordFileCallBack(GBT_CloseRecordFile_CB);	//�ر�¼���ļ���ע��ص��ӿ�
int ST_GBT_RegisterLseekRecordFileCallBack(GBT_LseekRecordFile_CB);	//���ƶ�дָ��
int ST_GBT_RegisterSetPlaySpeedCallBack(GBT_SetPlaySpeed_CB);
int ST_GBT_RegisterPauseRecordFileCallBack(GBT_PauseRecordFile_CB);
int ST_GBT_RegisterGetRecordFrameCallBack(GBT_GetRecordFrame_CB);	//��ȡ¼���ļ�һ֡������
int ST_GBT_RegisterGetHistoryFrameCallBack(GBT_GetHistoryFrame_CB);	//��ʷ��ע��ص��ӿ�

/******************************************************************************************************
									�澯ͨ���ص�ע��ӿ�
******************************************************************************************************/
int ST_GBT_RegisterOpenAlarmChannelCallBack(GBT_OpenAlarmChannel_CB);
int ST_GBT_RegisterGetAlarmMessageCallBack(GBT_GetAlarmMessage_CB);
int ST_GBT_RegisterCloseAlarmChannelCallBack(GBT_CloseAlarmChannel_CB);

/********************************************************************************************************
									Уʱ�ص�ע��ӿ�
*********************************************************************************************************/
int ST_GBT_RegisterSetDevTimeCallBack(GBT_SetDevTime_CB);

/*******************************************************************************************************
									�Խ�ͨ��
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

int ST_GBT_UpdateChannStatus(int nChann,int iStatus);	//����ͨ������״̬

int ST_GBT_SetHomePosionInfoCallBack(GBT_SetHomePosionInfo_CB SetHomePosionInfo_CB);
int ST_GBT_RegisterQueryPresetInfoCallBack(GBT_QueryPreset_CB QueryPreset_CB);

#ifdef __cplusplus
}
#endif

#endif

