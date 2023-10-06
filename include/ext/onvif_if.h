#ifndef _ONVIF_IF_H
#define _ONVIF_IF_H

#define MAX_UUID_LEN	64
#define MAX_HOST_IP_LEN	32
#define MAX_HOSTNAME_LEN	128
#define MEDIAPROFILE_MAXNUM 2
#define NETWORKPROTOCOLSNUM 2
#define MAX_OSD_NUM		6
#define MAX_OSD_PLAINTEXT_LEN		128
///////////////////// onvif 相关的结构体 //////////////////////

typedef struct ttaddress
{
    char type[8];
	char ipv4address[16];
}ttaddress_t;

typedef struct ttmulticast
{    
	char port[8];
	char ttl[8];
	char autostart[8];
	ttaddress_t address;
}ttmulticast_t;


typedef struct ttanalytics
{
    char rulesupport[8];
	char analyticsmodulesupport[8];
    char xaddr[64];
}ttanalytics_t;


typedef struct ttnetwork
{
    char ipfilter[8];
	char zeroconfiguration[8];
	char ipversion6[8];
	char dyndns[8];
}ttnetwork_t;

typedef struct ttsupportedversions
{
    char major[8];
	char minor[8];
}ttsupportedversions_t;

typedef struct ttextension
{
    char httpfirmwareupgrade[8];
	char httpsystembackup[8];
}ttextension_t;

typedef struct ttsystem
{
    char discoveryresolve[8];
	char discoverybye[8];
	char remotediscovery[8];
	char systembackup[8];
	char systemlogging[8];
	char firmwareupgrade[8];
	ttsupportedversions_t supportedversions;
	ttextension_t extension;
}ttsystem_t;

typedef struct ttio
{
    char inputconnectors[8];
	char relayoutputs[8];
}ttio_t;

typedef struct ttsecurity
{
    char tls1_1[8];
	char tls1_2[8];
	char onboardkeygeneration[8];
	char accesspolicyconfig[8];
	char x_509token[8];
	char samltoken[8];
	char kerberostoken[8];
	char reltoken[8];
	char dot1x[8];
	char tls1_0[8];
	char HttpDigest[8];
	char UsernameToken[8];
	char SupportedEAPMethods[16];
}ttsecurity_t;

typedef struct ttdevice
{
    char xaddr[64];
	ttnetwork_t network;
	ttsystem_t system;
	ttio_t io;
	ttsecurity_t security;
}ttdevice_t;

typedef struct ttevents
{
    char xaddr[64];
	char wssubscriptionpolicysupport[8];
	char wspullpointsupport[8];
	char wspausablesubscriptionmanagerinterfacesupport[8];
}ttevents_t;

typedef struct ttimaging
{
    char xaddr[64];
}ttimaging_t;

typedef struct ttstreamingcapabilities
{
    char rtpmulticast[8];
	char rtp_tcp[8];
	char rtp_rtsp_tcp[8];
}ttstreamingcapabilities_t;

typedef struct ttprofilecapabilities
{
    char maximumnumberofprofiles[8];
}ttprofilecapabilities_t;

typedef struct ttmedia
{
    char xaddr[64];
	ttstreamingcapabilities_t streamingcapabilities;
	ttprofilecapabilities_t profilecapabilities;
}ttmedia_t;

typedef struct ttptz
{
    char xaddr[64];
}ttptz_t;

typedef struct ttcapabilities
{
    ttanalytics_t analytics;
	ttdevice_t device;
	ttevents_t events;
	ttimaging_t imaging;
	ttmedia_t media;
	ttptz_t ptz;
}ttcapabilities_t;

typedef struct ttdhcp
{
	char type[8];
	char searchdomain[16];
	char ipv4address[16];
	char ipv6address[32];
}ttdhcp_t;

typedef struct ttresolution
{
    char width[8];
	char height[8];
}ttresolution_t;

typedef struct ttbounds
{
    char x[8];        //视频显示的最大坐标
	char y[8];
	char width[8];
	char height[8];
}ttbounds_t;

typedef struct ttvideosourceconfiguration
{   
    char enable;
    int usecount;
	char configurationtoken[64];
	char name[64];
	char sourcetoken[64];
	char framerate[16]; //vi 输入的帧率
	ttresolution_t resolution; //sensor 的实际最大像素
	ttbounds_t bounds; //实际使用的像素范围
}ttvideosourceconfiguration_t;

