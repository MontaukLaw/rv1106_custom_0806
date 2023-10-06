#ifndef	__TASK_LIB_H__
#define	__TASK_LIB_H__

#ifdef __cplusplus
extern "C"{
#endif

#include	<stdio.h>
#include	<stdlib.h>
#include	<pthread.h>
#include	<assert.h>


typedef int (*task_method_t)(void*, void*);

int taskLib_init(int task_num); //task_num: 20->500
int taskLib_deinit();

int taskLib_addTask(void *param1, void *param2, task_method_t method_func);
int taskLib_getRunTaskNum();
int taskLib_getWaitTaskNum();

void taskLib_usleep(int usec);
int taskLib_getTickCount(); //second num from OS start


#ifdef __cplusplus
}
#endif

#endif  //__TASK_LIB_H__


