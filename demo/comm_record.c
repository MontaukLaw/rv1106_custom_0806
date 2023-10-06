#include "common.h"
#include "comm_type.h"
#include "param.h"

#include "codec2_process.h"

#include "comm_app.h"
#include "uuid.h"
#include <stdbool.h>

#define MAX_PATH_SIZE 256
static char g_record_root_path[MAX_PATH_SIZE];
#define RECORD_ROOT_PATH "/userdata/media"
#define WRITE_BLOCK_SIZE 1024 * 1024

typedef struct __record_info_t__
{
	int ch;						   // 哪一个通道
	int ch_type;				   // 哪一路码流
	read_pos_t read_pos;		   // 读指针
	int data_len;				   // 有效数据长度
	int max_buf_len;			   // 最大缓存大小
	unsigned char *p_buff;		   // 本地视频缓存
	time_t last_flush_time;		   // 上一次刷新缓存时间
	int switch_time;			   // 文件切换时间
	char file_name[MAX_PATH_SIZE]; // 保存的文件名
	int fd;						   // 写文件的fd
	int record_flag;
	int pre_record_time; // 预录时间   <=0 不预录
	time_t start_time;	 // 开始录像的时间
	int state;			 // 状态，初始状态，正在录像状态，结束状态
	int iFrame_flag;	 //
	pthread_t tid;
	pthread_mutex_t mutex;
} record_info_t;

static record_info_t g_record_info[MAX_CHANN_NUM]; // 每个通道一个独立的
static void *RecordThread(void *pArg);

// 初始化
void comm_init_record(const char *pRecordPath)
{
	int ii;
	memset(&g_record_info, 0, sizeof(record_info_t) * MAX_CHANN_NUM);
	memset(g_record_root_path, 0, MAX_PATH_SIZE);
	if (pRecordPath != NULL)
	{
		strcpy(g_record_root_path, pRecordPath);
	}
	else
	{
		strcpy(g_record_root_path, RECORD_ROOT_PATH);
	}
	for (ii = 0; ii < MAX_CHANN_NUM; ii++)
	{
		g_record_info[ii].fd = INVALID_HANDLE_VALUE;
		g_record_info[ii].ch = ii;
		os_mutex_init(&g_record_info[ii].mutex);
		pthread_create(&g_record_info[ii].tid, NULL, RecordThread, &g_record_info[ii]);
	}
	return;
}

int comm_start_record(int ch, int ch_type, int switch_time)
{
	if (ch >= 0 && ch <= MAX_CHANN_NUM)
	{
		os_mutex_lock(&g_record_info[ch].mutex);
		if (g_record_info[ch].record_flag == OS_TRUE)
		{
			os_dbg("ch %d is already record", ch);
			os_mutex_unlock(&g_record_info[ch].mutex);
			return OS_SUCCESS;
		}
		g_record_info[ch].ch_type = ch_type;
		g_record_info[ch].switch_time = switch_time;
		if (g_record_info[ch].p_buff != NULL)
		{
			free(g_record_info[ch].p_buff);
			g_record_info[ch].p_buff = NULL;
		}
		g_record_info[ch].p_buff = (unsigned char *)malloc(MAX_RECORD_CACHE_SIZE);
		if (g_record_info[ch].p_buff == NULL)
		{
			os_dbg("ch %d is malloc cache faild", ch);
			os_mutex_unlock(&g_record_info[ch].mutex);
			return OS_FAILD;
		}
		g_record_info[ch].max_buf_len = MAX_RECORD_CACHE_SIZE;
		g_record_info[ch].data_len = 0;
		if (g_record_info[ch].fd != INVALID_HANDLE_VALUE)
		{
			os_close(g_record_info[ch].fd);
			g_record_info[ch].fd = INVALID_HANDLE_VALUE;
		}
		g_record_info[ch].last_flush_time = 0;
		g_record_info[ch].state = RECORD_STATE_INIT;
		g_record_info[ch].record_flag = OS_TRUE;
		os_mutex_unlock(&g_record_info[ch].mutex);
		return OS_SUCCESS;
	}
	return OS_FAILD;
}

