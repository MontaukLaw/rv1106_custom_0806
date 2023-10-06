#ifndef FILE_OPERA_H__
#define FILE_OPERA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

int read_file(char * filename, unsigned char* buff, int maxlen);  // for binary file
int write_file_bin(char * filename, unsigned char* buff, int len);   // for binary file 
int write_file(char * filename, char* buff); // for text file

bool is_exist_dir(char * path);

// 从全路径里 ，取得文件名。
char *find_file_name( char *name);

// 从文件名 取得 base 字符串，一般配合find_file_name 使用。
char * get_fn_base(char* fn);


#ifdef __cplusplus
}
#endif


#endif