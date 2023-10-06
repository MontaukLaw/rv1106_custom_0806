#ifndef _OS_INI_12345678910
#define _OS_INI_12345678910


//#define _OS_DEBUG
#define USING_SELECT
//#define USING_EPOLL

#ifdef __cplusplus
#define EXPORT_API extern "C" 
#else
#define EXPORT_API
#endif

///////////////////////////////////////
#ifdef _WIN32

     #ifndef WIN32_LEAN_AND_MEAN
     #define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
     #endif	//WIN32_LEAN_AND_MEAN

    #ifdef FD_SETSIZE
    #undef FD_SETSIZE
    #endif	//FD_SETSIZE

    #define FD_SETSIZE 1024
    
    #include <windows.h>
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    
    #pragma warning(disable:4311)

   #if	defined __MINGW32__
       #include <unistd.h>
       #include <stdint.h>
       #include <pthread.h>
       #include <sys/time.h>
       #include <fcntl.h>
       #include <time.h>	
   	typedef uint8_t		OS_UINT8_t;
   	typedef uint16_t	OS_UINT16_t;
   	typedef uint32_t	OS_UINT32_t;
   	typedef uint64_t	OS_UINT64_t;
   	
   	typedef int8_t	OS_INT8_t;
   	typedef int16_t	OS_INT16_t;
   	typedef int32_t	OS_INT32_t;
   	typedef int64_t	OS_INT64_t;
   	
   	#define OS_INLINT_t		inline
   	
   	typedef int OS_HANDLE_t;
   	typedef int OS_SOCKET_t;
   	typedef struct	sockaddr_in OS_SOCKADDR_IN;
   	typedef struct	sockaddr OS_SOCKADDR;
   	#define INVALID_SOCKET_VALUE	-1
   	
   	#ifndef INVALID_HANDLE_VALUE
   	#define INVALID_HANDLE_VALUE	INVALID_SOCKET_VALUE
   	#endif	//INVALID_HANDLE_VALUE
   	
      #define USING_OS_POSIX_PTHREAD
      #define ADDR(addr_)    inet_ntoa((addr_).sin_addr)
      #define PORT(addr_)    ntohs((addr_).sin_port)
      #define SIN_ADDR_IN(addr_) (addr_).sin_addr.s_addr
      #define SIN_PORT_IN(addr_) (addr_).sin_port
   
   #else
   
   	#pragma comment(lib,"Ws2_32.lib")
	#pragma comment(lib,"libpthreadGC2.dll.a")
   	#define USING_OS_POSIX_PTHREAD
	#include <pthread.h>
   	typedef UINT8	OS_UINT8_t;
   	typedef UINT16	OS_UINT16_t;
   	typedef UINT32	OS_UINT32_t;
   	typedef UINT64	OS_UINT64_t;
   	
   	typedef INT8	OS_INT8_t;
   	typedef INT16	OS_INT16_t;
   	typedef INT32	OS_INT32_t;
   	typedef INT64	OS_INT64_t;
   	
   	#define OS_INLINT_t	
   	
   	typedef HANDLE	OS_HANDLE_t;
   	typedef SOCKET	OS_SOCKET_t;
   	typedef struct	sockaddr_in OS_SOCKADDR_IN;
   	typedef struct	sockaddr OS_SOCKADDR;
   	#define INVALID_SOCKET_VALUE INVALID_SOCKET
   	
       #define ADDR(addr_)    inet_ntoa((addr_).sin_addr)
       #define PORT(addr_)    ntohs((addr_).sin_port)
       #define SIN_ADDR_IN(addr_) (addr_).sin_addr.s_addr
       #define SIN_PORT_IN(addr_) (addr_).sin_port
   
   #endif	//__MINGW32__

#else  //Linux

   #include <unistd.h>
   #include <stdint.h>
   #include <pthread.h>
   #include <arpa/inet.h>
   #include <net/if.h>
   #include <netinet/in.h>
   #include <sys/time.h>
   #include <fcntl.h>
   #include <time.h> 
   #include <poll.h>
   typedef uint8_t	OS_UINT8_t;
   typedef uint16_t	OS_UINT16_t;
   typedef uint32_t	OS_UINT32_t;
   typedef uint64_t	OS_UINT64_t;
   
   typedef int8_t	OS_INT8_t;
   typedef int16_t	OS_INT16_t;
   typedef int32_t	OS_INT32_t;
   typedef int64_t	OS_INT64_t;
   
   #define OS_INLINT_t		inline
   
   typedef int OS_HANDLE_t;
   typedef int OS_SOCKET_t;
   typedef struct	sockaddr_in OS_SOCKADDR_IN;
   typedef struct	sockaddr OS_SOCKADDR;
   
   #define SetSocketAddrIN(addr, family, port, _addr)	\
   do{		\
       addr.sin_family = family;		\
       addr.sin_port = htons(port);		\
       addr.sin_addr.s_addr = inet_addr(_addr);		\
   }while(0)

