#include "common.h"
#include "comm_type.h"
#include "param.h"
#include "comm_codec.h"
#include "onvif.h"
#include "onvif_if.h"
#include "comm_app.h"
#include "ptzLib.h"
#include "uuid.h"
#define MAX_DEVICE_CHANNEL_NUM	1
#define MAX_SUB_CHANNEL_NUM 2	

#define DATE_TIME_INDEX	4

#define ONVIF_OSD_TIME_INDEX 	0
#define ONVIF_OSD_LOGO_INDEX 	1
#define ONVIF_EXT_OSD_INDEX_1	2
#define ONVIF_EXT_OSD_INDEX_2	3
#define ONVIF_EXT_OSD_INDEX_3	4
#define ONVIF_EXT_OSD_INDEX_4	5

///////////// copy from netLib.h //////////////////////////////////////

#define PARAM_VIDEO_CONFIG 		0x000D 
#define PARAM_NETWORK_CONFIG 	0x0005

static int g_osdconfiguration_num = MAX_OSD_NUM;

static char multi_ip[24] = "224.224.0.123";
static int  multi_port = 9966;
static const char mainRtspStreamUri[128]="rtsp://%s:%d/stream/av0_0";
static const char subRtspStreamUri[128] = "rtsp://%s:%d/stream/av0_1";
static char g_device_uuid[64] = "uuid:0a6dc791-134f-4991-9af1-454778a1917b";
static char g_capture_buff[MAX_CAPTURE_LEN] = {0};

extern int send_msg_for_setting(int msg_type, int para1, int para2);
static char device_descover_mode[32] = {'D','i','s','c','o','v','e','r','a','b','l','e'};
static void app_init_uuid();

static ttvideosourceconfiguration_t g_videosourceconfiguration =
{
	.enable = 1,
	.sourcetoken = "VideoSourceToken_1",
	.name = "VideoSourceConfig000",
	.configurationtoken = "VideoSourceToken_1",
	.bounds = {"0","0","1920","1080"},
	.resolution = {"1920","1080"},
	.framerate = "30",
	.usecount = 0,
};
static videoencoderconfiguration_t g_videoencoderconfiguration_m = 
{
	.enable = 1,
	.encoding = "H264",
	.usecount = 0,
	.configurationtoken = "VideoEncoderToken_1",
	.name = "VideoEncoder_1",
	.h264  = {"32","Baseline"},
	.resolution = {"1920","1080"},
	.quality = "32",
	.ratecontrol = {"30","2","2000000"},
	.sessiontimeout = "PT10S",
};
static videoencoderconfiguration_t g_videoencoderconfiguration_s = 
{
	.enable = 1,
	.encoding = "H264",
	.usecount = 0,
	.configurationtoken = "VideoEncoderToken_2",
	.name = "VideoEncoder_2",
	.h264  = {"32","Baseline"},
	.resolution = {"640","480"},
	.quality = "32",
	.ratecontrol = {"30","2","512000"},
	.sessiontimeout = "PT10S",
};
static audiosourceconfiguration_t g_audiosourceconfiguration = 
{
	.enable = 0,
	.usecount = 0,
	.channels = "1",
	.configurationtoken = "mainAudioSourceToken",
	.name = "mainAudioSourceTokenAudioSource",
	.sourcetoken = "mainAudioSourceTokenAudioSource",
};
static audioencoderconfiguration_t g_audioencoderconfiguration = 
{
	.enable = 0,
	.usecount = 0,
	.encoding = "G711",
	.bitrate = "64",
	.samplerate = "8",
	.sessiontimeout = "PT10S",
	.configurationtoken = "mainAudioEncoderToken",
	.name = "mainAudioEncoderTokenAudioEncoder",
};
static ttnetworkinterfaces_t g_networkinterfaces = 
{
	.networkinterfacestoken = "eth0",
	.enabled = "true",
	.gateway = "192.168.2.1",
	.info = {
		.name = "eth0",
		.hwaddress = "00:1F:FD:7A:B0:05",
		.mtu = "1500",
	},
	.link = {
		.adminsettings = {
			.autonegotiation = "true",
			.speed = "100",
			.duplex = "Full",
		},
		.opersettings = {
			.autonegotiation = "true",
			.speed = "100",
			.duplex = "Full",
		},
		.interfacetype = "0",
	},
	.ipv4 = {
		.enabled = "true",
		.config = {
			.dhcp = "false",
			.manual = {
				.address = "192.168.2.88",
				.prefixlength = "24",
			},
		},
		.zeroconfiguration = {
			.interfacetoken = "eth0",
			.enabled = "false",
			.addresses = "192.254.2.100",
			.prefixLen = "16",
		},
	},
	.ipv6 = {
		.enabled = "false",
	},
	.dns = {
		.type = "IPv4",
		.searchdomain = 0,
		.ipv4address = "192.168.2.1",
	},
};


static ttnetworkinterfaces_t g_wirelessinterfaces = 
{
	.networkinterfacestoken = "wlan0",
	.enabled = "false",
	.gateway = "192.168.1.1",
	.info = {
		.name = "eth0",
		.hwaddress = "00:1F:FD:7A:B0:05",
		.mtu = "1500",
	},
	.link = {
		.adminsettings = {
			.autonegotiation = "true",
			.speed = "100",
			.duplex = "Full",
		},
		.opersettings = {
			.autonegotiation = "true",
			.speed = "100",
			.duplex = "Full",
		},
		.interfacetype = "0",
	},
	.ipv4 = {
		.enabled = "true",
		.config = {
			.dhcp = "false",
			.manual = {
				.address = "192.168.1.14",
				.prefixlength = "24",
			},
		},
		.zeroconfiguration = {
			.interfacetoken = "ra0",
			.enabled = "false",
			.addresses = "192.254.1.100",
			.prefixLen = "16",
		},
	},
	.ipv6 = {
		.enabled = "false",
	},
	.dns = {
		.type = "IPv4",
		.searchdomain = 0,
		.ipv4address = "192.168.2.1",
	},	
};