typedef struct ttratecontrol
{
    char frameratelimit[8];
	char encodinginterval[8];   //帧间隔
	char bitratelimit[8];    //码率
}ttratecontrol_t;

typedef struct ttH264
{
    char govlength[8];
	char h264profile[16];
}ttH264_t;

typedef struct videoencoderconfiguration
{
    char enable;
    int usecount;
	char encoding[8];      //编码格式
	char sessiontimeout[8];
	char quality[16];     // 图像质量,值越少质量越好
	ttresolution_t resolution;  //分辨率
	ttratecontrol_t ratecontrol;
	ttH264_t h264;
	char name[64];
	char configurationtoken[64];
}videoencoderconfiguration_t;

typedef struct audiosourceconfiguration
{
    char enable;
	int usecount;    //用户数
	char channels[8];
	char configurationtoken[64];  //属性名
	char name[64];                 //配置文件名
    char sourcetoken[64];
}audiosourceconfiguration_t;

typedef struct audioencoderconfiguration
{ 
    char enable;
	int usecount;
	char encoding[8];               //编码格式
	char bitrate[8];
	char samplerate[8];             //采样率
	char sessiontimeout[8];         //超时时间
	char configurationtoken[64];   //属性名
	char name[64];                  //配置文件名
}audioencoderconfiguration_t;


typedef struct osdposconfiguration
{
	char type[16];
	float x;
	float y;	
}osdposconfiguration_t;
typedef struct osdcolor
{
	int x;
	int y;
	int z;
	int transparent;
}osdcolor_t;
typedef struct osdtextconfiguration
{
		char type[16];
		char dateformat[16];
		char timeformat[16];
		int fontSize;
		osdcolor_t bgcolor;
		osdcolor_t fontcolor;
		char plaintext[MAX_OSD_PLAINTEXT_LEN];
}osdtextconfiguration_t;

typedef struct osdconfiguration
{
	int  enable;
	int usecont;
	int nil;
	int index;
	char token[64];
	char sourceToken[64];
	char type[16];
	osdposconfiguration_t position;
	osdtextconfiguration_t osdtext;
}osdconfiguration_t;
typedef struct osdconfigurationMgr
{
	int osdnum;
	osdconfiguration_t osd[MAX_OSD_NUM];
}osdconfigurationMgr_t;
typedef struct ttvector1d
{
    char x[8];
}ttvector1d_t;

typedef struct ttvector2d
{
    char x[8];
	char y[8];
}ttvector2d_t;

typedef struct ttfloatrange
{
    char min[8];
	char max[8];
}ttfloatrange_t;

typedef struct ttptzspeed
{
    ttvector2d_t pantilt;//水平垂直坐标
	ttvector1d_t zoom;   //焦距位置
}ttptzspeed_t;

typedef struct ttspace1ddescription
{
    char uri[96];
	ttfloatrange_t xrange;
}ttspace1ddescription_t;

typedef struct ttspace2ddescription
{
    char uri[96];
	ttfloatrange_t xrange;
	ttfloatrange_t yrange;
}ttspace2ddescription_t;

typedef struct ttpantiltlimits
{
    ttspace2ddescription_t range;
}ttpantiltlimits_t;

typedef struct ttptzspaces
{
    ttspace1ddescription_t absolutezoompositionspace;
    ttspace2ddescription_t absolutepantiltpositionspace;//绝对位置
	ttspace1ddescription_t relativezoomtranslationspace;
    ttspace2ddescription_t relativepantilttranslationspace;//相对位置
	ttspace1ddescription_t continuouszoomvelocityspace;
    ttspace2ddescription_t continuouspantiltvelocityspace;//连续位置
	ttspace1ddescription_t pantiltspeedspace;
	ttspace1ddescription_t zoomspeedspace;
}ttptzspaces_t;

typedef struct ttptznodeconfigration
{
    char enable;
    int  usecount;              //用户数
    char panttiltposition[96];     //absolute Pan/Tilt movements
    char zoomposition[96];         //absolute zoom movements
    char pantilttranslation[96];   //relative Pan/Tilt movements
    char zoomtranslation[96];      //relative zoom movements
    char pantiltvelocity[96];      //continuous Pan/Tilt movements
    char zoomvelocity[96];         //continuous zoom movements
    char ptztimeout[8];           //PTZTimeout
    ttptzspeed_t ptzspeed;             //PTZSpeed
    ttpantiltlimits_t pantiltlimits;        //Pan/Tilt limits
    ttspace1ddescription_t zoomlimits;           //Zoom limits
    char configurationtoken[64];  //属性名
    char name[64];                 //PTZ节点名
}ttptznodeconfigration_t;

