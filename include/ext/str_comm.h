#ifndef _STR_COMM_H
#define _STR_COMM_H

#ifdef __cplusplus
extern "C" {
#endif


/*����ǰ��Ŀհ��ַ�*/
void skip_spaces(const char **pp);

int get_word_sep(char *psz_buf,int *buf_size,char sep,const char **pp,int str_len);
int get_word_sep_ex(char *psz_buf,int *buf_size,const char *sep,const char **pp,int str_len);
/*��ȡ�հ��ַ�ǰ����ַ���*/
int get_word(char *psz_buf,int *buf_size,const char **pp,int str_len);
/* ��ȡĳ���ָ������ߵ��ַ���
*/
int get_assign(char *p_left,int *left_size,char *p_right,int *right_size,char sep,const char * psz_string,int str_len);
int get_assign_ex(char *p_left,int *left_size,char *p_right,int *right_size,const char *sep,const char * psz_string,int str_len);

/************************************************************************/
/*  ���º���ʹ��ָ��ķ�ʽ ,����ʱ���ҵ��Լ�����ֵ��                   */
/*  ע�ⷵ�ص��ַ���û�м� '\0' ��Ҫ�Լ����� '\0'                      */
/************************************************************************/
int get_word_sep_p(char **psz_buf,int *buf_size,char sep,const char **pp,int str_len);
int get_word_sep_ex_p(char **psz_buf,int *buf_size,const char *sep,const char **pp,int str_len);
int get_word_p(char **psz_buf,int *buf_size,const char **pp,int str_len);
int get_line_p(char **line ,int *line_len,const char **pp,int str_len);
int get_assign_p(char **p_left,int *left_size,char **p_right,int *right_size,char sep,const char * psz_string,int str_len);
int get_assign_ex_p(char **p_left,int *left_size,char **p_right,int *right_size,const char *sep,const char * psz_string,int str_len);
/*
* �����г��ȣ������� '\r\n'
*/
int get_line(char *line ,int *line_len,const char **pp,int str_len);
/*
*  decode_base64:
*  input: in ��Ҫ��������ݣ� trimTrailingZeros���Ƿ�ȥ����β��0�ַ�
* output�� resultSize ������ַ��ĳ���
* return�� �������ַ�
*/
unsigned char* decode_base64(char* in, unsigned int  *resultSize,int  trimTrailingZeros);
/*
*  encode_base64:
*  input: origSigned ��Ҫ��������ݣ� origLength���������ݵĳ���
* return�� �������ַ�
*/
char* encode_base64(char const* origSigned, unsigned int origLength);
int decode_uri(const char *in,int length,char *out,int *max);
int encode_uri_path(const char *in,int length,char *out,int *max);
int encode_uri_query(const char *in,int length,char *out,int *max);

#ifdef __cplusplus
}
#endif


#endif
