#ifndef _MEMPOOL_H_12345678910
#define _MEMPOOL_H_12345678910

#include "os_ini.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS_ALIGN	8
#define MAX_MEMPOOL_LEVEL	16
#define DEFAULT_CHUNK_INC		64
#define MAX_MEMPOOL_CHUNK		16
#define MAX_POOL_SIZE	128
#define MAX_POOL_CAP	(DEFAULT_CHUNK_INC*MAX_MEMPOOL_CHUNK)
#define OS_MAGIC_NUM	0xA5A5A5A5
#define MAX_MEMPOOL_NAME		32
typedef union mempool
{
	union mempool *next;
	OS_CHAR_t data[1];
}mempool_t;

struct mempool_mgr
{
	OS_MUTEX_t mutex;
	mempool_t *free_list;
	void *obj_list[MAX_MEMPOOL_CHUNK];
	
	volatile OS_INT32_t free;
	volatile OS_INT32_t used;
	volatile OS_INT32_t total;
	OS_INT32_t cap;
	OS_INT32_t delta_inc;
	OS_SIZE_t chunk_size;
	int index;  //obj_list 索引
	OS_CHAR_t	name[MAX_MEMPOOL_NAME];
};

typedef struct mempool_mgr mempool_mgr_t;

//通用内存分配管理
struct mempool_comm_mgr
{
	mempool_mgr_t mgr[MAX_MEMPOOL_LEVEL];
};
typedef struct mempool_comm_mgr mempool_comm_mgr_t;

mempool_comm_mgr_t *mempool_init();
OS_RESULT_t		mempool_destroy(mempool_comm_mgr_t * *comm_mgr_P);

void	*mempool_alloc(mempool_comm_mgr_t * comm_mgr_P,OS_SIZE_t size);
void mempool_free(mempool_comm_mgr_t * comm_mgr_P,void *obj);


OS_RESULT_t	mempool_init_spec(mempool_mgr_t *mempool_mgr_p,
	OS_INT32_t obj_num,OS_SIZE_t obj_size,OS_CHAR_CONST_PTR_t name);
OS_RESULT_t mempool_destroy_spec(mempool_mgr_t *mempool_mgr_p);
void *mempool_alloc_spec(mempool_mgr_t *mempool_mgr_p);
void	mempool_free_spec(mempool_mgr_t *mempool_mgr_p,void *obj);

OS_VERSION_t * mempool_get_version();

void monitor_mempool(mempool_comm_mgr_t * comm_mgr_P);
void monitor_spec_mempool(mempool_mgr_t *mempool_mgr_p);

void mempool_set_debug_level(OS_INT32_t level);

int comm_mempool_init();
void comm_mempool_destroy();
void *comm_mempool_alloc(OS_SIZE_t len);
void comm_mempool_free(void *obj);
void monitor_comm_mempool();

////////////////////////////
#define __USING_MEMPOOL_
#ifdef __USING_MEMPOOL_
#define os_malloc   comm_mempool_alloc
#define os_free		comm_mempool_free
#else
#define os_malloc   malloc
#define os_free		free
#endif


#ifdef __cplusplus
}
#endif

#endif
