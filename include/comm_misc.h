#ifndef __COMM_MISC_H
#define __COMM_MISC_H

#include "comm_type.h"

#define UART0 0
#define UART1 1
#define UART2 2

typedef int (*serial_stream_cb_fun_t)(int serial_no,unsigned char *buf,int size,void *pUser);
int comm_com_register_callback(serial_stream_cb_fun_t cb,void *pUser);
int comm_com_initComLib(void);
int comm_com_destroySerial(void);
int comm_com_refreshSerialParam(int serial_no, int bautrate, int parity, int stopbit, int databit);
int comm_com_sendDataToCom(int serial_no, unsigned char *buf, int size);
int comm_com_readDataFromCom(int serial_no,unsigned char *buf,int size);
int comm_com_getSerialPortFd(int serial_no);
void comm_com_monitorSerialParam(void);
/*transparent*/
int comm_com_startRecThread(int serial_no);
int comm_com_stopRecThread(int serial_no);
/*comStream.c*/
int comm_com_initRecBuf(int serial_no);
void comm_com_destroyRecBuf(int serial_no);
int comm_com_sendDataToRecBuf(int serial_no, char data);
int comm_com_sendRecBufBroadcast(int serial_no);
void comm_com_recDataWaiting(int serial_no);
int comm_com_getDataFromRecBuf(int serial_no, read_pos_t *read_pos);
int comm_com_getRecBufWritePos(int serial_no);
int comm_app_stopFeedwatchdogService(void);
int comm_app_startFeedwatchdogService(void);
void comm_com_set_debug_flag(int flag);
ssize_t comm_local_readn(int fd, void *vptr, size_t n);
ssize_t comm_local_writen(int fd, const void *vptr, size_t n);

time_t time_HumanToUnix(int year,int month,int day,int hour,int minute,int second);
void time_UnixToHuman(time_t utc,int *pyear,int *pmonth,int *pday,int *phour,int *pminute,int *psecond);


#endif
