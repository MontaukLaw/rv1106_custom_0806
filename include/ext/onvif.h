#ifndef _ONVIF_H
#define _ONVIF_H

int onvif_init(void *pArg);
void onvif_destroy(void *pArg);
int onvif_get_http_port();
void onvif_set_http_port(int port);
void onvif_monitor_comm_mempool();
#endif
