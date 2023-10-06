#ifndef _TASKMGR_H_12345678910
#define _TASKMGR_H_12345678910

#include "os_ini.h"
#include "mempool.h"
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TASK_NUM    64
#define TASK_RUNNING    0x01
#define TASK_EXITING    0x02

#define DEFAULT_TIMER_CALC		100 //系统使用的时间的最小精度为10ms

typedef enum _SELECT_TIMER_TICK_E
{
	SELECT_TIMER_TICK_DEFAULT = DEFAULT_TIMER_CALC,
	SELECT_TIMER_TICK_20MS = 20,
	SELECT_TIMER_TICK_40MS = 40,
	SELECT_TIMER_TICK_50MS = 50,
	SELECT_TIMER_TICK_200MS = 200,
	SELECT_TIMER_TICK_500MS = 500,
	SELECT_TIMER_TICK_1SEC = 1000,
	SELECT_TIMER_TICK_2SEC = 2000,
	SELECT_TIMER_TICK_5SEC = 5000,
	SELECT_TIMER_TICK_10SEC = 10000,
}TIMER_TICK_E;


	struct THR_TASK;

	typedef int (*INIT_FUNC_t)(struct THR_TASK *);
#ifdef _WIN32	
	typedef int (*NOTIFY_FUNC_t)(OS_SOCKET_t  ,const void *,int);
#else
		typedef int (*NOTIFY_FUNC_t)(OS_SOCKET_t  ,const void *,size_t);
#endif
	typedef void *(*THREAD_FUNC_t)(void *);
	typedef void (*SIG_FUNC_t)(int);
	struct EVENT_HANDLER_MGR;
	struct MQ_MGR;
#define TASK_NONBLOCK       0x01
struct THR_TASK_MGR;
struct THR_TASK
{
	struct THR_TASK *next;
	struct THR_TASK_MGR *task_mgr;			//用于标示自己是哪一个task_mgr 组
	OS_INT32_t   module;
	OS_SOCKET_t  read_fd;
	OS_SOCKET_t  write_fd;
	OS_INT32_t   status;             //可以用来记录线程的状态
	OS_INT32_t   attr;               //任务属性
	OS_INT32_t  base_timer_calc;	  //使用的定时器的精度。单位为ms
#if defined (USING_EPOLL)
	int epoll_fd;
#endif
	EVENT_HANDLER_LIST_t *event_map_array[MAX_EVENT_FD]; //和fd做一个映射关系
	INIT_FUNC_t init;
	NOTIFY_FUNC_t notify;
	void *arg;
	struct MQ_MGR msg_mgr;
	struct EVENT_HANDLER_MGR event_mgr;
};

typedef struct THR_TASK THR_TASK_t;

struct THR_TASK_MGR
{
	OS_INT32_t max_event_num;
	OS_INT32_t max_msg_num;
	OS_INT32_t max_task_num;
	OS_UINT64_t tick;
	mempool_mgr_t event_handler_pool;
	mempool_mgr_t msg_mgr_pool;
	mempool_mgr_t task_mgr_pool;
	mempool_comm_mgr_t *comm_mgr;
	THR_TASK_t *next;
};

typedef struct THR_TASK_MGR THR_TASK_MGR_t;


//初始化线程管理器队列

THR_TASK_MGR_t *init_thr_mgr(OS_INT32_t max_task,OS_INT32_t max_event,OS_INT32_t max_msgq);
int register_task(THR_TASK_MGR_t *,OS_INT32_t module_id,THREAD_FUNC_t thr_func,INIT_FUNC_t init_fun,OS_INT32_t  attr);
int unregister_task(THR_TASK_MGR_t *,OS_INT32_t module_id);
OS_BOOL unregister_all_task(THR_TASK_MGR_t *task_mgr_p);
int send_msg(THR_TASK_MGR_t *,MSG_NODE_t *msg);
OS_UINT64_t get_tick(THR_TASK_MGR_t *);
int init_timer_task(THR_TASK_MGR_t *,INIT_FUNC_t init_func);
void set_task_base_timer_calc(struct THR_TASK *pTask,OS_INT32_t base_timer_calc);
THR_TASK_t * get_task_info(THR_TASK_MGR_t *task_mgr_p,OS_INT32_t module_id);

#define BROADCAST_MODULE_ID     0xFFFF
#define TIMER_MODULER_ID        0xFFFE

//不要使用0xF000以上的ID作为模块ID，留给内部系统使用
int install_signal_hanlder(int signo,SIG_FUNC_t sig_func);

void taskmgr_set_debug_level(OS_INT32_t level);
OS_VERSION_t * taskmgr_get_version();

#ifdef __cplusplus
}
#endif
#endif