int comm_stop_record(int ch)
{
	if (ch >= 0 && ch <= MAX_CHANN_NUM)
	{
		os_mutex_lock(&g_record_info[ch].mutex);
		if (g_record_info[ch].record_flag == OS_FALSE)
		{
			os_dbg("ch %d is already record stoped", ch);
			os_mutex_unlock(&g_record_info[ch].mutex);
			return OS_SUCCESS;
		}
		g_record_info[ch].record_flag = OS_FALSE;
		os_mutex_unlock(&g_record_info[ch].mutex);
	}
	return OS_SUCCESS;
}

int comm_get_record_state(int ch)
{
	int state;
	os_mutex_lock(&g_record_info[ch].mutex);
	state = g_record_info[ch].state;
	os_mutex_unlock(&g_record_info[ch].mutex);
	return state;
}

int comm_get_record_ctrl(int ch)
{
	int flag;
	os_mutex_lock(&g_record_info[ch].mutex);
	flag = g_record_info[ch].record_flag;
	os_mutex_unlock(&g_record_info[ch].mutex);
	return flag;
}

// 启动的时候做一个文件管理列表，后面没生成一个文件，就加载到对应的列表中，
// 原子操作:添加文件，按时间排序；删除文件，按时间排序；初始化，连接所有的录像文件
// 录像文件 和 抓拍文件采用同样的
//
void comm_init_record_fileList()
{
}
void comm_add_record_file(int chann, const char *pFileName)
{
}
void comm_del_record_file(int chann, const char *pFileName)
{
}
// 返回最老的一个文件
const char *comm_get_record_delfile(int chann)
{
}
void *DiskManagerThread(void *pArg)
{
	// 启动的时候文件管理，磁盘大小不够的时候，删除对应的文件
}
void *RecordThread(void *pArg)
{
	record_info_t *p_record_info = (record_info_t *)pArg;
	int ch = p_record_info->ch;
	int chn_type;
	int record_flag;
	int record_state;
	int *lock_pos = &p_record_info->read_pos.lock_pos;
	net_frame_t *p_frame = NULL;
	av_frame_head_t frame_head;
	time_t t;
	;
	char buf[64];
	while (1)
	{
		record_flag = comm_get_record_ctrl(ch);
		record_state = comm_get_record_state(ch);
		// os_dbg("record_flag == %d record_state == %d",record_flag,record_state);
		if (record_flag == OS_TRUE)
		{
			// os_dbg("record_flag == %d record_state == %d",record_flag,record_state);

			if (record_state == RECORD_STATE_INIT) // 刚启动录像
			{
				// os_dbg("record_flag == %d record_state == %d",record_flag,record_state);

				os_mutex_lock(&p_record_info->mutex);
				memset(&p_record_info->read_pos, 0, sizeof(p_record_info->read_pos));
				chn_type = p_record_info->ch_type;
				p_record_info->read_pos.read_begin = net_stream_getNetStreamWritePos(ch, chn_type);
				p_record_info->read_pos.read_end = net_stream_getNetStreamWritePos(ch, chn_type);
				p_record_info->read_pos.lock_pos = p_record_info->read_pos.read_begin;
				t = time(NULL);
				strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", localtime(&t));
				sprintf(p_record_info->file_name, "%s/%s_ch%d.h264", g_record_root_path, buf, ch);
				p_record_info->fd = open(p_record_info->file_name, O_RDWR | O_CREAT);
				p_record_info->start_time = t;
				p_record_info->state = RECORD_STATE_RUNNING; // 正在录像
				p_record_info->iFrame_flag = OS_TRUE;
				p_record_info->data_len = 0;
				os_mutex_unlock(&p_record_info->mutex);
				os_dbg("===========================create file: %s ", p_record_info->file_name);
			}
			chn_type = p_record_info->ch_type;
			lock_pos = &p_record_info->read_pos.lock_pos;
			memset(&frame_head, 0, sizeof(av_frame_head_t));
			net_stream_lockMutex(ch, chn_type, *lock_pos);
			p_frame = net_stream_getNetStreamFromPool(ch, chn_type, &p_record_info->read_pos);
			if (p_frame == NULL)
			{
				net_stream_unlockMutex(ch, chn_type, *lock_pos);
				net_stream_netStreamWaiting_timeout(ch, chn_type, 1000);
				continue;
			}
			if (p_record_info->iFrame_flag == OS_TRUE)
			{
				if ((p_frame->frame_head.frame_type & 0x7F) != CODEC_I_FRAME_TYPE)
				{
					net_stream_unlockMutex(ch, chn_type, *lock_pos);
					continue;
				}
				p_record_info->iFrame_flag = OS_FALSE;
			}
			memcpy(&frame_head, &(p_frame->frame_head), sizeof(frame_head));
			if (p_record_info->p_buff != NULL)
			{
				t = time(NULL);

				if ((p_frame->frame_head.frame_type & 0x7F) == CODEC_I_FRAME_TYPE)
				{
					// 切换录像文件，必须要从I帧开始
					if (t - p_record_info->start_time > p_record_info->switch_time)
					{
						if (p_record_info->fd != INVALID_HANDLE_VALUE && p_record_info->data_len > 0)
						{
							os_write(p_record_info->fd, p_record_info->p_buff, p_record_info->data_len);
							fsync(p_record_info->fd);
							p_record_info->last_flush_time = t;
							os_close(p_record_info->fd);
						}
						strftime(buf, sizeof(buf), "%Y-%m-%d_%H_%M_%S", localtime(&t));
						sprintf(p_record_info->file_name, "%s/%s_ch%d.h264", g_record_root_path, buf, ch);
						p_record_info->fd = open(p_record_info->file_name, O_RDWR | O_CREAT);
						p_record_info->start_time = t;
						p_record_info->data_len = 0;
						os_dbg("++++++++++++++++++++ switch to file: %s ", p_record_info->file_name);
					}
				}

				if (p_record_info->data_len + frame_head.frame_size > p_record_info->max_buf_len)
				{
					if (p_record_info->fd != INVALID_HANDLE_VALUE)
					{
						os_write(p_record_info->fd, p_record_info->p_buff, p_record_info->data_len);
						fsync(p_record_info->fd);
						p_record_info->last_flush_time = t;
					}
					p_record_info->data_len = 0;
				}
				memcpy(p_record_info->p_buff + p_record_info->data_len, p_frame->frame_data, frame_head.frame_size);
				net_stream_unlockMutex(ch, chn_type, *lock_pos);
				p_record_info->data_len += frame_head.frame_size;

				if (p_record_info->data_len > WRITE_BLOCK_SIZE || (t - p_record_info->last_flush_time) > 10)
				{
					if (p_record_info->fd != INVALID_HANDLE_VALUE && p_record_info->data_len > 0)
					{
						os_write(p_record_info->fd, p_record_info->p_buff, p_record_info->data_len);
						fsync(p_record_info->fd);
						p_record_info->last_flush_time = t;
					}
					p_record_info->data_len = 0;
				}
			}
		}
		else
		{
			if (record_state == RECORD_STATE_RUNNING)
			{
				os_dbg("ch:%d stop record", ch);
				if (p_record_info->fd != INVALID_HANDLE_VALUE && p_record_info->data_len > 0)
				{
					os_write(p_record_info->fd, p_record_info->p_buff, p_record_info->data_len);
					fsync(p_record_info->fd);
					p_record_info->last_flush_time = t;
					os_close(p_record_info->fd);
					p_record_info->fd = -1;
				}
				os_mutex_lock(&p_record_info->mutex);
				p_record_info->state = RECORD_STATE_END;
				os_mutex_unlock(&p_record_info->mutex);
			}
			usleep(2000000);
		}
	};
	return NULL;
}
