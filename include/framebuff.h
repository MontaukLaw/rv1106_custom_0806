#ifndef __FRAME_BUFF_12345678910
#define __FRAME_BUFF_12345678910

#include "os_ini.h"
/************************************************************************
* frame buff 分为两种，一种用于av stream 可以从8k到32k的一个梯度，
* 大于32K的则需要多个来来级联，一种用于普通的数据 ，可自定义大小 
************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FRAME_FRAG	100		
#define DEFAULT_FRAME_NUM	256
#define FRAME_BUFF_UNIT (8*1024)


struct frame_buff_mgr;

struct frame_buff
{
	struct frame_buff *next;
	struct frame_buff *prev;
	struct frame_buff *instance;

	//一下两个用于stream frame
	OS_UINT16_t frame_type;         //帧类型
	OS_UINT16_t frame_ch;            //帧通道
	OS_INT32_t  code_type;
	OS_UINT32_t len;                //可用数据大小,单buff的大小
	OS_UINT32_t curr_len;			//用于指示当前邋frame_buffer的数据大小
	OS_UINT32_t data_len;            //帧大小,经过处理后的大小，用于释放buff，以向上取整为准
	OS_UINT32_t truesize;            //总数据大小,为几帧数据大小，这个是分配数据的实际大小
	OS_UINT32_t frame_no;           //帧序号
	OS_UINT32_t used_len;       //实际拷贝进去的总数据，用于frame_copy_data记录，大小应该等同于len
//=================以下新增的====================
	OS_UINT32_t device_type;
	OS_INT32_t video_standard;
	OS_INT32_t frame_rate;
	OS_INT32_t video_resolution;
	OS_UINT32_t sec;
	OS_UINT32_t usec;
//=========================================	
	OS_UINT64_t timestamp;          //精确到毫秒级时间戳

	unsigned char *head;      //指向buff的头部
	unsigned char *end;       //指向buff的尾部
	unsigned char *data;      //指向数据的头部
	unsigned char *tail;      //指向数据的尾部
	unsigned char *curr_data; //在有多个邋frame_buffer list的时候，用于指向当前邋frame_buffer的data和tail
	unsigned char *curr_tail;

	void (*init)(struct frame_buff *);
	void (*destroy)(struct frame_buff *);   //用于销毁结构体
	struct frame_buff_mgr *pmgr;			//用于保存自己的mgr，便于释放
	void *priv;                           //用户私有数据指针，可以在从head 开始.也可以指向别的地方
};

typedef struct frame_buff frame_buff_t;

struct frame_shared_info      //用于帧的引用计数
{
	volatile int user_ref;       //引用计数
	int nr_frags;       //碎片的个数
	OS_MUTEX_t  mutex;
	struct frame_buff *frag_list;   //用于多个frame list连接
};

typedef struct frame_shared_info frame_shared_info_t;

struct frame_buff_head
{
	struct frame_buff *next;
	struct frame_buff *prev;
	OS_UINT32_t  qlen;
	OS_MUTEX_t mutex;
};

typedef struct frame_buff_head frame_buff_head_t;

struct mempool_mgr;
struct frame_buff_mgr
{
	struct mempool_mgr *data_wrap_pool; 
	struct mempool_mgr *data_buff_pool;
	int wrap_pool_num; //buff wrap的个数，如果需要进行多个共享的话，则wrap_pool_num比 buff_pool_num 多几倍
	int buff_pool_num;
	int buff_size; //buff的长度
};
typedef struct frame_buff_mgr frame_buff_mgr_t;

//返回buff的end指针
static OS_INLINT_t unsigned char *frame_end_pointer(const struct frame_buff *frame)
{
	return frame->end;
}

#define frame_shinfo(frame) ((struct frame_shared_info *)(frame_end_pointer(frame)))

// 用于获取frame buff 链表中的下一个节点
static OS_INLINT_t frame_buff_t *__frame_buff_next(frame_buff_t *frame)
{
	frame_shared_info_t *pshinfo_frame = frame_shinfo(frame);
	return pshinfo_frame->frag_list;
}

//插入frame_buff
static OS_INLINT_t void __frame_queue_after(frame_buff_head_t *list,
										 frame_buff_t *prev, frame_buff_t *newbuf)
{
	frame_buff_t *next;
	list->qlen++;
	next = prev->next;
	newbuf->next = next;
	newbuf->prev = prev;
	next->prev = prev->next = newbuf;
}

//在头部插入
static OS_INLINT_t void __frame_queue_head(frame_buff_head_t *list,frame_buff_t *newbuf)
{
	__frame_queue_after(list, (frame_buff_t * )list, newbuf);
}

//在尾部插入
static OS_INLINT_t void __frame_queue_tail(frame_buff_head_t *list,frame_buff_t *newbuf)
{
	frame_buff_t *prev,*next;

	list->qlen++;
	next = (frame_buff_t *)list;
	prev = next->prev;
	newbuf->next = next;
	newbuf->prev = prev;
	next->prev = prev->next = newbuf;
}
//头部获取
static OS_INLINT_t frame_buff_t*__frame_dequeue(frame_buff_head_t *list)
{
	frame_buff_t *next,*prev,*result;
	prev = (frame_buff_t *)list;
	next = prev->next;
	result = NULL;
	if(next != prev)
	{
		result = next;
		next = next->next;
		list->qlen--;
		next->prev = prev;
		prev->next = next;
		result->next = result->prev = NULL;
	}
	return result;
}

static OS_INLINT_t void __frame_insert(frame_buff_t *newbuf,
									frame_buff_t *prev,frame_buff_t *next,
									frame_buff_head_t *list)
{
	newbuf->next = next;
	newbuf->prev = prev;
	next->prev = prev->next = newbuf;
	list->qlen++;
}

static OS_INLINT_t void __frame_append(frame_buff_t *old,frame_buff_t *newbuf,frame_buff_head_t *list)

{
	__frame_insert(newbuf,old,old->next,list);
}

static OS_INLINT_t void __frame_unlink(frame_buff_t *frame,frame_buff_head_t *list)
{
	frame_buff_t *next,*prev;
	list->qlen--;
	next = frame->next;
	prev = frame->prev;
	frame->next = frame->prev = NULL;
	next->prev = prev;
	prev->next = next;
}

static OS_INLINT_t frame_buff_t *frame_peek_tail(frame_buff_head_t *list_)
{
	frame_buff_t *list = ((frame_buff_t *)list_)->prev;
	if(list == (frame_buff_t *)list_)
	{
		list = NULL;
	}
	return list;
}

static OS_INLINT_t int frame_queue_len(frame_buff_head_t *list_)
{
	int qlen;
	os_mutex_lock(&list_->mutex);
	qlen = list_->qlen;
	os_mutex_unlock(&list_->mutex);
	return qlen;
}


static OS_INLINT_t frame_buff_t *__frame_dequeue_tail(frame_buff_head_t *list)
{
	frame_buff_t *frame = frame_peek_tail(list);
	if(frame)
	{
		__frame_unlink(frame,list);
	}
	return frame;
}

static OS_INLINT_t int frame_queue_empty(const frame_buff_head_t *list)
{
	return list->next == (frame_buff_t *)list;
}
//返回数据的尾部
static OS_INLINT_t unsigned char *frame_tail_pointer(frame_buff_t *frame)
{
	return frame->tail;
}
//重置数据的尾部
static OS_INLINT_t void frame_reset_tail_pointer(frame_buff_t *frame)
{
	frame->tail = frame->data;
}
//设置数据的尾部
static OS_INLINT_t void frame_set_tail_pointer(frame_buff_t *frame,const int offset)
{
	frame->tail = frame->data + offset;
}
//将数据尾部重新指向头且长度清零
static OS_INLINT_t void __frame_init(frame_buff_t *frame)
{
	frame->tail = frame->data;
	frame->len = 0;
	frame->used_len = 0;
}
//返回数据尾部指针，同时更新尾部指针和长度
static OS_INLINT_t unsigned char *__frame_put(frame_buff_t *frame,unsigned int len)
{
	unsigned char *temp = frame_tail_pointer(frame);
	frame->tail += len;
	frame->len += len;
	return temp;
}
//在数据头部增加len字节数据
static OS_INLINT_t unsigned char *__frame_push(frame_buff_t *frame,unsigned int len)
{
	frame->data -= len;
	frame->len += len;
	return frame->data;
}
//从数据头部取走len字节数据，更新buff长度
static OS_INLINT_t unsigned char *__frame_pull(frame_buff_t *frame,unsigned int len)
{
	unsigned char *ptmp = frame->data;
	frame->len -= len;
	frame->data +=len;
	return ptmp;
}
//在buff头部保留len字节长度
static OS_INLINT_t void frame_reserver(frame_buff_t *frame,int len)
{
	frame->data += len;
	frame->tail += len;
	frame->curr_data += len;
	frame->curr_tail += len;
}

//直接设置buff的长度
static OS_INLINT_t void __skb_trim(frame_buff_t *frame,unsigned int len)
{
	frame->len = len;
	frame_set_tail_pointer(frame,len);
}


//从队列头取出数据
extern frame_buff_t *frame_dequeue(frame_buff_head_t *list);
//从队列尾取出数据
extern frame_buff_t *frame_dequeue_tail(frame_buff_head_t *list);
//把frame从队列中断开
extern void frame_unlink(frame_buff_t *frame,frame_buff_head_t *list);
//把frame放到old frame的后面
extern void frame_append(frame_buff_t *old,frame_buff_t *newbuf,frame_buff_head_t *list);
//把frame放到old frame的前面
extern void frame_insert(frame_buff_t *old,frame_buff_t *newbuf,frame_buff_head_t *list);
//把frame放到队列的尾部
extern void frame_queue_tail(frame_buff_head_t *list,frame_buff_t *newbuf);
//把frame放到队列的首部
extern void frame_queue_head(frame_buff_head_t *list,frame_buff_t *newbuf);

//分配一个frame
extern frame_buff_t *alloc_frame(frame_buff_mgr_t *pmgr,unsigned int size);
//释放 一个 frame
extern void free_frame(frame_buff_t *frame);
//克隆一个frame 到新的frame
extern frame_buff_t *frame_clone(frame_buff_t *frame);
//拷贝一个frame到新的frame
extern frame_buff_t *frame_copy(frame_buff_t *frame);
//初始化 frame队列
extern void init_frame_buff_head(frame_buff_head_t *list);
//初始化 frame内存管理器
extern OS_RESULT_t  init_frame_buff_mgr(frame_buff_mgr_t *pmgr,const char *wrap_name,
										   const char *data_name,int wrap_pool_num,int buff_pool_num,
										   int data_buff_size);

extern void destroy_frame_buff_mgr(frame_buff_mgr_t *pmgr);
//给buff中拷贝数据，
extern int frame_copy_data(frame_buff_t *frame,unsigned char *buff,int len);
extern int frame_copy_frame(frame_buff_t *pframe,unsigned char *buff,int len,int *nResult);

extern void monitor_frame_buff();

/********************************************************************
* 设计一个通用的data buffer，给用户更多的使用权限
* 其操作和结够体类似于frame_buff，不同之处在于databuffer使用线性操作
* 分配固定大小的空间
********************************************************************/
//使用同样的数据结构和同样的方法
typedef struct frame_buff  data_buff_t;
typedef struct frame_buff_head data_buff_head_t;
typedef struct frame_buff_mgr data_buff_mgr_t;


