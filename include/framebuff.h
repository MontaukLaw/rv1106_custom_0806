#ifndef __FRAME_BUFF_12345678910
#define __FRAME_BUFF_12345678910

#include "os_ini.h"
/************************************************************************
* frame buff ��Ϊ���֣�һ������av stream ���Դ�8k��32k��һ���ݶȣ�
* ����32K������Ҫ�������������һ��������ͨ������ �����Զ����С 
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

	//һ����������stream frame
	OS_UINT16_t frame_type;         //֡����
	OS_UINT16_t frame_ch;            //֡ͨ��
	OS_INT32_t  code_type;
	OS_UINT32_t len;                //�������ݴ�С,��buff�Ĵ�С
	OS_UINT32_t curr_len;			//����ָʾ��ǰ��frame_buffer�����ݴ�С
	OS_UINT32_t data_len;            //֡��С,���������Ĵ�С�������ͷ�buff��������ȡ��Ϊ׼
	OS_UINT32_t truesize;            //�����ݴ�С,Ϊ��֡���ݴ�С������Ƿ������ݵ�ʵ�ʴ�С
	OS_UINT32_t frame_no;           //֡���
	OS_UINT32_t used_len;       //ʵ�ʿ�����ȥ�������ݣ�����frame_copy_data��¼����СӦ�õ�ͬ��len
//=================����������====================
	OS_UINT32_t device_type;
	OS_INT32_t video_standard;
	OS_INT32_t frame_rate;
	OS_INT32_t video_resolution;
	OS_UINT32_t sec;
	OS_UINT32_t usec;
//=========================================	
	OS_UINT64_t timestamp;          //��ȷ�����뼶ʱ���

	unsigned char *head;      //ָ��buff��ͷ��
	unsigned char *end;       //ָ��buff��β��
	unsigned char *data;      //ָ�����ݵ�ͷ��
	unsigned char *tail;      //ָ�����ݵ�β��
	unsigned char *curr_data; //���ж����frame_buffer list��ʱ������ָ��ǰ��frame_buffer��data��tail
	unsigned char *curr_tail;

	void (*init)(struct frame_buff *);
	void (*destroy)(struct frame_buff *);   //�������ٽṹ��
	struct frame_buff_mgr *pmgr;			//���ڱ����Լ���mgr�������ͷ�
	void *priv;                           //�û�˽������ָ�룬�����ڴ�head ��ʼ.Ҳ����ָ���ĵط�
};

typedef struct frame_buff frame_buff_t;

struct frame_shared_info      //����֡�����ü���
{
	volatile int user_ref;       //���ü���
	int nr_frags;       //��Ƭ�ĸ���
	OS_MUTEX_t  mutex;
	struct frame_buff *frag_list;   //���ڶ��frame list����
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
	int wrap_pool_num; //buff wrap�ĸ����������Ҫ���ж������Ļ�����wrap_pool_num�� buff_pool_num �༸��
	int buff_pool_num;
	int buff_size; //buff�ĳ���
};
typedef struct frame_buff_mgr frame_buff_mgr_t;

//����buff��endָ��
static OS_INLINT_t unsigned char *frame_end_pointer(const struct frame_buff *frame)
{
	return frame->end;
}

#define frame_shinfo(frame) ((struct frame_shared_info *)(frame_end_pointer(frame)))

// ���ڻ�ȡframe buff �����е���һ���ڵ�
static OS_INLINT_t frame_buff_t *__frame_buff_next(frame_buff_t *frame)
{
	frame_shared_info_t *pshinfo_frame = frame_shinfo(frame);
	return pshinfo_frame->frag_list;
}

//����frame_buff
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

//��ͷ������
static OS_INLINT_t void __frame_queue_head(frame_buff_head_t *list,frame_buff_t *newbuf)
{
	__frame_queue_after(list, (frame_buff_t * )list, newbuf);
}

//��β������
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
//ͷ����ȡ
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
//�������ݵ�β��
static OS_INLINT_t unsigned char *frame_tail_pointer(frame_buff_t *frame)
{
	return frame->tail;
}
//�������ݵ�β��
static OS_INLINT_t void frame_reset_tail_pointer(frame_buff_t *frame)
{
	frame->tail = frame->data;
}
//�������ݵ�β��
static OS_INLINT_t void frame_set_tail_pointer(frame_buff_t *frame,const int offset)
{
	frame->tail = frame->data + offset;
}
//������β������ָ��ͷ�ҳ�������
static OS_INLINT_t void __frame_init(frame_buff_t *frame)
{
	frame->tail = frame->data;
	frame->len = 0;
	frame->used_len = 0;
}
//��������β��ָ�룬ͬʱ����β��ָ��ͳ���
static OS_INLINT_t unsigned char *__frame_put(frame_buff_t *frame,unsigned int len)
{
	unsigned char *temp = frame_tail_pointer(frame);
	frame->tail += len;
	frame->len += len;
	return temp;
}
//������ͷ������len�ֽ�����
static OS_INLINT_t unsigned char *__frame_push(frame_buff_t *frame,unsigned int len)
{
	frame->data -= len;
	frame->len += len;
	return frame->data;
}
//������ͷ��ȡ��len�ֽ����ݣ�����buff����
static OS_INLINT_t unsigned char *__frame_pull(frame_buff_t *frame,unsigned int len)
{
	unsigned char *ptmp = frame->data;
	frame->len -= len;
	frame->data +=len;
	return ptmp;
}
//��buffͷ������len�ֽڳ���
static OS_INLINT_t void frame_reserver(frame_buff_t *frame,int len)
{
	frame->data += len;
	frame->tail += len;
	frame->curr_data += len;
	frame->curr_tail += len;
}

//ֱ������buff�ĳ���
static OS_INLINT_t void __skb_trim(frame_buff_t *frame,unsigned int len)
{
	frame->len = len;
	frame_set_tail_pointer(frame,len);
}


//�Ӷ���ͷȡ������
extern frame_buff_t *frame_dequeue(frame_buff_head_t *list);
//�Ӷ���βȡ������
extern frame_buff_t *frame_dequeue_tail(frame_buff_head_t *list);
//��frame�Ӷ����жϿ�
extern void frame_unlink(frame_buff_t *frame,frame_buff_head_t *list);
//��frame�ŵ�old frame�ĺ���
extern void frame_append(frame_buff_t *old,frame_buff_t *newbuf,frame_buff_head_t *list);
//��frame�ŵ�old frame��ǰ��
extern void frame_insert(frame_buff_t *old,frame_buff_t *newbuf,frame_buff_head_t *list);
//��frame�ŵ����е�β��
extern void frame_queue_tail(frame_buff_head_t *list,frame_buff_t *newbuf);
//��frame�ŵ����е��ײ�
extern void frame_queue_head(frame_buff_head_t *list,frame_buff_t *newbuf);

//����һ��frame
extern frame_buff_t *alloc_frame(frame_buff_mgr_t *pmgr,unsigned int size);
//�ͷ� һ�� frame
extern void free_frame(frame_buff_t *frame);
//��¡һ��frame ���µ�frame
extern frame_buff_t *frame_clone(frame_buff_t *frame);
//����һ��frame���µ�frame
extern frame_buff_t *frame_copy(frame_buff_t *frame);
//��ʼ�� frame����
extern void init_frame_buff_head(frame_buff_head_t *list);
//��ʼ�� frame�ڴ������
extern OS_RESULT_t  init_frame_buff_mgr(frame_buff_mgr_t *pmgr,const char *wrap_name,
										   const char *data_name,int wrap_pool_num,int buff_pool_num,
										   int data_buff_size);

extern void destroy_frame_buff_mgr(frame_buff_mgr_t *pmgr);
//��buff�п������ݣ�
extern int frame_copy_data(frame_buff_t *frame,unsigned char *buff,int len);
extern int frame_copy_frame(frame_buff_t *pframe,unsigned char *buff,int len,int *nResult);

extern void monitor_frame_buff();

/********************************************************************
* ���һ��ͨ�õ�data buffer�����û������ʹ��Ȩ��
* ������ͽṻ��������frame_buff����֮ͬ������databufferʹ�����Բ���
* ����̶���С�Ŀռ�
********************************************************************/
//ʹ��ͬ�������ݽṹ��ͬ���ķ���
typedef struct frame_buff  data_buff_t;
typedef struct frame_buff_head data_buff_head_t;
typedef struct frame_buff_mgr data_buff_mgr_t;


