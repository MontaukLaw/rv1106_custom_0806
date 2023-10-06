#ifndef __4G_H___
#define __4G_H___

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



int open_4G(void);

//默认是移动的 cmnet
int set_apn_name(char *apn);


int start_stream_of_4G(void);

int close_4G(void);

int get_4G_fd(void);


#ifdef __cplusplus
};
#endif


#endif