/************************************************************************
 *
 * Copyright (C), Santachi Video Technology (Shenzhen) Co.,Ltd.
 *
 * File Name   : rtspServLib.h
 *
 * Description : Provide the initialization, de-initialization
 *               and stream data callback function interfaces
 *               to the application level.
 *
 * Author      : hulz <lizhen.hu@santachi.com.cn>
 *               liujingwen <jingwen.liu@santachi.com.cn>
 *
 * Create Date : 2012-08-01
 *
 ***********************************************************************/


#ifndef __RTSPLIB_H__
#define __RTSPLIB_H__


#define DFL_RTSP_PORT       554	/* Default RTSP port. */
#define DFL_HTTP_PORT       8080	/* Default HTTP port. */

#define DFL_MCAST_MIN_ADDR	"224.100.100.100"

/**
 * @strm_rpospp:    two level pointer to stream read position.
 */
typedef int (*getStreamCB)(int local_chn, int chn_type,
		void *buf, void **rpos, void *ctx);
typedef int (*chnVerifyCB)(int chn, int chn_type, void *ctx);
typedef int (*mediaVerifyCB)(int chn, int chn_type, int media_type, void *ctx);

int st_rtsp_initLib(unsigned short port,int max_frame_size);
int st_rtsp_deinitLib(void);
int st_rtsp_startService(getStreamCB get_stream_cb,
		chnVerifyCB chn_verify_cb,
		mediaVerifyCB media_verify_cb, void *ctx);
int st_rtsp_stopService(void);
int st_rtsp_setMultiCastStartAddr(char *ipaddr);
int st_rtsp_setupRtcp(int enable, int interval_s);
int st_rtsp_getVersion(char *version, char *buildDate);

#endif