/********************************************************************
* 适合framebuff的操作同样也使用于 databuff
*********************************************************************/
//从队列头取出数据
extern data_buff_t *data_dequeue(data_buff_head_t *list);
//从队列尾取出数据
extern data_buff_t *data_dequeue_tail(data_buff_head_t *list);
//把data从队列中断开
extern void data_unlink(data_buff_t *frame,data_buff_head_t *list);
//把data放到old data的后面
extern void data_append(data_buff_t *old,data_buff_t *newbuf,data_buff_head_t *list);
//把data放到old data的前面
extern void data_insert(data_buff_t *old,data_buff_t *newbuf,data_buff_head_t *list);
//把data放到队列的尾部
extern void data_queue_tail(data_buff_head_t *list,data_buff_t *newbuf);
//把data放到队列的首部
extern void data_queue_head(data_buff_head_t *list,data_buff_t *newbuf);

//分配一个data
extern data_buff_t *alloc_data(data_buff_mgr_t *pmgr);
//释放 一个 frame
extern void free_data(frame_buff_t *frame);
//克隆一个data 到新的data
extern data_buff_t *data_clone(data_buff_t *frame);
//拷贝一个data到新的data
extern data_buff_t *data_copy(data_buff_t *frame);
//初始化 data队列
extern void init_data_buff_head(data_buff_head_t *list);
//初始化 frame内存管理器
extern int init_data_buff_mgr(data_buff_mgr_t *pmgr,const char *wrap_name,const char *data_name,int wrap_pool_num,int buff_pool_num,int data_buff_size);
extern void destroy_data_buff_mgr(data_buff_mgr_t *pmgr);
//给buff中拷贝数据，
extern int data_copy_data(data_buff_t *frame,unsigned char *buff,int len);

extern void monitor_data_buff(data_buff_mgr_t *pmgr);



void frame_buff_set_debug_level(OS_INT32_t level);
extern OS_VERSION_t * frame_buff_get_version();
#ifdef __cplusplus
}
#endif

#endif
