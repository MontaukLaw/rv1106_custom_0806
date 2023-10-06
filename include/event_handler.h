#ifndef __EVENT_HANDLER_H_12345678910
#define __EVENT_HANDLER_H_12345678910

#include "os_ini.h"
#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_TIMER_TICK  10

struct THR_TASK;
struct EVENT_HANDLER_LIST;
struct MQ_LIST;

typedef int (*H_READ)(OS_SOCKET_t fd, struct THR_TASK *,struct EVENT_HANDLER_LIST *);
typedef int (*H_CONNECTED)(OS_SOCKET_t fd, struct THR_TASK *,struct EVENT_HANDLER_LIST *);
typedef int (*H_WRITE)(OS_SOCKET_t fd, struct THR_TASK *,struct EVENT_HANDLER_LIST *);
typedef int (*H_EXEC)(OS_SOCKET_t fd, struct THR_TASK *,struct EVENT_HANDLER_LIST *);
typedef int (*H_TIMEOUT)(struct THR_TASK *,struct EVENT_HANDLER_LIST *);
typedef int (*H_ACCEPT)(OS_SOCKET_t,struct THR_TASK *);
typedef int (*H_MSG_HANDLER)(struct THR_TASK *,struct MQ_LIST *);
typedef int (*H_DESTROY)(void *);

#define EVENT_READ 0x01    //可读，
#define EVENT_WRITE 0x02   //可写
#define EVENT_RD_WR (0x01 | 0x02)   //可读，可写
#define EVENT_CONNECT   0x10
#define EVENT_ACCEPT    0x04        //用于accept
#define EVENT_TIMEOUT 0x08   //超时
#define HANDLE_MSG      0x0080  //消息处理另处理
#define CHECK_REGISTER  0x8000  //是否检查重复注册时间标志
#define EVENT_INTERVAL  0x4000  //是否是长期可用
#define EVENT_DESTROY   0x2000  //该事件是否需要被销毁

#define DEFAULT_MAX_EVENT  1024
#define MAX_EVENT_FD		 65536		//进程最大的描述符值
#define MAX_EVENT	1024

struct EVENT_HANDLER
{
	OS_SOCKET_t fd; //监听的文件描述符
	OS_UINT32_t event_id; //唯一标示一个事件ID
	int flag; // 1:可读，2:写 3:可读,可写
	OS_UINT32_t timeout_val; //超时时间,毫秒
	OS_UINT64_t base;
	H_READ read;//
	H_WRITE write;
	H_ACCEPT accept;
	H_TIMEOUT timeout;
	H_MSG_HANDLER msg_handler;
	H_CONNECTED connected;
	H_EXEC exec;//异常处理
	H_DESTROY destroy;      //用于在销毁event_handler时调用
	struct sockaddr_in addr;
	int addr_len;
	struct THR_TASK *task;		//标示下自己输入哪个task
	void *pmsg;
};
typedef struct EVENT_HANDLER EVENT_HANDLER_t;

struct EVENT_HANDLER_LIST
{
	EVENT_HANDLER_t event;
	struct EVENT_HANDLER_LIST *next;
	struct EVENT_HANDLER_LIST *prev;
};
typedef struct EVENT_HANDLER_LIST EVENT_HANDLER_LIST_t;


struct EVENT_HANDLER_MGR
{
	EVENT_HANDLER_LIST_t head;       //其他事件挂在同一个消息队列上
	EVENT_HANDLER_LIST_t *msg_handler;    //消息独立出来优先处理
	struct THR_TASK_MGR *task_mgr; 
	OS_SOCKET_t maxfd ;
	OS_UINT32_t timeout_val;
#if	defined (USING_EPOLL)
	int epoll_fd;
	struct epoll_event *event;
#elif defined (USING_COMPLETEPORT) 

#elif defined (USING_SELECT)
	fd_set rdset ;
	fd_set wrset;
	fd_set errset;
#endif
};

typedef struct EVENT_HANDLER_MGR EVENT_HANDLER_MGR_t;

#define EVENT_HANDLER_LIST_INIT(event_hanlder_list,fd_,flag_)       \
	do   {															\
		event_hanlder_list.next = NULL;                               \
		event_hanlder_list.prev = NULL;                               \
		event_hanlder_list.event.fd = (fd_);                            \
		event_hanlder_list.event.event_id = 0;                          \
		event_hanlder_list.event.flag = (flag_);                         \
		event_hanlder_list.event.timeout_val = 0;                       \
		event_hanlder_list.event.base = 0;                              \
		event_hanlder_list.event.read = NULL;                           \
		event_hanlder_list.event.write = NULL;                          \
		event_hanlder_list.event.accept = NULL;                         \
		event_hanlder_list.event.timeout = NULL;                        \
		event_hanlder_list.event.msg_handler = NULL;                    \
		event_hanlder_list.event.connected = NULL;                      \
		event_hanlder_list.event.exec = NULL;                           \
		event_hanlder_list.event.destroy = NULL;                        \
		event_hanlder_list.event.pmsg = NULL;                           \
	}while(0)


void init_event_handler_mgr(OS_UINT32_t max_event);
void init_event_handle(struct THR_TASK_MGR *,EVENT_HANDLER_MGR_t  *event_mgr);

int register_event_handle(struct THR_TASK *ptask,EVENT_HANDLER_LIST_t *handler);
int unregister_event_list(struct THR_TASK *ptask,EVENT_HANDLER_LIST_t *handler_list);
EVENT_HANDLER_LIST_t *unregister_event_handle(struct THR_TASK *ptask,OS_UINT32_t event_id);

int register_msg_handler(struct THR_TASK *ptask,H_MSG_HANDLER msg_handdler_fun);
int modify_event_handle(struct THR_TASK *ptask,OS_UINT32_t event_id,int flag);

EVENT_HANDLER_LIST_t *get_event_handler(struct THR_TASK *ptask,OS_UINT32_t event_id);
EVENT_HANDLER_LIST_t *get_event_handler_by_fd(struct THR_TASK *ptask,OS_SOCKET_t fd);
OS_BOOL remove_event_handler(struct THR_TASK *ptask,OS_SOCKET_t fd,int flag);

OS_BOOL remove_event_handler_list(struct THR_TASK *ptask,EVENT_HANDLER_LIST_t *handler_list,int flag);

OS_BOOL add_event_handler_list(struct THR_TASK *ptask,EVENT_HANDLER_LIST_t *handler_list,int flag);
OS_BOOL add_event_handle(struct THR_TASK *ptask,OS_SOCKET_t fd,int flag);
OS_BOOL destroy_event_handle(struct THR_TASK *ptask,EVENT_HANDLER_LIST_t *handler_list);

void monitor_event_handle();

void event_handler_set_debug_level(OS_INT32_t level);
OS_VERSION_t * event_handler_get_version();

#ifdef __cplusplus
}
#endif

#endif
