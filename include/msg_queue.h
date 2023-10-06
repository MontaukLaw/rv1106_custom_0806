#ifndef _MSG_QUEUE_H_12345678910
#define _MSG_QUEUE_H_12345678910

#include "os_ini.h"
#ifdef __cplusplus
extern "C"
{
#endif

	struct THR_TASK_MGR;
#define MAX_MSG_NUM 1024
	struct MSG_NODE
	{
		OS_INT32_t src_mid;
		OS_INT32_t dst_mid;
		OS_INT32_t type;
		OS_INT32_t msg_len;
		OS_CHAR_t *msg;
	};

	typedef struct MSG_NODE MSG_NODE_t;

	struct MQ_LIST
	{
		struct MQ_LIST *next;
		MSG_NODE_t msg_node;
	};

	typedef struct MQ_LIST MQ_LIST_t;

	struct MQ_MGR
	{
		struct THR_TASK_MGR *task_mgr_p;
		MQ_LIST_t *head;
		MQ_LIST_t *tail;
		OS_INT32_t used;
		OS_MUTEX_t mutex;
	};
	typedef struct MQ_MGR MQ_MGR_t;

	// ��ʼ����Ϣ���й�����
	void init_msg_mgr(OS_INT32_t cap);
	void destroy_msg_mgr();
	void init_msg_queue(struct THR_TASK_MGR *task_mgr_p, MQ_MGR_t *mq);
	void destroy_msg_queue(MQ_MGR_t *mq);
	// ����Ϣ�ӵ�����β��
	OS_INT32_t put_msg_tail(MQ_MGR_t *mq, MSG_NODE_t *msg);
	// ����Ϣ�ӵ�����ͷ��
	OS_INT32_t put_msg_head(MQ_MGR_t *mq, MSG_NODE_t *msg);
	// ������е���Ϣ
	OS_BOOL clear_msg(MQ_MGR_t *mq);
	// ��ͷ����ȡһ����Ϣ
	MQ_LIST_t *get_msg(MQ_MGR_t *mq);
	// �ͷ�һ����Ϣ
	void free_msg(struct THR_TASK_MGR *task_mgr_p, MQ_LIST_t *msg);
	int get_msg_count(MQ_MGR_t *mq);

	void monitor_msg_queue();

	void msg_queue_set_debug_level(OS_INT32_t level);
	OS_VERSION_t *msg_queue_get_version();

#define MSG_TYPE_TIMEOUT_INTERVAL 0x12345

#ifdef __cplusplus
}
#endif

#endif