static ttnetworkinterfaces_t g_pppoeinterfaces = 
{
	.networkinterfacestoken = "ppp0",
	.enabled = "false",
	.gateway = "192.168.1.1",
	.info = {
		.name = "eth0",
		.hwaddress = "00:1F:FD:7A:B0:05",
		.mtu = "1500",
	},
	.link = {
		.adminsettings = {
			.autonegotiation = "true",
			.speed = "0",
			.duplex = "Full",
		},
		.opersettings = {
			.autonegotiation = "true",
			.speed = "0",
			.duplex = "Full",
		},
		.interfacetype = "0",
	},
	.ipv4 = {
		.enabled = "true",
		.config = {
			.dhcp = "false",
			.manual = {
				.address = "192.168.1.14",
				.prefixlength = "24",
			},
		},
		.zeroconfiguration = {
			.interfacetoken = "ppp0",
			.enabled = "false",
			.addresses = "192.254.1.100",
			.prefixLen = "16",
		},
	},
	.ipv6 = {
		.enabled = "false",
	},
};


static osdconfiguration_t g_osdconfiguration[MAX_OSD_NUM] = 
{
		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_OSD_TIME_INDEX,
			.token = "TimeTittle_000",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "DateAndTime",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "yyyy-MM-dd",
				.timeformat = "HH:mm:ss",
			},				
		},		

		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_OSD_LOGO_INDEX,
			.token = "ChannTittle_000",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "Plain",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "\0",
				.timeformat = "\0",
				.plaintext = "CHANN",
			},
		},
		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_EXT_OSD_INDEX_1,
			.token = "CustomOSD_001",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "Plain",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "\0",
				.timeformat = "\0",
				.plaintext = "CustomOSD_001",
			},		
		},
		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_EXT_OSD_INDEX_2,
			.token = "CustomOSD_002",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "Plain",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "\0",
				.timeformat = "\0",
				.plaintext = "CustomOSD_002",
			},			
		},
		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_EXT_OSD_INDEX_3,
			.token = "CustomOSD_003",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "Plain",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "\0",
				.timeformat = "\0",
				.plaintext = "CustomOSD_003",
			},		
		},
		{
			.enable = 1,
			.usecont = 2,
			.nil = 1,
			.index = ONVIF_EXT_OSD_INDEX_4,
			.token = "CustomOSD_004",
			.sourceToken = "VideoSourceToken_1",
			.type = "Text",
			.position = {
				.type = "Custom",
				.x = -1,
				.y = -1,
			},
			.osdtext = {
				.type = "Plain",
				.fontSize = 10,
				.bgcolor = {
					.x = 255,
					.y = 255,
					.z = 255,
					.transparent = 0,
				},
				.fontcolor = {
					.x = 0,
					.y = 0,
					.z = 0,
					.transparent = 0,				
				},
				.dateformat = "\0",
				.timeformat = "\0",
				.plaintext = "CustomOSD_003",
			},		
		},
};

static int onvif_send_msg_for_setting(int msg_type, int para1, int para2)
{
	int paras[2];
	int try_times = 3;
	paras[0] = para1;
	paras[1] = para2;
	
	os_dbg(" send msg: %d ", msg_type);
	
	while(sched_send_msg(msg_type, &paras, sizeof(paras)) < 1)
	{
		usleep(50 * 1000);
		if(try_times > 0)
			try_times --;
		else
			break;
	}
	if(try_times <= 0)
		os_dbg(" send msg fail!!!");

}

char *app_get_capture_buff()
{
	return g_capture_buff;
}
int app_get_capture_buff_size()
{
	return MAX_CAPTURE_LEN;
}

int app_get_capture_data(int ch,char *pbuff,int buff_size,int *ret_size)
{
	os_dbg("app_get_capture_data.......ch == %d ...buff_size == %d ...\n",ch,buff_size);
	*ret_size = buff_size;
	if(app_capture_pic(ch*2,/*70,*/ pbuff,  ret_size) < 0)
	{
		*ret_size = 0;
	}	
	return OS_SUCCESS;
}
static int app_get_prefixLen(const unsigned char *netmask)
{
	int ii;
	unsigned int val = 0;
	unsigned char netmask_val[4];
	app_convertAddr2Value(netmask,netmask_val);
	val = netmask_val[0]<<24|netmask_val[1]<<16|netmask_val[2]<<8|netmask_val[3];
	os_dbg("netmask == %s",netmask);
	os_dbg("val :%d.%d.%d.%d  ==== %08x ",netmask_val[3],netmask_val[2],netmask_val[1],netmask_val[0],val);
	for(ii =0;ii<32;ii++)
	{
		if((val & (1<<(32-ii-1))) == 0)
		{
			break;
		}
	}
	return ii;
}
static unsigned int app_set_prefixLen(int prefixLen)
{
	unsigned int netmask = 0xFFFFFFFF;
	int ii;
	for(ii = 32-prefixLen; ii>0;ii--)
	{
		netmask &= ~(1<<ii);
	}
	return netmask;
}
void app_netConvertToAddr(const uchar_t *ipbuf, char *addr)
{
	if (addr == NULL)return;
	sprintf(addr, "%d.%d.%d.%d", ipbuf[0], ipbuf[1], ipbuf[2], ipbuf[3]);
	
}
int app_convertAddr2Value(const char *pAddr,unsigned char *dst)
{
	int s = inet_pton(AF_INET,pAddr,dst);
	if(s== 1)
	{
		return OS_SUCCESS;
	}
	os_dbg("inet_pton %s faild %d",pAddr,errno);
	return OS_FAILD;
}

