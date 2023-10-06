#ifndef __MISC_COMM_H
#define __MISC_COMM_H


#ifdef __cplusplus
extern "C" {
#endif


	/*忽略前面的空白字符*/
	void skip_spaces(const char **pp);

	int get_word_sep(char *psz_buf,int *buf_size,char sep,const char **pp,int str_len);
	int get_word_sep_ex(char *psz_buf,int *buf_size,const char *sep,const char **pp,int str_len);
	/*提取空白字符前面的字符串*/
	int get_word(char *psz_buf,int *buf_size,const char **pp,int str_len);
	/* 获取某个分隔符两边的字符串
	*/
	int get_assign(char *p_left,int *left_size,char *p_right,int *right_size,char sep,const char * psz_string,int str_len);
	int get_assign_ex(char *p_left,int *left_size,char *p_right,int *right_size,const char *sep,const char * psz_string,int str_len);

	/************************************************************************/
	/*  以下函数使用指针的方式 ,返回时候找到以及长度值，                   */
	/*  注意返回的字符串没有加 '\0' 需要自己增加 '\0'                      */
	/************************************************************************/
	int get_word_sep_p(char **psz_buf,int *buf_size,char sep,const char **pp,int str_len);
	int get_word_sep_ex_p(char **psz_buf,int *buf_size,const char *sep,const char **pp,int str_len);
	int get_word_p(char **psz_buf,int *buf_size,const char **pp,int str_len);
	int get_line_p(char **line ,int *line_len,const char **pp,int str_len);
	int get_assign_p(char **p_left,int *left_size,char **p_right,int *right_size,char sep,const char * psz_string,int str_len);
	int get_assign_ex_p(char **p_left,int *left_size,char **p_right,int *right_size,const char *sep,const char * psz_string,int str_len);
	/*
	* 返回行长度，不包括 '\r\n'
	*/
	int get_line(char *line ,int *line_len,const char **pp,int str_len);
	int decode_uri(const char *in,int length,char *out,int *max);
	int encode_uri_path(const char *in,int length,char *out,int *max);
	int encode_uri_query(const char *in,int length,char *out,int *max);

	unsigned char* decode_base64(char* in, unsigned int  *resultSize,int trimTrailingZeros) ;
	char* encode_base64(char const* origSigned, unsigned origLength)  ;


#define SOCKET_SERVER_TYPE  0x01            /*用于创建服务器端socket*/
#define SOCKET_CLIENT_TYPE  0x02            /*用于创建客户端socket*/
OS_SOCKET_t create_tcp_socket(int port,int type);
OS_SOCKET_t create_udp_socket(int port,int type);
OS_SOCKET_t create_multi_socket(int port,const char *ip_addr);

#ifdef __cplusplus
}
#endif

#endif