typedef struct ttptznode
{
    char homesupported[8];
    char maximumnumberofpresets[8];//预置点总数
    char configurationtoken[64];  //属性名
    char name[64];                 //PTZ节点名
    ttptzspaces_t ptzspaces;
	char auxiliarycommands[128];
	struct ttptznode *next;
}ttptznode_t;

typedef struct ttptzmovestatus
{
    char pantilt[8];
	char zoom[8];
}ttptzmovestatus_t;

/*PTZ 状态*/
typedef struct ttptzstatus
{
    char error[16];   //错误状态描述
    ttptzspeed_t position;   //位置描述
	ttptzmovestatus_t movestatus;  //移动状态描述
}ttptzstatus_t;

/*设置预置点*/
typedef struct ttpreset
{
    char profiletoken[16];
	char presetname[64];   //预置点名
	char presettoken[16];
	struct ttpreset *next;
}ttpreset_t;


/*设置初始位置*/
typedef struct tthomeposition
{
    char profiletoken[16];  //标识名
	char x[8];        //水平坐标
	char y[8];        //垂直坐标
	char z[8];        //焦距远近
}tthomeposition_t;

typedef struct ttmediaprofile
{
    char profiletoken[64];   //属性结点名
	char profilename[64];    //文件名
	ttvideosourceconfiguration_t videosourceconfiguration; //视频配置参数
	videoencoderconfiguration_t videoencoderconfiguration; //视频解编码参数
	audiosourceconfiguration_t audioSourceconfiguration;   //音频配置参数
	audioencoderconfiguration_t audioencoderconfiguration; //音频编解码参数
	ttptznodeconfigration_t ptznodeconfigration;       //PTZ配置参数
}ttmediaprofile_t;

typedef struct ttmediaprofilemax
{
    ttmediaprofile_t mediaprofile[MEDIAPROFILE_MAXNUM];
}ttmediaprofilemax_t;

typedef struct ttcreatmediaprofile
{
	int used_flag;			//is any one used  this media_profile
    ttmediaprofile_t mediaprofile;
	struct ttcreatmediaprofile *next;
}ttcreatmediaprofile_t;


typedef struct streamuri
{
    char stream[16];
	char protocol[16];
	char profiletoken[32];
}streamuri_t;

typedef struct datetime
{
    char hour[8];
    char min[8];
    char sec[8];
    char day[8];
    char month[8];
    char year[8]; 
}datetime_t;

typedef struct ttdatetimemsg
{
    char datetimetype[16];
	char datetimetz[32];
	char daylightsavings[8];
}ttdatetimemsg_t;

typedef struct ttadminsettings
{
    char autonegotiation[8];
	char speed[8];
	char duplex[8];
}ttadminsettings_t;

typedef struct ttlink
{
    char interfacetype[8];
	ttadminsettings_t adminsettings;
	ttadminsettings_t opersettings;
}ttlink_t;

typedef struct ttinfo
{
    char name[8];
	char mtu[8];
	char hwaddress[32];
}ttinfo_t;

typedef struct ttmanual
{
    char prefixlength[8];
	char address[16];
}ttmanual_t;

typedef struct ttconfig
{
    char dhcp[8];
	ttmanual_t manual;
}ttconfig_t;
typedef struct ttzeroconfiguration
{
    char interfacetoken[8];
	char enabled[8];
	char addresses[16];
	char prefixLen[8];
}ttzeroconfiguration_t;

typedef struct ttip
{
    char enabled[8];
	ttconfig_t config;
	ttzeroconfiguration_t zeroconfiguration;
}ttip_t;

typedef struct ttnetworkinterfaces
{
    char networkinterfacestoken[8];
	char enabled[8];
	char gateway[16];
	ttinfo_t info;
	ttlink_t link;
	ttip_t ipv4;
	ttip_t ipv6;
	ttdhcp_t dns;
} ttnetworkinterfaces_t;


typedef struct ttscope
{
    char scopedef[16];
	char scopeitem[64];
	struct ttscope *next;
}ttscope_t;

typedef struct ttdeviceinformation
{
    char manufacturer[8];
	char hardwareid[8];
	char mode[16];
	char firmwareversion[16];
	char serialnumbe[64];
}ttdeviceinformation_t;