int app_getImageAttr(int resolution,  int *width,int *heigh)
{
		int width_height[16][2] = {
			{176,144},
			{352,288},
			{704,288},
			{704,576}, //MD1
			{320,240},			
			{640,480},
			{1280,960},
			{1280,720},
			{1920,1080}, //8
			{1024,768},
			{2048,1536},
			{2304,1296},
			{2688,1520},
			{2592,1944},
			{3840,2160},
   		 };

	if (resolution > 15)
	{
	    os_dbg("%s: unsupported resolution\n", __FUNCTION__);
	    return -1;
	}
	else
	{
		*width = width_height[resolution][0];
		*heigh = width_height[resolution][1];
		os_dbg("app_getImageAttr ===> resolution :%d %dx%d \n",resolution,*width,*heigh);
	}
    return 0;
}
static int g_resCap_P[16][2] =
{
	{176,144},
	{352,288},
	{704,288},
	{704,576}, //MD1
	{320,240},			
	{640,480},
	{1280,960},
	{1280,720},
	{1920,1080}, //8
	{1024,768},
	{2048,1536},
	{2304,1296},
	{2688,1520},
	{2592,1944},
	{3840,2160},

};

int app_getResolution(int width,int heigh)
{
	int ii;
	for(ii = 0;ii < 16;ii++)
	{
		if(g_resCap_P[ii][0] == width && g_resCap_P[ii][1] == heigh)
		{
			return ii;
		}
	}
	return -1;
}
int app_set_ImageSetting(ttvideo_attr_t * pAttr)
{
	comm_video_config_t config;
	comm_param_getVideoConfigParam(0, &config);
	config.brightness = atoi(pAttr->Brightness);
	config.saturation = atoi(pAttr->ColorSaturation);
	config.contrast = atoi(pAttr->Contrast);
	config.sharpness = atoi(pAttr->Sharpness);
	comm_param_setVideoConfigParam(0, config);
	onvif_send_msg_for_setting(PARAM_VIDEO_CONFIG,0,0);
	return OS_SUCCESS;
}

int app_get_ImageSetting(ttvideo_attr_t * pAttr)
{
	comm_video_config_t config;
	comm_param_getVideoConfigParam(0, &config);
	sprintf(pAttr->Brightness,"%d",config.brightness);
	sprintf(pAttr->ColorSaturation,"%d",config.saturation);
	sprintf(pAttr->Contrast,"%d",config.contrast);
	sprintf(pAttr->Sharpness,"%d",config.sharpness);//	用于锐度调节
	return OS_SUCCESS;
}

int app_get_device_uuid(char *uuid,int len)
{
	snprintf(uuid,len,"uuid:%s",g_device_uuid);
	return OS_SUCCESS;
}


int app_get_network_config(ttnetworkinterfaces_t *pNif)
{

	int nResultLen = 0;
	char ipaddr[COMM_ADDRSIZE];
	char netmask[COMM_ADDRSIZE];
	char gw[COMM_ADDRSIZE];
	char f_dns[COMM_ADDRSIZE] = {0};
	char s_dns[COMM_ADDRSIZE] = {0};	
	comm_network_config_t network_config;
	//获取有线网卡信息
	comm_param_getNetworkStruct(&network_config);
	comm_getDns(f_dns, s_dns, COMM_ADDRSIZE);
	if(strlen(f_dns) >0)
	{
		memcpy(g_networkinterfaces.dns.ipv4address,f_dns,COMM_ADDRSIZE);
		os_dbg("f_dns :%s ",g_networkinterfaces.dns.ipv4address);
	}
	comm_getGateWay(g_networkinterfaces.gateway,16);
	comm_getMacAddr(g_networkinterfaces.info.name,g_networkinterfaces.info.hwaddress,32);
	if(network_config.dhcp_flag == 1)
	{
		memcpy(g_networkinterfaces.ipv4.config.dhcp,"true",8);
	}
	comm_get_local_ip(g_networkinterfaces.info.name, g_networkinterfaces.ipv4.config.manual.address, COMM_ADDRSIZE);
	comm_getNetMask(g_networkinterfaces.info.name,netmask, COMM_ADDRSIZE);
	os_dbg("netmask == %s",netmask);
	sprintf(g_networkinterfaces.ipv4.config.manual.prefixlength,"%d" ,app_get_prefixLen(netmask));
	memcpy(pNif,&g_networkinterfaces,sizeof(ttnetworkinterfaces_t));
	os_dbg("...... %p ....",pNif);
	return OS_SUCCESS;
}
int app_get_wireless_config(ttnetworkinterfaces_t *pNif)
{

	return OS_SUCCESS;
}
int app_get_pppoe_config(ttnetworkinterfaces_t *pNif)
{
	return OS_SUCCESS;
}
int app_set_DNSInfo(const char *pInterface,const char *pAddr)
{
	
	return OS_SUCCESS;
}
int app_set_Gateway(const char *pInterface,const char *pAddr)
{
	return OS_SUCCESS;
}

