#ifndef __COMM_APP_H
#define __COMM_APP_H


#ifdef __cplusplus
extern "C" {
#endif

/* stream*/
void net_stream_lockMutex(int channel,int stream_index,int pos);
void net_stream_unlockMutex(int channel,int stream_index,int pos);
int net_stream_initStreamLib();
void net_stream_deinitStreamLib();
void net_stream_netStreamWaiting_timeout(int channel,int stream_index,int msec);
int net_stream_getNetStreamWritePos(int channel, int stream_index);
net_frame_t* net_stream_getNetStreamFromPool(int channel, int stream_index, read_pos_t *read_pos);
int net_stream_sendFrameToNetPool(int channel, int stream_index, av_frame_t *enc_frame);
void net_stream_netStreamBroadcast(int channel, int stream_index);

/*alarm*/
int net_stream_initAlarmStream();
void net_stream_destroyAlarmStream();
int net_stream_sendAlarmToNetPool(alarm_info_t *p_alarm_info);
void net_stream_alarmStreamBroadcast(void);
void net_stream_alarmStreamWaiting(int msec);
alarm_info_t *net_stream_getAlarmStreamFromPool(read_pos_t *read_pos);
int net_stream_getAlarmStreamWritePos();
void net_alarm_lockMutex(int pos);
void net_alarm_unlockMutex(int pos);





const char *comm_getEthName();
const char *comm_getWifiName();
int comm_del_gateway(char *gw,char *ifname);
int comm_route_set (char *dev, char *desination, char *gw, char *nm);
int comm_set_netmask(char* name,const char *netmask);
int comm_set_gateway (char *ifname,unsigned int gw,unsigned int mask);
int comm_set_arp(char* pmac,unsigned int gateway);
int comm_set_ip(char* name,const char  *ip);
int comm_set_ip_down(const char* name);
int comm_set_ip_up(char* name);
int comm_get_local_ip(const char *interface, char *addr,int addrlen);
void comm_addrConvertToNet(unsigned char *ipbuf, const char *addr);
void comm_netConvertToAddr(const unsigned char *ipbuf, char *addr);
int comm_getLinkStatus(const char *if_name);
int comm_setAutoDns(void);
int comm_setGateWay(const char *gateway);
int comm_delGateWay(const char *gateway);
int comm_setBroadCast(const char *name,const char *broadcast);
int comm_setNetMask(const char *name,const char *netmask);
int comm_setIpAddr(const char *name,const char *ipaddr);
int comm_setMacAddr(const char *name,const char *macaddr);
int comm_getDns(char *firstdns, char *seconddns, size_t len);
int comm_getGateWay_ex(char *gateway, size_t len);
int comm_getGateWay(char *gateway, size_t len);
int comm_getBroadCast(const char *name,char *broadcast, size_t len);
int comm_getNetAddr(const char *name,char *netaddr, size_t len);
int comm_getNetMask(const char *name,char *netmask, size_t len);
int comm_getIpAddr(const char *name,char *ipaddr, size_t len);
int comm_getPPPIpAddr(char *ipaddr, size_t len);
int comm_getMacAddr(const char *eth_name,char *macaddr, size_t len);
int comm_getIfindex(const char *name,int *ifindex);
int comm_setDns(const char *firstdns, const char *seconddns);
int comm_convertMacToHex(const char *Mac,char *MacHex);




void init_common_lib();
int app_init_onvif();
int comm_app_get_ViInfo(int camId,int *nWidth,int *nHeigh,int *fps);


#define MAX_RECORD_CACHE_SIZE	2*1024*1024	// 缓存2M的数据
#define RECORD_STATE_INIT		0x0		
#define	RECORD_STATE_RUNNING	0x01
#define	RECORD_STATE_END		0x02
#define RECORD_STATE_ERROR		0xFF	//录像错误

// 如果有sd卡，则录像在sd卡中，否则录像在/userdata/media路径中
void comm_init_record(const char *pRecordPath);
int comm_start_record(int ch,int ch_type,int switch_time);
int comm_stop_record(int ch);
int comm_get_record_state(int ch);
int comm_get_record_ctrl(int ch);

void comm_init_multiRtp();
int comm_start_multiRtp(const char *ip,int port,int ch,int ch_type);
int comm_stop_multiRtp(int ch,int ch_type);
int  comm_start_auth_service();
int comm_init_gbtPlatform();
int comm_app_get_multiples(int ch);
int comm_app_get_multiples_status();
void comm_app_disable_multiples();
int  comm_start_auth_service();
void app_init_ircut();
void app_init_storage();


#ifdef __cplusplus
};
#endif


#endif
