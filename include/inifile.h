#ifndef _INI_H_12345678910
#define _INI_H_12345678910

#ifdef LINUX /* Remove CR, on unix systems. */
#include <ctype.h>
#define INI_REMOVE_CR
#define DONT_HAVE_STRUPR
#endif

#ifndef CCHR_H
#define CCHR_H
typedef const char cchr;
#endif

#ifndef __cplusplus

typedef int bool;
#define true  1
#define TRUE  1
#define false 0
#define FALSE 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define tpNULL       0
#define tpSECTION    1
#define tpKEYVALUE   2
#define tpCOMMENT    3

struct ENTRY
{
	char   Type;
	char  *Text;
	struct ENTRY *pPrev;
	struct ENTRY *pNext;
};

typedef struct
{
	struct ENTRY *pSec;
	struct ENTRY *pKey;
	char          KeyText [128];
	char          ValText [128];
	char          Comment [255];
} EFIND;

typedef struct  
{
	struct ENTRY *pEntry;
	struct ENTRY *pCurEntry;
	FILE *iniFile;
	char result[255];
}INIFILE_t;



/* Macros */
#define ArePtrValid(Sec,Key,Val) ((Sec!=NULL)&&(Key!=NULL)&&(Val!=NULL))
#define INIT_INIFILE(x)		do{(x).pEntry = NULL;(x).pCurEntry = NULL;(x).iniFile = NULL;}while(0)

/* Connectors of this file (Prototypes) */
//这个函数仅用于读取数据
bool    OpenIniFile (INIFILE_t* pIniFile,cchr *FileName);

bool    ReadBool    (INIFILE_t* pIniFile,cchr *Section, cchr *Key, bool   Default);
int     ReadInt     (INIFILE_t* pIniFile,cchr *Section, cchr *Key, int    Default);
double  ReadDouble  (INIFILE_t* pIniFile,cchr *Section, cchr *Key, double Default);
cchr   *ReadString  (INIFILE_t* pIniFile,cchr *Section, cchr *Key, cchr  *Default);

void    WriteBool   (INIFILE_t* pIniFile,cchr *Section, cchr *Key, bool   Value);
void    WriteInt    (INIFILE_t* pIniFile,cchr *Section, cchr *Key, int    Value);
void    WriteDouble (INIFILE_t* pIniFile,cchr *Section, cchr *Key, double Value);
void    WriteString (INIFILE_t* pIniFile,cchr *Section, cchr *Key, cchr  *Value);

bool	DeleteKey (INIFILE_t* pIniFile,cchr *Section, cchr *Key);

bool    WriteIniFile (INIFILE_t* pIniFile,cchr *FileName);
void    CloseIniFile (INIFILE_t* pIniFile);

void WriteLongLongInt(INIFILE_t* pIniFile,cchr *Section, cchr *Key, long long   Value);
long long ReadLongLongInt(INIFILE_t* pIniFile,cchr * Section, cchr * pKey, long long  Default);
char *__lltoa(char *pBuf,int iLen,long long lVal);

#ifdef __cplusplus
}
#endif
#endif