int app_set_network_config(ttnetworkinterfaces_t *pNif)
{

	int nResultLen = 0;
	comm_network_config_t network_config;
	unsigned int netmask;
	char addr[COMM_ADDRSIZE];
	//获取有线网卡信息
	comm_param_getNetworkStruct(&network_config);
	os_dbg("gateway:%s dhcp_flag:%s ipv4address:%s netmask:%s dns:%s",
		pNif->gateway,
		pNif->ipv4.config.dhcp,
		pNif->ipv4.config.manual.address,
		pNif->ipv4.config.manual.prefixlength,
		pNif->dns.ipv4address);
	
	memcpy(&g_networkinterfaces,pNif,sizeof(ttnetworkinterfaces_t));
	if(strcmp(g_networkinterfaces.ipv4.config.dhcp,"true") == 0)
	{
		network_config.dhcp_flag = 1;
	}
	else
	{
		network_config.dhcp_flag = 0;
		comm_addrConvertToNet(network_config.first_dns_addr,g_networkinterfaces.dns.ipv4address);
		comm_addrConvertToNet(network_config.def_gateway,g_networkinterfaces.gateway);
		comm_addrConvertToNet(network_config.ip_addr,g_networkinterfaces.ipv4.config.manual.address);
		netmask = app_set_prefixLen(atoi(g_networkinterfaces.ipv4.config.manual.prefixlength));
		network_config.net_mask[0] = netmask&0xFF;
		network_config.net_mask[1] = netmask>>8&0xFF;
		network_config.net_mask[2] = netmask>>16&0xFF;
		network_config.net_mask[3] = netmask>>24&0xFF;
		comm_netConvertToAddr(network_config.net_mask,addr);
		os_dbg("netmask:%s ========%08x",addr,netmask);
	}
	comm_param_setNetworkStruct(network_config);
	return OS_SUCCESS;
}
int app_get_hostip(char *ipaddr,int len)
{
	comm_getIpAddr("eth0",ipaddr,len);
	return OS_SUCCESS;
}
int app_update_network()
{
	comm_param_startNetwork();
	return OS_SUCCESS;
}

