#ifndef _THREADPOOL_H_0123456789
#define _THREADPOOL_H_0123456789

#ifdef __cplusplus
extern "C" {
#endif

#define INFINITE_THREADS -1
#define EMAXTHREADS (-8 & 1<<29)

#define HIGH_PRIORITY  1
#define MED_PRIORITY 2
#define LOW_PRIORITY 3

#define THREADCREATEPOLICY_0		0  //�����������̷߳������
#define THREADCREATEPOLICY_1		1  //�����в���̬�����߳�
#define THREADCREATEPOLICY_2		2  //����ÿ�δ���threadDelta���̵߳Ĳ���
struct tagTaskInfo;
typedef void *(*start_routine)(struct tagTaskInfo *ower,void *arg1,void *arg2);
typedef void *(*free_routine)(struct tagTaskInfo *ower,void *arg1,void *arg2);
typedef struct tagThreadPoolAttr
{
	int minThreads;
	int maxThreads;
	int maxIdleTime;
	int threadDelta;	//ÿ�����ӵ��߳�����
	int tasksPerThread;
	int maxtasksTotal;
	int starvationTime;
	int schedPolicy;
	int threadCreatePolicy;
}ThreadPoolAttr_t, *PThreadPoolAttr_t;
typedef struct tagThreadPoolStats
{
	int totaltasksHQ;
	int totaltasksMQ;
	int totaltasksLQ;
	int workerThreads;
	int idleThreads;
	int totalThreads;
	int currenttasksHQ;
	int currenttasksMQ;
	int currenttasksLQ;
}ThreadPoolStats_t,*PThreadPoolStats_t;

typedef struct tagTaskInfo
{
	struct tagTaskInfo *next;
	struct tagTaskInfo *prev;
	struct tagTaskInfo *ower;
	start_routine start_func;
	void *start_arg1;
	void *start_arg2;
	free_routine free_func;
	void *free_arg1;
	void *free_arg2;
	struct timeval requestTime;
	int priority;
	int TaskId;
}TaskInfo_t,*PTaskInfo_t;

typedef struct tagThreadPoolMgr
{
	OS_MUTEX_t mutex;
	OS_COND_t  cond;  
	OS_COND_t start_and_shutdown;		//�ȴ��߳��������
	int lastTaskId;	//���һ�������id��ʾ
	int shutdown; //�̹߳رձ�־
	int busyThreads; //������������̵߳�����
	ThreadPoolAttr_t threadPoolAttr; //�̳߳ص�����
	ThreadPoolStats_t threadpoolstats;
	void * mempool_mgr; //���ڹ����߳�����
	//��Ҫ����һ���ڴ�������������е�����
	TaskInfo_t lowTaskList;  //�����ȼ��Ķ���
	TaskInfo_t medTaskList;  //�е����ȼ��Ķ���
	TaskInfo_t highTaskList;  //�����ȼ��Ķ���
}ThreadPoolMgr_t,*PThreadPoolMgr_t;

int ThreadPoolInit( ThreadPoolMgr_t *pThreadPool,ThreadPoolAttr_t *pAttr);
int ThreadPoolDestroy(ThreadPoolMgr_t *pThreadPool);
TaskInfo_t * ThreadPoolCreatTask(ThreadPoolMgr_t *pThreadPool);
int ThreadPoolSetTaskStartFunc(TaskInfo_t *pTask,start_routine start_func,void *start_arg1,void *start_arg2);
int ThreadPoolSetTaskFreeFunc(TaskInfo_t *pTask,free_routine free_func,void *free_arg1,void *free_arg2);
int ThreadPoolAddTask(ThreadPoolMgr_t *pThreadPool,TaskInfo_t *pTask,int priority,int *taskId);
void threadpool_set_debug_level(OS_INT32_t level);
OS_VERSION_t * threadpool_get_version();
void monitor_threadpool(ThreadPoolMgr_t *pThreadPool);
int ThreadPoolInitLibrary();
int ThreadPoolCleanUpLibrary();

#ifdef __cplusplus
}
#endif

#endif