//#define USING_HIP_OS_CONFIG
#define INVALID_SOCKET_VALUE	-1
#define INVALID_HANDLE_VALUE	INVALID_SOCKET_VALUE
#define closesocket(fd) close(fd)
#define USING_OS_POSIX_PTHREAD
#ifdef USING_HIP_OS_CONFIG
#include "common.h"
#endif


int os_comm_isReadable(int fd,int msec);
int os_comm_isWriteable(int fd,int msec);
ssize_t os_comm_readn(int fd, void *vptr, size_t n);
ssize_t os_comm_writen(int fd, const void *vptr, size_t n);
ssize_t os_comm_readn_timeout(int fd, void *vptr, size_t n,int msec);
ssize_t os_comm_writen_timeout(int fd, const void *vptr, size_t n,int msec);

#endif	//_WIN32

#define ADDR(addr_)    inet_ntoa((addr_).sin_addr)
#define PORT(addr_)    ntohs((addr_).sin_port)
#define SIN_ADDR_IN(addr_) (addr_).sin_addr.s_addr
#define SIN_PORT_IN(addr_) (addr_).sin_port
#define IN
#define OUT
#define INOUT

typedef unsigned char uchar_t ;
typedef unsigned short ushort_t;
#ifndef uint16_t
typedef OS_UINT16_t uint16_t;
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <assert.h>
#include<signal.h>
#include <errno.h>

typedef char *	OS_CHAR_PTR_t;
typedef const char *	OS_CHAR_CONST_PTR_t;

typedef unsigned char *	OS_UCHAR_PTR_t;
typedef const unsigned char *	OS_UCHAR_CONST_PTR_t;

typedef char OS_CHAR_t;

typedef unsigned char OS_UCHAR_t;

typedef size_t	OS_SIZE_t;
typedef size_t	OS_SSIZE_t;

#define OS_TRUE		1
#define OS_FALSE	0
#define OS_SUCCESS	0
#define OS_FAILD	(-1)
typedef OS_INT32_t	OS_RESULT_t;

typedef struct __OS_VERSION_tag
{
	char major;	//主版本
	char minor;	//次版本
	char revised; //修订版本
	char	res;		//保留字段
	struct  
	{
		const char * data_desc;	// build date
		const char * time_desc; //build time
	}build_desc;
}OS_VERSION_t;



#ifdef _WIN32
typedef struct _stat		OS_STAT_t;
#define os_strncasecmp		_strnicmp
#define os_stat(file,buff) _stat((file),(buff))
#define os_snprintf		_snprintf
#define os_sleep(x)		Sleep(x)
#else
typedef struct stat		OS_STAT_t;
#define os_strncasecmp	  strncasecmp
#define os_stat(file,buff) stat((file),(buff))
#define os_snprintf		snprintf
#define os_sleep(x)		usleep((x)*1000)
#endif	//_WIN32



#ifdef _SDK_TEST
#ifdef WIN32
	#ifndef __MINGW32__
		#ifdef _USRDLL
		#define EXPORT_TEST_API   __declspec(dllexport)
		#else
		#define EXPORT_TEST_API   __declspec(dllimport)
		#endif
	#else
		#define EXPORT_TEST_API
	#endif
	#else
	#define EXPORT_TEST_API
	#endif
#else
	#define EXPORT_TEST_API
#endif

typedef OS_INT32_t OS_BOOL;
#define OS_TRUE		1
#define OS_FALSE	0


EXPORT_API void os_set_nonblock(OS_SOCKET_t fd);

#ifdef _WIN32
#define os_isNetNeedRdWrMore \
		(WSAEWOULDBLOCK == WSAGetLastError()||WSAEINTR == WSAGetLastError())
#else
#define os_isNetNeedRdWrMore (errno == EAGAIN || errno == EINTR)
#endif