int app_get_deviceinfo_config(ttdeviceinformation_t * pDeviceinfomation)
{

	comm_version_info_t versionInfo;
	comm_param_getVersionInfo(&versionInfo);
	strcpy(pDeviceinfomation->hardwareid, "common");
	strcpy(pDeviceinfomation->manufacturer, "public");
	strcpy(pDeviceinfomation->mode, "ipcamera");
	strcpy(pDeviceinfomation->serialnumbe, versionInfo.device_serial_no);
	strcpy(pDeviceinfomation->firmwareversion,versionInfo.version_number);
	
	return OS_SUCCESS;	
}
int app_get_hostname(char *hostname,int len)
{
	comm_device_info_t device_config;
	comm_param_getDeviceInfo(&device_config);
	if(strlen(device_config.device_name) >0)
	{
		memcpy(hostname,device_config.device_name,len);
	}
	else
	{
		memcpy(hostname,"Ipcamera",len);
	}
	return OS_SUCCESS;
}
//multi ip and port s not support, this is nly a dummy
int app_get_multicast_config(ttmulticast_t *multicast)
{
    strcpy(multicast->address.type, "IPv4");
	strcpy(multicast->address.ipv4address, multi_ip);
	snprintf(multicast->port, 8, "%d", multi_port);
	strcpy(multicast->ttl, "4");
	strcpy(multicast->autostart, "true");
	os_dbg(" multiaddr: %s:%s ",multicast->address.ipv4address,multicast->port);
	return OS_SUCCESS;
}
int app_get_videosource_config(ttvideosourceconfiguration_t *pVideosourceconfiguration)
{
	int usercount = pVideosourceconfiguration->usecount;
	memcpy(pVideosourceconfiguration,&g_videosourceconfiguration,sizeof(g_videosourceconfiguration));
	pVideosourceconfiguration->usecount = usercount;
	return OS_SUCCESS;
}
int app_get_videoencoder_config(videoencoderconfiguration_t *pVideoencoderconfiguration)
{
	int iRet = OS_SUCCESS;
	comm_video_encode_t video_encoder;
	int width;
	int heigh;
	int nResult;
	int usercount = pVideoencoderconfiguration->usecount;
	os_dbg("pVideoencoderconfiguration :%p,configurationtoken :%s",pVideoencoderconfiguration,pVideoencoderconfiguration->configurationtoken);
	if(0 == strcmp(pVideoencoderconfiguration->configurationtoken,g_videoencoderconfiguration_m.configurationtoken))
	{
		//master stream
		comm_param_getVideoEncodeParam(0, &video_encoder);
		if(app_getImageAttr(video_encoder.resolution, &width, &heigh) ==0)
		{

			sprintf(g_videoencoderconfiguration_m.resolution.width,"%d",width);
			sprintf(g_videoencoderconfiguration_m.resolution.height,"%d",heigh);
			sprintf(g_videoencoderconfiguration_m.ratecontrol.bitratelimit,"%d",video_encoder.level);
			sprintf(g_videoencoderconfiguration_m.ratecontrol.encodinginterval,"%d",video_encoder.Iframe_interval);
			sprintf(g_videoencoderconfiguration_m.ratecontrol.frameratelimit,"%d",video_encoder.frame_rate);
			sprintf(g_videoencoderconfiguration_m.quality,"%d",video_encoder.Qp);
			printf("name:%s encoding:%s %sx%s %s %s %s \n",
				g_videoencoderconfiguration_m.name,
				g_videoencoderconfiguration_m.encoding,
				g_videoencoderconfiguration_m.resolution.width,
				g_videoencoderconfiguration_m.resolution.height,
				g_videoencoderconfiguration_m.ratecontrol.bitratelimit,
				g_videoencoderconfiguration_m.ratecontrol.encodinginterval,
				g_videoencoderconfiguration_m.ratecontrol.frameratelimit);

			
			memcpy(pVideoencoderconfiguration,&g_videoencoderconfiguration_m,sizeof(g_videoencoderconfiguration_m));
			pVideoencoderconfiguration->usecount = usercount;
			iRet = OS_SUCCESS;
		}
	}
	else if(0 == strcmp(pVideoencoderconfiguration->configurationtoken,g_videoencoderconfiguration_s.configurationtoken))
	{
		//slave stream
		comm_param_getSlaveEncodeParam(0, &video_encoder);
		if(app_getImageAttr(video_encoder.resolution, &width, &heigh) ==0)
		{
			sprintf(g_videoencoderconfiguration_s.resolution.width,"%d",width);
			sprintf(g_videoencoderconfiguration_s.resolution.height,"%d",heigh);
			sprintf(g_videoencoderconfiguration_s.ratecontrol.bitratelimit,"%d",video_encoder.level);
			sprintf(g_videoencoderconfiguration_s.ratecontrol.encodinginterval,"%d",video_encoder.Iframe_interval);
			sprintf(g_videoencoderconfiguration_s.ratecontrol.frameratelimit,"%d",video_encoder.frame_rate);
		
			printf("name:%s encoding:%s %sx%s %s %s %s \n",
				g_videoencoderconfiguration_s.name,
				g_videoencoderconfiguration_s.encoding,
				g_videoencoderconfiguration_s.resolution.width,
				g_videoencoderconfiguration_s.resolution.height,
				g_videoencoderconfiguration_s.ratecontrol.bitratelimit,
				g_videoencoderconfiguration_s.ratecontrol.encodinginterval,
				g_videoencoderconfiguration_s.ratecontrol.frameratelimit);	
			memcpy(pVideoencoderconfiguration,&g_videoencoderconfiguration_s,sizeof(g_videoencoderconfiguration_s));
			pVideoencoderconfiguration->usecount = usercount;

			iRet = OS_SUCCESS;
		}
	}
	os_dbg("==================================");
	return iRet;
}
int app_get_audiosource_config(audiosourceconfiguration_t *pAudioSourceconfiguration)
{
	int usercount = pAudioSourceconfiguration->usecount;
	memcpy(pAudioSourceconfiguration,&g_audiosourceconfiguration,sizeof(g_audiosourceconfiguration));
	pAudioSourceconfiguration->usecount = usercount;
	return OS_SUCCESS;
}
int app_get_audioencoder_config(audioencoderconfiguration_t *pAudioencoderconfiguration)
{
	int usercount = pAudioencoderconfiguration->usecount;
	memcpy(pAudioencoderconfiguration,&g_audioencoderconfiguration,sizeof(g_audioencoderconfiguration));
	pAudioencoderconfiguration->usecount = usercount;
	return OS_SUCCESS;
}
int app_get_streamUri(char * streamuri, int len, int streamFlag,int protocol)
{

	char ipaddr[32];
	comm_getIpAddr("eth0",ipaddr,32);
	int nResultLen = 0;
	//获取有线网卡信息
	if(streamFlag == 0) //master stream
	{
		snprintf(streamuri,len,mainRtspStreamUri,ipaddr,554);
	}
	else // slave stream
	{
		snprintf(streamuri,len,subRtspStreamUri,ipaddr,554);
	}
	return OS_SUCCESS;
}
int app_get_ntpinfo_config(ttntpinfo_t * pNTPInfo)
{
	memcpy(pNTPInfo->type,"DNS",strlen("DNS")+1);
	memcpy(pNTPInfo->searchdDNSdomain,"pool.ntp.org",strlen("pool.ntp.org")+1);
	fprintf(stderr,"app_get_ntpinfo_config \n");
	return OS_SUCCESS;
}
int app_set_ntpinfo_config(ttntpinfo_t * pNTPInfo)
{
	fprintf(stderr,"app_set_ntpinfo_config \n");
	return OS_SUCCESS;
}
int app_get_tzone(const char *TZ)
{
	int ii ;
	int sign = 1;
	int tz_hour = 0;
	int tz_minute = 0;	
	int len = strlen(TZ);
	const char *pos = TZ;
	while(*pos && isalpha(*pos))pos++;
	if(*pos == '-')
	{
			sign = -1;
			pos++;
	}
	else if(*pos == '+')
	{
		sign = 1;
		pos++;
	}
	sscanf(pos,"%d:%d",&tz_hour,&tz_minute);
	os_dbg("app_get_tzone  %d:%d \n",tz_hour,tz_minute);
	return (tz_hour*3600 +tz_minute*60)*sign;
}
int app_get_dateTime(datetime_t * datetime)
{

    time_t   now;         //实例化time_t结构
    struct tm  timenow_r;
    struct tm  *timenow;         //实例化tm结构指针

    /*time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now*/
    time(&now); 
	/*localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)*/
    timenow   =   localtime_r(&now,&timenow_r);

	/*将时间转换成字符串*/
    snprintf(datetime->hour, 8, "%02d", timenow->tm_hour);
	snprintf(datetime->min, 8, "%02d", timenow->tm_min);
	snprintf(datetime->sec, 8, "%02d", timenow->tm_sec);
	snprintf(datetime->day, 8, "%02d", timenow->tm_mday);
	snprintf(datetime->month, 8, "%02d", (timenow->tm_mon + 1));
	snprintf(datetime->year, 8, "%04d", (timenow->tm_year + 1900));
	
	return OS_SUCCESS;
}
int app_get_dateTime_ext(ttdatetimemsg_t * msg)
{
	strcpy(msg->datetimetype, "NTP");
	strcpy(msg->datetimetz, "CST-8");
	strcpy(msg->daylightsavings, "true");
	
	return OS_SUCCESS;
}

