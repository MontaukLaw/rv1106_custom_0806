/*
 * Copyright :(C) 2006 santachi corp.
 * Filename : encNetLib.h
 * Discription : head file of encNetLib.h
 * Created : liuqs <liuqs_santachi@126.com> 2006/11/21
 * Modified :  
 */

#ifndef __ENCNET_LIB_H__
#define __ENCNET_LIB_H__

int st_net_initEncNetLib(void);
int st_net_deinitEncNetLib(void);
int st_net_startEncNetService(void);
int st_net_stopEncNetService(void);

int st_net_startNetServer(void);
int st_net_stopNetServer(void);

int st_net_refreshDdns(pppoe_info_t tmp);
int st_net_getMulticastAddr(char *ipaddr, char *maddr);
int st_net_sendAlarmInfoToClient(void *palarm_info);

int st_net_refreshNetwork(void);
int st_net_restartNetServer(void);

int st_net_refreshGateWay(void);
int st_net_sendArp(char *eth_name,char *ip,char *mac);
int st_net_get_defaultgw_flag();


/*pppLib.c*/
typedef struct __pppoe_ctrl_t
{
	int control;
	char account[64];
	char password[24];
	char interface[12];
	int type;
	char name[32];
	char network[16];
	char apn[64];
	char tel_num[64];
	char user_id[64];
	char pass[64];
}pppoe_ctrl_t;

int st_net_pppinitEncNetLib(void);
int st_net_pppdeinitEncNetLib(void);
int st_net_pppstartEncNetServer(void);
int st_net_pppstopEncNetServer(void);
int st_net_ppprestartEncNetLib(void);
int st_net_pppGetCdmaFlag(void);


#endif /* __ENC_NET_LIB_H__ */

