#ifndef _UTF8_TO_GB2312_H
#define _UTF8_TO_GB2312_H
#ifdef __cplusplus
extern {
#endif

int utf8_to_gb2312(const unsigned char *in_buf,int in_len,unsigned char *out_buf);

#ifdef  __cplusplus
};
#endif
#endif
