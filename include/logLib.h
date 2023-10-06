#ifndef __logLib_h__
#define __logLib_h__


/** 
 * 版权所有: 2011 金三立视频科技（深圳）有限公司
 * 文件 名 : logLib.h
 * 摘 要   : log manage module
 * 创建日期: 2011-03-01
 * 作 者   : 姓名 maohongwei@gmail.com
 * 修 改   : 姓名日期 修改内容描述 
 */
#ifdef __cplusplus
extern "C"{
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#define LOG_VERSION     "V1.3"

#define LOG_MAX_ITEM_NUM    (1024)
#define LOG_MAX_DESC_LEN     (128)

#define LOG_TYPE_ALL         0x00  //所有日志
#define LOG_TYPE_SYSTEM      0x01  //系统日志
#define LOG_TYPE_ALARM       0x02  //报警日志
#define LOG_TYPE_OPERATE     0x03  //操作日志
#define LOG_TYPE_NETWORK     0x04  //网络日志


typedef struct log_item_s {
    int     type;
    int     time;
    char    desc[LOG_MAX_DESC_LEN];
}log_item_t;


typedef struct log_list_s {
    log_item_t  node;
    struct log_list_s *next;
}log_list_t;


void hisi_printd( FILE *fp
                , const char *module_name
                , const char *file_name
                , int  line
                , const char *function_name
                , const char *fmt,...);

int hisi_log_write(  char *module_name
                    , int type
                    , char *fmt,...);

int hisi_log_query(  char *module_name
                    , int type
                    , int begin_time
                    , int end_time
                    , log_list_t **pp_log_list);

int hisi_log_delete(  char *module_name
                    , int type
                    , int begin_time
                    , int end_time);

int hisi_log_sync(void);

/**
 *  exprot interface
 *
 */
#ifndef MODULE_NAME
#define MODULE_NAME "log"
#endif


/**
 *
 *  
 *
 */
#define LOG_DBG_MEM     2
#define LOG_DBG_STDERR  1

int st_log_initLib(int dbg_mask);

/**
 *
 *
 */
int st_log_deinitLib(void);


#define printf(args...)     hisi_printd(stderr,MODULE_NAME,__FILE__,__LINE__,__FUNCTION__,##args)
#define st_printf(args...)  hisi_printd(stderr,MODULE_NAME,__FILE__,__LINE__,__FUNCTION__,##args)
#define st_log_write(type, args...) hisi_log_write(MODULE_NAME,type,##args)
#define st_log_query(type, begin_time, end_time, log_list)  hisi_log_query(MODULE_NAME, type, begin_time, end_time, log_list)
#define st_log_delete(type, begin_time, end_time)  hisi_log_delete(MODULE_NAME, type, begin_time, end_time)
#define st_log_flush   hisi_log_sync 


#ifdef __cplusplus
}
#endif

#endif //__logLib_h__
