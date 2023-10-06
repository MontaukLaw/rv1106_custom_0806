/********************************************************************
 * Copyright(C), 2012, Santachi Video Technology (Shenzhen) Co.,Ltd.
 ********************************************************************
 * File Name    : rtspCliLib.h
 * Description  : 
 * Author       : st001732 <jingwen.liu@santachi.com.cn>
 * Date         : 2012-12-28
 ********************************************************************/

#ifndef __RTSPCLILIB_H_
#define __RTSPCLILIB_H_

#define RTSPCLI_BUILD_VERSION	"V1.1.1"
#define RTSPCLI_BUILD_DATE		"2021-11-26"

typedef int (*rtspcli_getFrameCB)(char *buf, int len,
		int stream_type, int encode_type, void *pUserData, int64_t pts);

typedef int (*rtspcli_getStatusCB)(int type, void *pUserData);

int st_rtspcli_getVersion(char *version, char *buildDate);
int st_rtspcli_initLib(void);
int st_rtspcli_deinitLib(void);

unsigned long st_rtspcli_open(const char *url, int transMode,
		rtspcli_getFrameCB pfFrame_CB,
		rtspcli_getStatusCB pfStatus_CB,
		void *pUserdata, int *error,
		const char *username, const char *password);

/* only use to open santachi devices */
unsigned long st_rtspcli_open2(char *ip, int port,
		int transMode, int chanType, int chn,
		rtspcli_getFrameCB frame_cb,
		rtspcli_getStatusCB status_cb,
		void *pUserdata, int *error,
		const char *username, const char *password);

int st_rtspcli_close(unsigned long hand);

#endif
