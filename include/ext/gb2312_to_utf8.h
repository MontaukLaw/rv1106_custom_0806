#ifndef _ZTE_GB2312_TO_UTF8_H
#define	_ZTE_GB2312_TO_UTF8_H

#ifdef __cplusplus
extern {
#endif

int gb2312_to_utf8(const unsigned char *input,int input_len,unsigned char *output);

#ifdef	__cplusplus
};
#endif

#endif