#ifdef _WIN32
	#if	defined __MINGW32__
		#define os_gettimeofday gettimeofday
	#else
		EXPORT_API int os_gettimeofday(struct timeval *tv,struct timezone *tz);
	#endif
		EXPORT_API OS_RESULT_t os_pipe(OS_SOCKET_t fd[2]);
		EXPORT_API void os_close(OS_SOCKET_t fd);
		EXPORT_API int os_read(OS_SOCKET_t s,char *buff,int len);
		EXPORT_API int os_write(OS_SOCKET_t s,const void *buff,int len);

#else
#define os_pipe	 pipe
#define os_read read
#define os_write write
#define os_close close
#define os_gettimeofday gettimeofday
#endif



#ifdef USING_OS_POSIX_PTHREAD
#include <pthread.h>

typedef pthread_mutex_t      OS_MUTEX_t;
#define os_mutex_lock(mutex)       pthread_mutex_lock((mutex))
#define os_mutex_unlock(mutex)     pthread_mutex_unlock((mutex))
#define os_mutex_init(mutex)       pthread_mutex_init((mutex),NULL)
#define os_mutex_destroy(mutex)    pthread_mutex_destroy((mutex))

typedef pthread_cond_t OS_COND_t;

EXPORT_API  OS_INLINT_t int os_cond_timedwait_0(OS_COND_t* a,OS_MUTEX_t* b, OS_INT32_t usec)
{
	struct timespec abstime;
	struct timeval now;
    os_gettimeofday(&now, NULL);
	abstime.tv_sec = now.tv_sec + usec/1000;
	abstime.tv_nsec = now.tv_usec * 1000 + (usec%1000)*1000;
	return pthread_cond_timedwait(a, b, &abstime);
}

#define os_cond_init(cond)       pthread_cond_init((cond), NULL)
#define os_cond_wait(cond, mutex)       pthread_cond_wait((cond), (mutex))
#define os_cond_timedwait(cond, mutex, usec) os_cond_timedwait_0((cond),(mutex),(usec))
#define os_cond_signal(cond)       pthread_cond_signal((cond))
#define os_cond_destroy(cond)       pthread_cond_destroy((cond))
#else
typedef CRITICAL_SECTION    OS_MUTEX_t;
#define os_mutex_lock(mutex)       EnterCriticalSection((mutex))
#define os_mutex_unlock(mutex)     LeaveCriticalSection((mutex))
#define os_mutex_init(mutex)       InitializeCriticalSection((mutex))
#define os_mutex_destroy(mutex)    DeleteCriticalSection((mutex))

#define os_cond_mutex_lock(mutex)       os_mutex_lock((mutex))
#define os_cond_mutex_unlock(mutex)     os_mutex_unlock((mutex))
#define os_cond_mutex_init(mutex)	   os_mutex_init((mutex))
#define os_cond_mutex_destroy(mutex)	os_mutex_destroy((mutex))
typedef HANDLE		OS_COND_t;
EXPORT_API  OS_INLINT_t int os_cond_timedwait_0(OS_COND_t* a,OS_MUTEX_t* b,int c)
{
	DWORD ret; 

	ResetEvent(*(a));
	os_mutex_unlock((b));
	ret = WaitForSingleObject(*a, c); 
	os_mutex_lock((b)); 
	return ret;
}

#define os_get_time(abstime) ((abstime)->tv_sec*1000+(abstime)->tv_nsec/(1000*1000))
#define os_cond_init(cond) do {HANDLE hcond=CreateEvent(NULL,FALSE,TRUE,FALSE); if(hcond != NULL)*cond = hcond; else {assert(NULL);}} while(0)
#define os_cond_wait(cond, mutex) do{ResetEvent(*(con));os_mutex_unlock((mutex));WaitForSingleObject(*(con),INFINITE);os_mutex_lock((mutex));}while(0)
#define os_cond_timedwait(cond, mutex, tmspace) os_cond_timedwait_0((cond),(mutex),os_get_time((tmspace)))

#define os_cond_signal(cond) SetEvent(*(cond))
#define os_cond_destroy(cond) do {if((cond))CloseHandle(*(cond));*(cond)=NULL;}while(0)

#endif

#ifdef _OS_DEBUG
#define OS_ASSERT(arg)  assert(arg)
#else
#define OS_ASSERT(arg)
#endif

#endif
