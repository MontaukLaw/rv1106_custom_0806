#ifndef __ONVIF_PARAM_H
#define __ONVIF_PARAM_H

typedef struct tagOnvif_DeviceInfo
{
    char manufacturer[8];
	char hardwareid[8];
	char mode[16];
	char firmwareversion[16];
	char serialnumbe[64];	
}Onvif_DeviceInfo_t;

typedef struct tagOnvif_SysCfgInfo
{
	int channel_num;
	int prober_num;
	int output_num;
	int ptz_enable;
	int storage_enable;
}Onvif_SysCfgInfo_t;
typedef struct tagOnvif_VideoChannelInfo
{

}Onvif_VideoChannelInfo_t;
typedef struct tagOnvif_VideoEncoderInfo
{

}Onvif_VideoEncoderInfo_t;
typedef struct tagOnvif_AudioChannelInfo
{

}Onvif_AudioChannelInfo;
typedef struct tagOnvif_AudioEncoderInfo
{

}Onvif_AudioEncoderInfo_t;
typedef struct tagOnvif_PtzCfgInfo
{

}Onvif_PtzCfgInfo_t;
typedef struct tagOnvif_NetworkInfo
{

}Onvif_NetworkInfo_t;
#endif