typedef struct ttusers
{
    char username[128];  //用户名小于128字节
	char password[128];  //用户密码小于128字节
	char userlever[16];  //用户等级小于16字节
	struct ttusers *next;
}ttusers_t;

typedef struct ttprotocols
{
    char name[8];
	char enabled[8];
	char port[8];
}ttprotocols_t;

typedef struct ttnetworkprotocols
{
    ttprotocols_t networkprotocols[NETWORKPROTOCOLSNUM];
}ttnetworkprotocols_t;

/*用来保存客户端的guid*/
typedef struct ttrtsphttpguid
{
    int recvfd;  //接收通道的fd
	int sendfd;  //发送通道的fd
	int guid;    //对应guid
	struct ttrtsphttpguid *next;
}ttrtsphttpguid_t;

typedef struct ttrecordingsource
{
    char sourceId[64];   //记录客服端来源
    char name[64];     //用户信息名
    char location[64];  //录像的物理位置
    char description[64]; //录像信息描述
    char address[64];   //录像媒体的URI
}ttrecordingsource_t;

typedef struct ttrecordingconfiguration
{
    ttrecordingsource_t recordingsource;  //录像资源信息
    char content[64];   //录像资源描述
    char maximumretentiontime[16];  //录像的最长时间
}ttrecordingconfiguration_t;

typedef struct tttrackconfiguration
{
    char tracktype[16];   //通道类型
	char description[64];  //通道信息描述
}tttrackconfiguration_t;

typedef struct tttracks
{
    char tracktoken[64];  //通道名
    tttrackconfiguration_t trackconfiguration;  //通道配置信息
}tttracks_t;

typedef struct ttsource
{
    char sourcetoken[64];  //源信息属性
    char token[64];    //源信息名
    char autocreatereceiver[8];  //录像信息标识
}ttsource_t;

typedef struct ttrecording
{
    char recordingtoken[64];  //存储视频名
    ttrecordingconfiguration_t recordingconfiguration; //录像信息
	tttracks_t tracks;  //通道信息
    struct ttrecording *next;
}ttrecording_t;

typedef struct ttjobtracks
{
    char sourcetag[64];   //源目的
    char destination[64]; //数据存储描述
}ttjobtracks_t;

typedef struct ttrecordingjob
{
    char jobtoken[64]; 
    char recordingtoken[64];  //存储视频名
    char mode[64];       //工作模式
    char priority[8];   //优先级
    ttsource_t source;   //源信息
	ttjobtracks_t jobtracks;  //通道信息
    struct ttrecordingjob *next;
}ttrecordingjob_t;

typedef struct ttreplayrecording
{
    char sessiontimeout[32];  //录像回放时间
}ttreplayrecording_t;

typedef struct ttrenew_msg
{
    int time;
	int port;
	char ip[32];
}ttrenew_msg_t;

typedef struct ttvideo_attr
{
    char Brightness[8];
	char ColorSaturation[8];
	char Sharpness[8];
	char Contrast[8];
}ttvideo_attr_t;

typedef struct ttrelayout_attr
{
    char RelayOutputToken[32];
	char Mode[32];
	char DelayTime[32];
	char IdleState[32];
}ttrelayout_attr_t;

typedef struct ttpullmessge
{
    int fd;
	int time;
}ttpullmessge_t;

typedef struct ttmdalarm
{
    int alarmoffdelay;
	int alarmondelay;
	int alarmcount;
}ttmdalarm_t;

typedef struct ONVIF_VIDEOSIZE
{
    unsigned short int width;
    unsigned short int height;
}ONVIF_VIDEOSIZE_t;

typedef struct ttnptinfo
{
	char type[8];
	char searchdDNSdomain[64];
	char ipv4address[16];
	char ipv6address[32];
}ttntpinfo_t;

#define MAX_RESOLUTION_NUM		16
typedef struct _ResolutionInfo
{
	int res_num;
	int width[MAX_RESOLUTION_NUM];
	int height[MAX_RESOLUTION_NUM];
}ResolutionInfo_t;

enum _NVT_PTZ_CONTROL
{
	NVT_PTZ_MOVE_UP_START		= 0,
	NVT_PTZ_MOVE_DOWN_START,
	NVT_PTZ_MOVE_LEFT_START,
	NVT_PTZ_MOVE_RIGHT_START,
	NVT_PTZ_MOVE_LEFT_UP_START, //device maybe not support
	NVT_PTZ_MOVE_RIGHT_UP_START, //device maybe not support
	NVT_PTZ_MOVE_LEFT_DOWN_START, //device maybe not support
	NVT_PTZ_MOVE_RIGHT_DOWN_START, //device maybe not support
	NVT_PTZ_MOVE_STOP,

