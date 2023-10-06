#ifndef _LIB_HTTP_H
#define _LIB_HTTP_H

#include "os_ini.h"
#include "framebuff.h"

#ifdef __cplusplus
extern "C" {
#endif

//定义http 处理返回结果
#define HTTP_PARSER_OK      0x0   //刚好解析成功
#define HTTP_PARSER_NEED_MORE   0x01    //需要更多的字符进行解析
#define HTTP_PARSER_CONTINUE    0x02    //还有剩余的数据进行解析
#define HTTP_PARSER_ERROR       0xFFFF  //解析出错

#define HTTP_WRITE_AGAIN        0x01
#define HTTP_WRITE_ERROR        0xFFFF
#define HTTP_WRITE_OK           0x0
#define MAX_HTTP_CONTENT_LEN		64*1024
//把所有http的头方法定义成索引的方式，再以索引的方式获取
//对于其他不认识的头,则忽略

enum HTTP_HDR_INDEX_
{
    /*http通用头*/
    HTTP_GEN_HDR_CACHE_CONTROL = 0,
    HTTP_GEN_HDR_CONNECTION,
    HTTP_GEN_HDR_DATE,
    HTTP_GEN_HDR_PRAGMA,
    HTTP_GEN_HDR_TRAILER,
    HTTP_GEN_HDR_TRANSFER_ENCODING,
    HTTP_GEN_HDR_UPGRAGE,
    HTTP_GEN_HDR_VIA,
    HTTP_GEN_HDR_WARNING,
    HTTP_GEN_HDR_COOKIE,
    HTTP_GEN_HDR_SET_COOKIE,
    HTTP_GEN_HDR_SET_COOKIE2,

    /*http请求头，客户端发过来的*/
    HTTP_REQ_HDR_ACCEPT,
    HTTP_REQ_HDR_ACCEPT_CHARSET,
    HTTP_REQ_HDR_ACCEPT_LANGUAGE,
    HTTP_REQ_HDR_ACCEPT_ENCODING,
    HTTP_REQ_HDR_AUTHORIZATION,
    HTTP_REQ_HDR_EXPECT,
    HTTP_REQ_HDR_FROM,
    HTTP_REQ_HDR_HOST,
    HTTP_REQ_HDR_IF_MATCH,
    HTTP_REQ_HDR_IF_MODIFIED_SINCE,
    HTTP_REQ_HDR_IF_NONE_MATCH,
    HTTP_REQ_HDR_IF_RANGE,
    HTTP_REQ_HDR_IF_UNMODIFIED_SICE,
    HTTP_REQ_HDR_MAX_FORWARDS,
    HTTP_REQ_HDR_PROXY_AUTHORIZATION,
    HTTP_REQ_HDR_RANGE,
    HTTP_REQ_HDR_REFERER,
    HTTP_REQ_HDR_TE,
    HTTP_REQ_HDR_USER_AGENT,

    /*http响应头，服务器回的*/
    HTTP_RESP_HDR_ACCEPT_RANGES,
    HTTP_RESP_HDR_AGE,
    HTTP_RESP_HDR_ETAG,
    HTTP_RESP_HDR_LOCATION,
    HTTP_RESP_HDR_PROXY_AUTHENTICATE,
    HTTP_RESP_HDR_RETRY_AFTER,
    HTTP_RESP_HDR_SERVER,
    HTTP_RESP_HDR_VARY,
    HTTP_RESP_HDR_WWW_AUTHENTICATE,

    /*http实体数据说明头*/
    
    HTTP_ENTITY_HDR_ALLOW,
    HTTP_ENTITY_HDR_CONTENT_ENCODING,
    HTTP_ENTITY_HDR_CONTENT_LANGUAGE,
    HTTP_ENTITY_HDR_CONTENT_LENGTH,
    HTTP_ENTITY_HDR_CONTENT_LOCATION,
    HTTP_ENTITY_HDR_MD5,
    HTTP_ENTITY_HDR_CONTENT_RANGE,
    HTTP_ENTITY_HDR_CONTENT_TYPE,
    HTTP_ENTITY_HDR_EXPIRES,
    HTTP_ENTITY_HDR_LAST_MODIFIED,
    HTTP_ENTITY_HDR_X_SESSIONCOOKIE,

    HTTP_METHOD,
    HTTP_STATE,
    HTTP_BOUNDARY,
    HTTP_INDEX_MAX,

    
    /*http 方法*/
   /*   HTTP_METHOD_OPTIONS,
        HTTP_METHOD_GET,
        HTTP_METHOD_HEAD,
        HTTP_METHOD_POST,
        HTTP_METHOD_PUT,
        HTTP_METHOD_DELETE,
        HTTP_METHOD_TRACE,
        HTTP_METHOD_CONNECT,
    */
    
    /*这个永远是最后面的那个，用于说明此enum的个数*/

   
    
};

typedef enum HTTP_HDR_INDEX_ HTTP_HDR_INDEX_E;

#define HTTP_INIT_STATE     0x00    //还没有读取http的任何数据
#define HTTP_HEAD_STATE     0x01    //已经读取 http 第一行的数据，正在解析http head 数据
#define HTTP_BODY_STATE     0x02    //http head 数据已经解析完毕，其他的数据为http body数据
#define HTTP_END_STATE      0x03    //http已经处理完本次所有数据


#define HTTP_TYPE_INVAILD   -1
#define HTTP_TPYE_INT       1
#define HTTP_TPYE_STRING    2
#define HTTP_TPYE_BYTE      3
#define HTTP_TPYE_DOUBLE    4
#define HTTP_TPYE_SHORT     5

#define HTTP_CONNECTION_NONE_STATE    0
#define HTTP_CONNECTION_CLOSE_STATE   1
#define HTTP_CONNECTION_KEEPLIVE_STATE   2
struct http_file
{
    int fd;
    char *name;
    OS_STAT_t stat;
    OS_UINT64_t start_l;
    OS_UINT64_t end_l;
    OS_UINT64_t curr_l;
};

struct http_field_attr
{
    char *name;
    char *value;
    int value_type;
    struct http_field_attr *next_slib;
    struct http_field_attr *child;
    struct http_field_attr *parent;
};
typedef struct http_field_attr http_field_attr_t;

struct http_field
{
   char *name;          //字段名称实际由索引已经确定
   char *pvalue;         //字段的值，表示field后的值
   int  len;            // -1 表示此field 没有被使用，否则表示此field 后面的value的长度                    
   struct http_field_attr *field_attr;
};

typedef struct http_field http_field_t;

struct http_field_map
{
    struct http_field http_root[HTTP_INDEX_MAX];
};
typedef struct http_field_map http_field_map_t;


#define HTTP_FUNC_TYPE_PARSER_HDR           0x01
#define HTTP_FUNC_TYPE_PARSER_BODY          0x02
#define HTTP_FUNC_TYPE_PARSER_ERR           0x03

struct http_context;

typedef int (*http_parser_func)(struct http_context *);
typedef int (*http_set_parser_func)(struct http_context*,http_parser_func pfunc,int func_type);
#define MAX_LINE_LEN    1024

struct http_context
{
  	OS_SOCKET_t  fd;
	int errcode;					// 解析中间如果出现错误，则在解析后可以获取该错误号的
	int guid;                        //dugang creat
	struct sockaddr_in addr;        //dugang creat
    int ver;                        //http处理的版本号： 1: http 1.0 2,http 1.1
    int state;                      //http处理器处理数据的状态
    int err_code;                   //解析中间出现的错误
    int cgi_mode;                       //时候是使用cgi模式处理，暂不支持
    http_parser_func parser_hdr;        //解析header部分完毕回调函数
    http_parser_func parser_body;       //解析body 部分完毕回调函数
    http_parser_func parser_err;        //解析中间入到错误时调用
    http_set_parser_func set_parser;        //设置解析回调函数
    data_buff_t *recv_buff_p;   //用于接收到的数据的缓存
    data_buff_t *send_buff_p;   //发送的数据的缓存
    http_field_map_t field_map; //用于接收到的http解析数据
    http_field_map_t field_map_write; //用于发送http head 的数据
//    http_field_t cookie;              //保存cookie的值
//    http_field_t boundary;            //用于保存 boundary 的值
    http_field_map_t filed_map_save;  //用于保存 cookie，boundary 等需要回送的数据 
    char line[MAX_LINE_LEN];        //用于缓存每一行获取的数据，最大应该不会超过1024个字节
    const char *line_p;
    int line_len;
    char left[MAX_LINE_LEN];         //分割符左边的数据缓冲
    const char *left_p;
    int left_len;
    char right[MAX_LINE_LEN];        //分隔符右边的数据缓冲
    const char *right_p;
    int right_len;
    char *body;                     //指向body,发送或接收的body都是由这个指定的
    char body_content[MAX_HTTP_CONTENT_LEN];
	int body_content_len;
    int body_len;                   //body长度
    struct http_file  *file;        // 要发送的文件
    
};
typedef struct http_context http_context_t;

struct hdr_map;
typedef int (*map_func)(struct hdr_map *,http_context_t *);
struct hdr_map
{
    const char *context;
    int len;
    int index;
    map_func parser;
};

http_context_t *http_alloc_context();
void http_free_context(http_context_t *http_context);
data_buff_t *http_alloc_buff();
void http_free_buff(data_buff_t *buff);
void http_init_context(int http_conn_max,int buff_size);
void http_destroy_context();

int http_read_tcp(http_context_t *http_context);
int http_write_tcp(http_context_t *http_context);
int http_read_udp(http_context_t *http_context);
int http_write_udp(http_context_t *http_context);
int http_timeout(http_context_t *http_context);
void http_set_parser(http_context_t *http_context,http_parser_func pfunc,int type);
int http_make_request_header(http_context_t *http_context);
int http_make_response_header(http_context_t *http_context);
int http_make_body(http_context_t *http_context,const char *body,int len);

struct http_field_attr *http_get_field_child(struct http_field_attr *parent);
struct http_field_attr *http_get_field_next(struct http_field_attr *field_attr);
void http_print_field_attr(struct http_field_attr *root);
void http_print_field_map(http_field_map_t *http_field_map);
void http_enable_debug();
void http_disable_debug();
//以下函数用于测试才翻到这里的
int http_parser_content(http_context_t *http_context);

void http_init_data_buff(data_buff_t * data_buff_p);
const char * http_get_cmd_path(http_context_t *http_context);
const char * http_get_method(http_context_t *http_context);
const char *http_get_boundary(http_context_t *http_context);
int http_get_status_code(http_context_t *http_context);
const char * http_get_content_type(http_context_t *http_context);
const char *http_get_x_sessioncookie(http_context_t *http_context);
const char *http_get_accept(http_context_t *http_context);
const char *http_get_host(http_context_t *http_context);
void monitor_http_buff();
struct http_field *http_get_field_value_by_name(http_field_map_t *http_field_map,char *field);
struct http_field *http_get_field_value_by_index(http_field_map_t *http_field_map,HTTP_HDR_INDEX_E index);
int http_parser_content_ex(http_context_t *http_context);


#ifdef __cplusplus
}
#endif
/*
Content-type 文件类型列表
    'ez'        => 'application/andrew-inset',
    'hqx'        => 'application/mac-binhex40',
    'cpt'        => 'application/mac-compactpro',
    'doc'        => 'application/msword',
    'bin'        => 'application/octet-stream',
    'dms'        => 'application/octet-stream',
    'lha'        => 'application/octet-stream',
    'lzh'        => 'application/octet-stream',
    'exe'        => 'application/octet-stream',
    'class'        => 'application/octet-stream',
    'so'        => 'application/octet-stream',
    'dll'        => 'application/octet-stream',
    'oda'        => 'application/oda',
    'pdf'        => 'application/pdf',
    'ai'        => 'application/postscript',
    'eps'        => 'application/postscript',
    'ps'        => 'application/postscript',
    'smi'        => 'application/smil',
    'smil'        => 'application/smil',
    'mif'        => 'application/vnd.mif',
    'xls'        => 'application/vnd.ms-excel',
    'ppt'        => 'application/vnd.ms-powerpoint',
    'wbxml'        => 'application/vnd.wap.wbxml',
    'wmlc'        => 'application/vnd.wap.wmlc',
    'wmlsc'        => 'application/vnd.wap.wmlscriptc',
    'bcpio'        => 'application/x-bcpio',
    'vcd'        => 'application/x-cdlink',
    'pgn'        => 'application/x-chess-pgn',
    'cpio'        => 'application/x-cpio',
    'csh'        => 'application/x-csh',
    'dcr'        => 'application/x-director',
    'dir'        => 'application/x-director',
    'dxr'        => 'application/x-director',
    'dvi'        => 'application/x-dvi',
    'spl'        => 'application/x-futuresplash',
    'gtar'        => 'application/x-gtar',
    'hdf'        => 'application/x-hdf',
    'js'        => 'application/x-javascript',
    'skp'        => 'application/x-koan',
    'skd'        => 'application/x-koan',
    'skt'        => 'application/x-koan',
    'skm'        => 'application/x-koan',
    'latex'        => 'application/x-latex',
    'nc'        => 'application/x-netcdf',
    'cdf'        => 'application/x-netcdf',
    'sh'        => 'application/x-sh',
    'shar'        => 'application/x-shar',
    'swf'        => 'application/x-shockwave-flash',
    'sit'        => 'application/x-stuffit',
    'sv4cpio'    => 'application/x-sv4cpio',
    'sv4crc'    => 'application/x-sv4crc',
    'tar'        => 'application/x-tar',
    'tcl'        => 'application/x-tcl',
    'tex'        => 'application/x-tex',
    'texinfo'    => 'application/x-texinfo',
    'texi'        => 'application/x-texinfo',
    't'            => 'application/x-troff',
    'tr'        => 'application/x-troff',
    'roff'        => 'application/x-troff',
    'man'        => 'application/x-troff-man',
    'me'        => 'application/x-troff-me',
    'ms'        => 'application/x-troff-ms',
    'ustar'        => 'application/x-ustar',
    'src'        => 'application/x-wais-source',
    'xhtml'        => 'application/xhtml+xml',
    'xht'        => 'application/xhtml+xml',
    'zip'        => 'application/zip',
    'au'        => 'audio/basic',
    'snd'        => 'audio/basic',
    'mid'        => 'audio/midi',
    'midi'        => 'audio/midi',
    'kar'        => 'audio/midi',
    'mpga'        => 'audio/mpeg',
    'mp2'        => 'audio/mpeg',
    'mp3'        => 'audio/mpeg',
    'aif'        => 'audio/x-aiff',
    'aiff'        => 'audio/x-aiff',
    'aifc'        => 'audio/x-aiff',
    'm3u'        => 'audio/x-mpegurl',
    'ram'        => 'audio/x-pn-realaudio',
    'rm'        => 'audio/x-pn-realaudio',
    'rpm'        => 'audio/x-pn-realaudio-plugin',
    'ra'        => 'audio/x-realaudio',
    'wav'        => 'audio/x-wav',
    'pdb'        => 'chemical/x-pdb',
    'xyz'        => 'chemical/x-xyz',
    'bmp'        => 'image/bmp',
    'gif'        => 'image/gif',
    'ief'        => 'image/ief',
    'jpeg'        => 'image/jpeg',
    'jpg'        => 'image/jpeg',
    'jpe'        => 'image/jpeg',
    'png'        => 'image/png',
    'tiff'        => 'image/tiff',
    'tif'        => 'image/tiff',
    'djvu'        => 'image/vnd.djvu',
    'djv'        => 'image/vnd.djvu',
    'wbmp'        => 'image/vnd.wap.wbmp',
    'ras'        => 'image/x-cmu-raster',
    'pnm'        => 'image/x-portable-anymap',
    'pbm'        => 'image/x-portable-bitmap',
    'pgm'        => 'image/x-portable-graymap',
    'ppm'        => 'image/x-portable-pixmap',
    'rgb'        => 'image/x-rgb',
    'xbm'        => 'image/x-xbitmap',
    'xpm'        => 'image/x-xpixmap',
    'xwd'        => 'image/x-xwindowdump',
    'igs'        => 'model/iges',
    'iges'        => 'model/iges',
    'msh'        => 'model/mesh',
    'mesh'        => 'model/mesh',
    'silo'        => 'model/mesh',
    'wrl'        => 'model/vrml',
    'vrml'        => 'model/vrml',
    'css'        => 'text/css',
    'html'        => 'text/html',
    'htm'        => 'text/html',
    'asc'        => 'text/plain',
    'txt'        => 'text/plain',
    'rtx'        => 'text/richtext',
    'rtf'        => 'text/rtf',
    'sgml'        => 'text/sgml',
    'sgm'        => 'text/sgml',
    'tsv'        => 'text/tab-separated-values',
    'wml'        => 'text/vnd.wap.wml',
    'wmls'        => 'text/vnd.wap.wmlscript',
    'etx'        => 'text/x-setext',
    'xsl'        => 'text/xml',
    'xml'        => 'text/xml',
    'mpeg'        => 'video/mpeg',
    'mpg'        => 'video/mpeg',
    'mpe'        => 'video/mpeg',
    'qt'        => 'video/quicktime',
    'mov'        => 'video/quicktime',
    'mxu'        => 'video/vnd.mpegurl',
    'avi'        => 'video/x-msvideo',
    'movie'        => 'video/x-sgi-movie',
    'ice'        => 'x-conference/x-cooltalk', 
*/

#endif