int app_set_normalTime(int year,int mon,int day,int hour,int min,int sec)
{

	comm_time_info_t time_info;
	time_info.hour = hour;
	time_info.minute = min;
	time_info.second = sec;
	time_info.year = year;
	time_info.month = mon;
	time_info.day = day;
	fprintf(stderr,"app_set_dateTime %d-%d-%d %d:%d:%d\n",
		time_info.year,time_info.month,time_info.day,time_info.hour,time_info.minute,time_info.second);
	comm_param_SetSysTime(&time_info);	

	return OS_SUCCESS;
}
int app_set_serverInfo(int cmdType,const char *ipaddr,int port,int keepalive,const char *prefix,int prefix_flg)
{
#if 0
	http_platform_t hp;
	st_param_getHttpConfig(&hp);
	memcpy(hp.platformIP,ipaddr,32);
	hp.http_enable = 1;
	hp.platformPort = port;
	hp.keepAliveTIme = keepalive;
	hp.prefix_flg = prefix_flg;
	if(prefix_flg == 1)
	{
		memcpy(hp.prefix,prefix,64);
	}
	fprintf(stderr,"app_set_serverInfo flag: %d port: %d keepalive:%d prefix_flg %d prefix:%s \n",hp.http_enable,hp.platformPort,hp.keepAliveTIme,prefix_flg,prefix);
	st_param_setHttpConfig(hp);
#endif	
	return OS_SUCCESS;
}

/////only used for manual
int app_set_dateTime(datetime_t * datetime,ttdatetimemsg_t * msg)
{

	comm_time_info_t time_info;
	time_info.hour = atoi(datetime->hour);
	time_info.minute = atoi(datetime->min);
	time_info.second = atoi(datetime->sec);
	time_info.year = atoi(datetime->year);
	time_info.month = atoi(datetime->month);
	time_info.day = atoi(datetime->day);
	fprintf(stderr,"====> %s %s %s <<<<<<\n",msg->datetimetype,msg->datetimetz,msg->daylightsavings);
	fprintf(stderr,"app_set_dateTime %d-%d-%d %d:%d:%d\n",
		time_info.year,time_info.month,time_info.day,time_info.hour,time_info.minute,time_info.second);
	comm_param_SetSysTime(&time_info);	
	return OS_SUCCESS;
}
///// only used for ntp
int app_set_dateTime_ext(ttdatetimemsg_t *msg)
{
	fprintf(stderr,"====> %s %s %s <<<<<<\n",msg->datetimetype,msg->datetimetz,msg->daylightsavings);
	return OS_SUCCESS;
}
int app_set_sysFactory_default(int flag)
{

	if(flag == 0)
	{
		comm_param_resetDefaultParam();
	}
	else if(flag == 1)
	{
		comm_param_resetFactoryParam();
	}
	
	return OS_SUCCESS;
}

int app_check_Resolution(int ch,int subCh,int width,int heigh)
{
	ResolutionInfo_t ResolutionInfo;
	int iRet = OS_FAILD;
	int ii;
	if(app_get_Resolutions(ch,subCh,&ResolutionInfo) == OS_SUCCESS)
	{
		for(ii =0; ii < ResolutionInfo.res_num;ii++)
		{
			if(ResolutionInfo.width[ii] == width && ResolutionInfo.height[ii] ==heigh)
			{
				os_dbg("app_check_Resolution:resolution  %dx%d is OK \n",width,heigh);
				iRet = OS_SUCCESS;
				break;
			}
		}
	}
	return iRet;
}
int app_check_Ratecontrol(int bitrate,int interval,int framerate)
{
	if( (bitrate >= 64 && bitrate <= 6000) &&
		(interval > 0 && interval <=200) &&
		(framerate > 1 && framerate <= 30))
	{
		os_dbg("app_check_Ratecontrol is Ok <<<<<<<<<<<<<<<<<\n");
		return OS_SUCCESS;
	}
	os_dbg("app_check_Ratecontrol is error <<<<<<<<<<<<<<<<<\n");
	return OS_FAILD;
}
int app_get_Resolutions(int ch,int subCh,ResolutionInfo_t *pResolutionInfo)
{

	int ResolutionCap;
	int ii;
	if(ch > MAX_DEVICE_CHANNEL_NUM || subCh >MAX_SUB_CHANNEL_NUM)
	{
		return OS_FAILD;
	}
	ResolutionCap = comm_param_getSupportResolution(ch,subCh);
	pResolutionInfo->res_num = 0;
	//IPC 的情况下

	for(ii =0; ii< 16;ii++)
	{
		if(ResolutionCap&(0x01<<ii))
		{
			pResolutionInfo->width[pResolutionInfo->res_num] = g_resCap_P[ii][0];
			pResolutionInfo->height[pResolutionInfo->res_num] = g_resCap_P[ii][1];
			pResolutionInfo->res_num++;
			fprintf(stderr,"app_get_Resolutions =====>res_num:%d %dx%d\n",pResolutionInfo->res_num,g_resCap_P[ii][0],g_resCap_P[ii][1]);

		}
	}

	return OS_SUCCESS;
}

