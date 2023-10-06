#ifndef WIFI_OPERA_H
#define WIFI_OPERA_H

#include <stdbool.h>

int wifi_set_node_name(char*name);

/* 取得wifi ap信息, 耗时2s 左右； 如果得到的AP 不够全面，可以都调用几次 */
int wifi_scan_info(char *buff, int max_len);


int wifi_connect_ap(char * ssid, char *passwd);


int wifi_disconnect_ap(void);


bool wifi_remove_ap(char * ssid);



#endif