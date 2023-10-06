#ifndef __ATLIB_H__
#define __ATLIB_H__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	ERROR_OK = 0,
	ERROR_4GMODULE_NOT_WORK = -1,
	ERROR_SIMCARD_NOT_READY = -2,
	ERROR_MUTEX_INIT_FAIL = -3,
	ERROR_OPEN_UART_FAIL = -4,
}ERROR_TYPE_4G;

//默认是移动的 cmnet
int atLib_init();
int atLib_startService();
int atLib_stopService();
int atLib_getSignalVal();
int atLib_set_apn(const char *apn,const char *user,const char *passwd);
//0 -- close 1 --- open 2 --- dial to net successfully

int atLib_getStatus();

#ifdef __cplusplus
};
#endif

#endif