	NVT_PTZ_ZOOM_ADD_START,
	NVT_PTZ_ZOOM_SUB_START,
	NVT_PTZ_ZOOM_STOP,

	NVT_PTZ_PRESET_CALL,
	NVT_PTZ_PRESET_SET, //if preset_name exist,do nothing; if not,add preset_name
	NVT_PTZ_PRESET_DEL,
	NVT_PTZ_GOTO_HOME,
	NVT_PTZ_SET_HOMEPOSITION
};

typedef struct __NvtPtzConf_t{
	int chann; 
	int cmd; 
	int param1; 
	int param2; 
} NvtPtzConf_t;


//获取设备的 uuid
int app_get_device_uuid(char *uuid,int len);
//获取设备的网络信息
int app_get_network_config(ttnetworkinterfaces_t *pNif);
int app_set_network_config(ttnetworkinterfaces_t *pNif);
int app_get_deviceinfo_config(ttdeviceinformation_t *pDeviceinfomation);
int app_get_hostip(char *ipaddr,int len);
int app_set_hostip(char *ipaddr,int len);
int app_get_hostname(char *hostname,int len);
int app_set_hostname(const char *hostname);
int app_get_avencoder_config(ttmediaprofilemax_t *mediaprofiles);
int app_get_multicast_config(ttmulticast_t *multicast);
int app_set_multicast_config(ttmulticast_t *multicast);
int app_get_videosource_config(ttvideosourceconfiguration_t *pVideosourceconfiguration);
int app_set_videosource_config(ttvideosourceconfiguration_t *pVideosourceconfiguration);
int app_get_videoencoder_config(videoencoderconfiguration_t *pVideoencoderconfiguration);
int app_set_videoencoder_config(videoencoderconfiguration_t *pVideoencoderconfiguration);
int app_get_audiosource_config(audiosourceconfiguration_t *pAudioSourceconfiguration);
int app_set_audiosource_config(audiosourceconfiguration_t *pAudioSourceconfiguration);
int app_set_audioencoder_config(audioencoderconfiguration_t *pAudioencoderconfiguration);
int app_get_audioencoder_config(audioencoderconfiguration_t *pAudioencoderconfiguration);
int app_get_streamUri(char *streamuri,int len,int streamFlag/*0:main stream,1:substream*/,int protocol/*0:udp ,1:tcp*/);
int app_get_ntpinfo_config(ttntpinfo_t *pNTPInfo);
int app_set_ntpinfo_config(ttntpinfo_t *pNTPInfo);
int app_get_dateTime(datetime_t *datetime);
int app_set_dateTime(datetime_t * datetime,ttdatetimemsg_t * msg);
int app_get_dateTime_ext(ttdatetimemsg_t *msg);
int app_set_dateTime_ext(ttdatetimemsg_t *msg);
int app_set_sysFactory_default(int flag/*1:hard  0:soft*/);
int app_get_Resolutions(int ch,int subCh,ResolutionInfo_t *pResolutionInfo);
int app_get_discoverMode(char *mode,int len);
int app_set_discoverMode(const char *mode);
int app_get_capture_data(int ch,char *pbuff,int buff_size,int *ret_size);
int app_set_normalTime(int year,int mon,int day,int hour,int min,int sec);
int app_set_serverInfo(int cmdType,const char *ipaddr,int port,int keepalive,const char *prefix,int prefix_flg);
int app_get_osdconfigurationMgr(osdconfigurationMgr_t *pOSDConfigurationMgr); //一次性获取所有的osd配置
int app_set_osdconfiguration(osdconfiguration_t *pOSDConfiguration);//一次性设置一个osd配置
char *app_get_capture_buff();
int app_get_capture_buff_size();
int app_start_multicast_stream(int ch,int ch_type);
int app_stop_multicast_stream(int ch,int ch_type);
int app_ptzCtrl(NvtPtzConf_t *pConf);
int app_get_ImageSetting(ttvideo_attr_t *pAttr);
int app_set_ImageSetting(ttvideo_attr_t *pAttr);
int app_set_DNSInfo(const char *pInterface,const char *pAddr);
int app_set_Gateway(const char *pInterface,const char *pAddr);
int app_update_network();
#endif