int app_set_videoencoder_config(videoencoderconfiguration_t * pVideoencoderconfiguration)
{
#if 0
	int iRet = OS_FAILD;
	int width = atoi(pVideoencoderconfiguration->resolution.width);
	int heigh = atoi(pVideoencoderconfiguration->resolution.height);
	int bitratelimit = atoi(pVideoencoderconfiguration->ratecontrol.bitratelimit);
	int encodinginterval = atoi(pVideoencoderconfiguration->ratecontrol.encodinginterval);
	int frameratelimit = atoi(pVideoencoderconfiguration->ratecontrol.frameratelimit);
	int quality = atoi(pVideoencoderconfiguration->quality);
	fprintf(stderr,">>>> app_set_videoencoder_config: bitrate %d interval %d framerate %d quality %d <<<<< \n",
		bitratelimit,encodinginterval,frameratelimit,quality);

	video_encode_t video_encoder;
	if(0 == strcmp(pVideoencoderconfiguration->name,"VideoEncoder_1")) //master stream
	{
		if((app_check_Resolution(0,0,width,heigh)== OS_SUCCESS) &&
			(app_check_Ratecontrol(bitratelimit,encodinginterval,frameratelimit) == OS_SUCCESS) 
			/*&&(quality > 21 && quality < 40) */)
			{
				st_param_getVideoEncodeParam(0, &video_encoder);
				memcpy(&g_videoencoderconfiguration_m,pVideoencoderconfiguration,sizeof(g_videoencoderconfiguration_m));			
				video_encoder.frame_rate = frameratelimit;
				video_encoder.level = bitratelimit;
//				video_encoder.Qp = quality;
				video_encoder.Iframe_interval = encodinginterval;
				video_encoder.resolution = app_getResolution(width,heigh);
				st_param_setVideoEncodeParam(0, video_encoder);
				
				printf("mainStream name:%s encoding:%s %sx%s %s %s %s \n",
					g_videoencoderconfiguration_m.name,
					g_videoencoderconfiguration_m.encoding,
					g_videoencoderconfiguration_m.resolution.width,
					g_videoencoderconfiguration_m.resolution.height,
					g_videoencoderconfiguration_m.ratecontrol.bitratelimit,
					g_videoencoderconfiguration_m.ratecontrol.encodinginterval,
					g_videoencoderconfiguration_m.ratecontrol.frameratelimit);
				
				iRet =OS_SUCCESS;
			}
	}
	else if(0 == strcmp(pVideoencoderconfiguration->name,"VideoEncoder_2")) // sub stream
	{
		if((app_check_Resolution(0,1,width,heigh)== OS_SUCCESS) &&
			(app_check_Ratecontrol(bitratelimit,encodinginterval,frameratelimit) == OS_SUCCESS) 
			/*&&(quality > 21 && quality < 40)*/)
			{
				st_param_getSlaveEncodeParam(0, &video_encoder);
				memcpy(&g_videoencoderconfiguration_s,pVideoencoderconfiguration,sizeof(g_videoencoderconfiguration_s));
				video_encoder.frame_rate = frameratelimit;
				video_encoder.level = bitratelimit;
//				video_encoder.Qp = quality;
				video_encoder.Iframe_interval = encodinginterval;
				video_encoder.resolution = app_getResolution(width,heigh);
				st_param_setSlaveEncodeParam(0, video_encoder);
				
				printf("subStream name:%s encoding:%s %sx%s %s %s %s \n",
					g_videoencoderconfiguration_s.name,
					g_videoencoderconfiguration_s.encoding,
					g_videoencoderconfiguration_s.resolution.width,
					g_videoencoderconfiguration_s.resolution.height,
					g_videoencoderconfiguration_s.ratecontrol.bitratelimit,
					g_videoencoderconfiguration_s.ratecontrol.encodinginterval,
					g_videoencoderconfiguration_s.ratecontrol.frameratelimit);
				iRet =OS_SUCCESS;
			}	
	}
#endif
	return OS_SUCCESS;
}
int app_set_multicast_config(ttmulticast_t * pMulticast)
{
	multi_port = atoi(pMulticast->port);
	memset(multi_ip,0,sizeof(multi_ip));
	strcpy(multi_ip,pMulticast->address.ipv4address );
	os_dbg("multicast : %s :%d",multi_ip,multi_port);
	return OS_SUCCESS;
}
int app_start_multicast_stream(int ch,int ch_type)
{
	os_dbg("start ch %d_%d",ch,ch_type);
	return comm_start_multiRtp(multi_ip,multi_port,ch,ch_type);
}
int app_stop_multicast_stream(int ch,int ch_type)
{
	os_dbg("stop ch %d_%d",ch,ch_type);
	return comm_stop_multiRtp(ch, ch_type);
}

int app_set_audioencoder_config(audioencoderconfiguration_t * pAudioencoderconfiguration)
{
	if((atoi(pAudioencoderconfiguration->bitrate) != 64) ||
		(atoi(pAudioencoderconfiguration->samplerate) != 8) ||
		(strcmp(pAudioencoderconfiguration->encoding,"G711")))
	{
		return OS_FAILD;
	}
	return OS_SUCCESS;
}
int app_get_discoverMode(char * mode, int len)
{
	memcpy(mode,device_descover_mode,len);
	return OS_SUCCESS;
}
int app_set_discoverMode(const char * mode)
{
	memcpy(device_descover_mode,mode,strlen(mode));
	return OS_SUCCESS;
}


int app_convert_OSDDevice2Onvif(int dx,int dy,float *ox,float *oy)
{
#if 0
	video_encode_t video_encoder;
	int width,heigh;
	st_param_getVideoEncodeParam(0, &video_encoder);
	if(app_getImageAttr(video_encoder.resolution, &width, &heigh) ==0)
	{
	   *ox = ((float)(dx - (int)width/2))/(float)(width/2);
	   *oy = ((float)(dy - (int)heigh/2))/(float)(heigh/2);
 		fprintf(stderr, "org pos: (%d, %d) --> (%.2f, %.2f)\n", dx, dy, *ox, *oy);	
	}
#endif	
	return OS_SUCCESS;
}
int app_convert_OSDOnvif2Device(int *dx,int *dy,float ox,float oy)
{
#if 0
	video_encode_t video_encoder;
	int width,heigh;
	st_param_getVideoEncodeParam(0, &video_encoder);
	if(app_getImageAttr(video_encoder.resolution, &width, &heigh) ==0)
	{
	    *dx = (1.0 + ox) * ((float)width) / 2.0;
	    *dy = (1.0 - oy) * ((float)heigh) / 2.0;	
 		fprintf(stderr, "org pos: (%.2f, %.2f) --> (%d, %d)\n", ox, oy, dx, dy);	
		
	}
#endif	
	return OS_SUCCESS;
}

