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

#define THREADCREATEPOLICY_0		0  //按照正常的线程分配策略
#define THREADCREATEPOLICY_1		1  //运行中不动态分配线程
#define THREADCREATEPOLICY_2		2  //按照每次创建threadDelta个线程的策略
struct tagTaskInfo;
typedef void *(*start_routine)(struct tagTaskInfo *ower,void *arg1,void *arg2);
typedef void *(*free_routine)(struct tagTaskInfo *ower,void *arg1,void *arg2);
typedef struct tagThreadPoolAttr
{
	int minThreads;
	int maxThreads;
	int maxIdleTime;
	int threadDelta;	//每次增加的线程数量
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
	OS_COND_t start_and_shutdown;		//等待线程启动完成
	int lastTaskId;	//最后一个任务的id标示
	int shutdown; //线程关闭标志
	int busyThreads; //在运行任务的线程的数量
	ThreadPoolAttr_t threadPoolAttr; //线程池的属性
	ThreadPoolStats_t threadpoolstats;
	void * mempool_mgr; //用于管理线程任务
	//需要增加一个内存管理，来管理所有的任务
	TaskInfo_t lowTaskList;  //低优先级的队列
	TaskInfo_t medTaskList;  //中等优先级的队列
	TaskInfo_t highTaskList;  //高优先级的队列
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