/********************************************************************
* �ʺ�framebuff�Ĳ���ͬ��Ҳʹ���� databuff
*********************************************************************/
//�Ӷ���ͷȡ������
extern data_buff_t *data_dequeue(data_buff_head_t *list);
//�Ӷ���βȡ������
extern data_buff_t *data_dequeue_tail(data_buff_head_t *list);
//��data�Ӷ����жϿ�
extern void data_unlink(data_buff_t *frame,data_buff_head_t *list);
//��data�ŵ�old data�ĺ���
extern void data_append(data_buff_t *old,data_buff_t *newbuf,data_buff_head_t *list);
//��data�ŵ�old data��ǰ��
extern void data_insert(data_buff_t *old,data_buff_t *newbuf,data_buff_head_t *list);
//��data�ŵ����е�β��
extern void data_queue_tail(data_buff_head_t *list,data_buff_t *newbuf);
//��data�ŵ����е��ײ�
extern void data_queue_head(data_buff_head_t *list,data_buff_t *newbuf);

//����һ��data
extern data_buff_t *alloc_data(data_buff_mgr_t *pmgr);
//�ͷ� һ�� frame
extern void free_data(frame_buff_t *frame);
//��¡һ��data ���µ�data
extern data_buff_t *data_clone(data_buff_t *frame);
//����һ��data���µ�data
extern data_buff_t *data_copy(data_buff_t *frame);
//��ʼ�� data����
extern void init_data_buff_head(data_buff_head_t *list);
//��ʼ�� frame�ڴ������
extern int init_data_buff_mgr(data_buff_mgr_t *pmgr,const char *wrap_name,const char *data_name,int wrap_pool_num,int buff_pool_num,int data_buff_size);
extern void destroy_data_buff_mgr(data_buff_mgr_t *pmgr);
//��buff�п������ݣ�
extern int data_copy_data(data_buff_t *frame,unsigned char *buff,int len);

extern void monitor_data_buff(data_buff_mgr_t *pmgr);



void frame_buff_set_debug_level(OS_INT32_t level);
extern OS_VERSION_t * frame_buff_get_version();
#ifdef __cplusplus
}
#endif

#endif
