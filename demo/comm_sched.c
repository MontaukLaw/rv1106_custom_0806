#include "common.h"
#include "comm_type.h"
#include "param.h"

#include "codec2_process.h"

#include "comm_app.h"
#include "uuid.h"
#include <stdbool.h>

static THR_TASK_MGR_t *g_task_mgr = NULL;
ThreadPoolMgr_t g_ThreadPoolMgr;
#define BASE_SCHED_MODULE 0xF000

#define CODEC_MAX_TASK 10
#define CODEC_MAX_EVENT 256
#define CODEC_MAX_MQ 2000
extern OS_UINT64_t comm_app_get_last_multiples_tick();
OS_UINT64_t comm_get_task_tick()
{
	OS_UINT64_t tick = get_tick(g_task_mgr);
	return tick;
}

extern void setting_msg_handle(int msg_type, int para1, int para2);

// timeout process
static int on_timeout_msg(THR_TASK_t *ptask, EVENT_HANDLER_LIST_t *handler_list)
{
	/*
		OS_UINT64_t tick = get_tick(g_task_mgr);
		if(comm_app_get_multiples_status() == 1)
		{
			OS_UINT64_t last_tick = comm_app_get_last_multiples_tick();
			if(tick - last_tick > 50000)
			{
				comm_app_disable_multiples();
			}
		}
	*/
	return OS_SUCCESS;
}

// 这里处理上位机的指令
static int on_handler_msg(THR_TASK_t *ptask, MQ_LIST_t *pmq)
{
	int msg_type = pmq->msg_node.type;

	os_dbg("!!!! msg %d coming(%d) !!!", pmq->msg_node.type, pmq->msg_node.msg_len);

	if (pmq->msg_node.msg_len >= 2 * sizeof(int))
	{
		int para1, para2;
		memcpy(&para1, pmq->msg_node.msg, sizeof(int));
		memcpy(&para2, pmq->msg_node.msg + sizeof(int), sizeof(int));
		setting_msg_handle(msg_type, para1, para2);
	}

	switch (pmq->msg_node.type)
	{
	default:
		break;
	}
	return OS_SUCCESS;
}

static int on_init_sched(THR_TASK_t *ptask)
{
	EVENT_HANDLER_LIST_t handler_list;
	EVENT_HANDLER_LIST_INIT(handler_list, INVALID_HANDLE_VALUE, EVENT_TIMEOUT | EVENT_INTERVAL);
	handler_list.event.timeout = on_timeout_msg;
	handler_list.event.timeout_val = 10000; // 10 sec timer
	register_event_handle(ptask, &handler_list);
	set_task_base_timer_calc(ptask, 100); // change timer scheder

	register_msg_handler(ptask, on_handler_msg);
	return OS_SUCCESS;
}
// first call in main function
void init_common_lib()
{
	comm_mempool_init();
	update_server_config();
	os_uuid_init();
	net_stream_initStreamLib();
	net_stream_initAlarmStream();
	Client_initService();
}

static pthread_mutex_t lock;
int comm_init_sched()
{
	int iRet;
	g_task_mgr = init_thr_mgr(CODEC_MAX_TASK, CODEC_MAX_EVENT, CODEC_MAX_MQ);
	init_timer_task(g_task_mgr, NULL);
	iRet = register_task(g_task_mgr, BASE_SCHED_MODULE, NULL, on_init_sched, 0);
	if (iRet == OS_FALSE)
	{
		os_dbg("register_task : %x faild", BASE_SCHED_MODULE);
		return OS_FAILD;
	}

	pthread_mutex_init(&lock, NULL);
	return OS_SUCCESS;
}

// send msg  --- 不确定是线程安全的，加把锁。
int sched_send_msg(int msg_type, void *pMsg, int msg_len)
{
	MSG_NODE_t msg;
	msg.dst_mid = BASE_SCHED_MODULE;
	msg.src_mid = -1;
	msg.type = msg_type;
	msg.msg = pMsg;
	msg.msg_len = msg_len;

	pthread_mutex_lock(&lock);
	int ret = send_msg(g_task_mgr, &msg);
	pthread_mutex_unlock(&lock);

	return ret;
}

void *comm_app_get_task_mgr()
{
	return g_task_mgr;
}
