#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H

#include <unistd.h>
#include <stdint.h>
#include<stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>

#include "os_ini.h"
#include "mempool.h"
#include "linux_list.h"
#include "msg_queue.h"
#include "event_handler.h"
#include "taskmgr.h"
#include "framebuff.h"
#include "misc_comm.h"
#include "lib_http.h"
#include "log.h"
#include "threadpool.h"
#include "inifile.h"

#define COMM_API __attribute__((visibility("default")))

static int debug_level = INFO_MSG;

extern int h_errno;

#ifdef __cplusplus

#include <string>
#include <list>
#include <map>
typedef std::string CString;
using namespace std;
#endif


#if defined(__GNUC__)||defined(__GNUG__)
#ifndef atomic_inc
typedef volatile int  atomic_t;
#define atomic_inc(x) __sync_add_and_fetch((x),1)   
#define atomic_dec(x) __sync_sub_and_fetch((x),1)   
#define atomic_add(x,y) __sync_add_and_fetch((x),(y))   
#define atomic_sub(x,y) __sync_sub_and_fetch((x),(y))
#define atomic_cmp_swap(x,y,z) __sync_bool_compare_and_swap((x),(y),(z))
#define atomic_init(v) 	do{*(v) = 0;}while(0)
#endif
#endif

typedef void*      LPVOID;
typedef char*		LPSTR;
typedef const char*  LPCTSTR;

#define UPALIGNTO(value, align) ((value + align - 1) & (~(align - 1)))

#define UPALIGNTO16(value) UPALIGNTO(value, 16)

#define DOWNALIGNTO16(value) (UPALIGNTO(value, 16) - 16)

#define DOWNALIGNTO(value, align) (value & (~(align - 1)))

typedef void *(*LPTHREAD_START_ROUTINE)(LPVOID);

#define CREAT_FILE_MODE	(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

#ifdef __cplusplus
class CMutex
{
	public:
		CMutex(pthread_mutex_t &mutex)
			:m_pMutex(mutex)
		{
			lock();
		};
		~CMutex()
		{
			unlock();
		};
	private:
		void lock(){pthread_mutex_lock(&m_pMutex);};
		void unlock(){pthread_mutex_unlock(&m_pMutex);};
		pthread_mutex_t &m_pMutex;
};
#endif

#endif