int app_get_osdconfigurationMgr(osdconfigurationMgr_t * pOSDConfigurationMgr)
{
	pOSDConfigurationMgr->osdnum = g_osdconfiguration_num;
	memcpy(pOSDConfigurationMgr->osd,g_osdconfiguration,sizeof(g_osdconfiguration));
	return OS_SUCCESS;
}

int app_set_osdconfiguration(osdconfiguration_t * pOSDConfiguration)
{
#if 0
	int index = pOSDConfiguration->index;
	if(index < g_osdconfiguration_num)
	{
		int px,py;
		memcpy(&g_osdconfiguration[index],pOSDConfiguration,sizeof(osdconfiguration_t));
		app_convert_OSDOnvif2Device(&px,&py,pOSDConfiguration->position.x,pOSDConfiguration->position.y);
		app_updateOSDEx(px,py,index,pOSDConfiguration->osdtext.plaintext);
		
	}
#endif	
	return OS_SUCCESS;
}

void app_init_uuid()
{
	char szMac[20] = {0};
	os_uuid_t nsid;
	os_uuid_t uid;
	comm_getMacAddr("eth0",szMac,20);
	nsid.time_mid = 0xabcd;
	nsid.time_low = 0xabcdef;
	nsid.time_hi_and_version = 0x1234;
	nsid.clock_seq_hi_and_reserved = 0x5678;
	nsid.time_low = 0x3456;
	memcpy(nsid.node,"ipcam",6);
	os_uuid_create_from_name(&uid,nsid, szMac, strlen(szMac)+1);
	os_uuid_unpack(&uid,g_device_uuid);
	
	
}
int app_init_onvif()
{
	app_init_uuid();	//获取 uuid
	onvif_init(NULL);
	return OS_SUCCESS;
}
int app_destroy_onvif()
{
	onvif_destroy(NULL);
	return OS_SUCCESS;
}

// 云台控制
int app_ptzCtrl(NvtPtzConf_t *ptzConf)
{
	ptz_cmd_param_t ptz_cmd;
	os_dbg("cmd == %d param1 == %d param2 == %d",ptzConf->cmd,ptzConf->param1,ptzConf->param2);
	memset(&ptz_cmd,0,sizeof(ptz_cmd_param_t));
	switch(ptzConf->cmd)
	{
		case NVT_PTZ_MOVE_UP_START:
			ptz_cmd.cmd = PTZ_UPSTART;
			ptz_cmd.value = ptzConf->param2;
			break;
		case NVT_PTZ_MOVE_DOWN_START:
			ptz_cmd.cmd = PTZ_DOWNSTART;
			ptz_cmd.value = ptzConf->param2;
			break;
		case NVT_PTZ_MOVE_LEFT_START:
			ptz_cmd.cmd = PTZ_LEFTSTART;
			ptz_cmd.value = ptzConf->param1;
			break;
		case NVT_PTZ_MOVE_RIGHT_START:
			ptz_cmd.cmd = PTZ_RIGHTSTART;
			ptz_cmd.value = ptzConf->param1;
			break;
		case NVT_PTZ_MOVE_LEFT_UP_START:
			ptz_cmd.cmd = PTZ_LEFTUPSTART;
			ptz_cmd.value = (ptzConf->param1 + ptzConf->param2)/2;
			break;
		case NVT_PTZ_MOVE_RIGHT_UP_START:
			ptz_cmd.cmd = PTZ_RIGHTUPSTART;
			ptz_cmd.value = (ptzConf->param1 + ptzConf->param2)/2;
			break;
		case NVT_PTZ_MOVE_LEFT_DOWN_START:
			ptz_cmd.cmd = PTZ_LEFTDOWNSTART;
			ptz_cmd.value = (ptzConf->param1 + ptzConf->param2)/2;
			break;
		case NVT_PTZ_MOVE_STOP:
			ptz_cmd.cmd = PTZ_UPSTOP;
			break;
		case NVT_PTZ_ZOOM_ADD_START:
			ptz_cmd.cmd= PTZ_ZOOMADDSTART;
			break;
		case NVT_PTZ_ZOOM_SUB_START:
			ptz_cmd.cmd= PTZ_ZOOMSUBSTART;
			break;
		case NVT_PTZ_PRESET_CALL:
			ptz_cmd.cmd = PTZ_PREVPOINTCALL;
			ptz_cmd.value = ptzConf->param1;
			break;
		case NVT_PTZ_PRESET_SET:
			ptz_cmd.cmd = PTZ_PREVPOINTSET;
			ptz_cmd.value = ptzConf->param1;
			break;
		case NVT_PTZ_PRESET_DEL:
			ptz_cmd.cmd = PTZ_PREVPOINTDEL;
			ptz_cmd.value = ptzConf->param1;			
			break;
		case NVT_PTZ_GOTO_HOME:
			break;
		case NVT_PTZ_SET_HOMEPOSITION:
			break;
		default:
			break;
	}
	os_dbg("................");
	// 发送给云台控制线程
	comm_ptz_sendCommand(0,&ptz_cmd);	
}
