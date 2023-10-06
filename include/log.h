#ifndef __LOG_H_12345678910
#define __LOG_H_12345678910

#ifdef __cplusplus
extern "C"
{
#endif

#define ERR_MSG 0x01
#define WARN_MSG 0x02
#define INFO_MSG 0x03
#define DEBUG_MSG 0x04
#define LOG_MSG 0x05

	extern void dump_print(FILE *file, const char *file_name, const int line, const char *function_name, const char *fmt, ...);

	void write_log(char *format, ...);

#ifdef _WIN32

#if defined(__MINGW32__)
#define msg_dump(stream, level, args...)                                  \
	do                                                                    \
	{                                                                     \
		if (level <= debug_level)                                         \
		{                                                                 \
			dump_print(stream, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)
#define debug_printf(level, args...)                                      \
	do                                                                    \
	{                                                                     \
		if (level <= debug_level)                                         \
		{                                                                 \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)
#define os_dbg(args...)                                                   \
	do                                                                    \
	{                                                                     \
		if (debug_level > 2)                                              \
		{                                                                 \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)
#else

// VC 6.0 ���ϲ�֧�����ַ�ʽ
#if (_MSC_VER > 1200)
#define msg_dump(stream, level, args, ...)                                             \
	do                                                                                 \
	{                                                                                  \
		if (level <= debug_level)                                                      \
		{                                                                              \
			dump_print(stream, __FILE__, __LINE__, __FUNCTION__, ##args, __VA_ARGS__); \
		}                                                                              \
	} while (0)
#define debug_printf(level, args, ...)                                                 \
	do                                                                                 \
	{                                                                                  \
		if (level <= debug_level)                                                      \
		{                                                                              \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args, __VA_ARGS__); \
		}                                                                              \
	} while (0)
#define os_dbg(args, ...)                                                              \
	do                                                                                 \
	{                                                                                  \
		if (debug_level > 2)                                                           \
		{                                                                              \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args, __VA_ARGS__); \
		}                                                                              \
	} while (0)
#else
#define msg_dump(stream, level, args, ...) \
	{                                      \
	}
#define debug_printf(level, args, ...) \
	{                                  \
	}
#define os_dbg(args, ...) \
	{                     \
	}
#endif
#endif

#else
#define msg_dump(stream, level, args...)                                  \
	do                                                                    \
	{                                                                     \
		if (level <= debug_level)                                         \
		{                                                                 \
			dump_print(stream, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)
#define debug_printf(level, args...)                                      \
	do                                                                    \
	{                                                                     \
		if (level <= debug_level)                                         \
		{                                                                 \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)

#define os_dbg(args...)                                                   \
	do                                                                    \
	{                                                                     \
		if (debug_level > 2)                                              \
		{                                                                 \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
		}                                                                 \
	} while (0)

/*�������ǰ��ɫ*/
// ��ɫ
#define PRINT_FONT_BLA printf("\033[30m");
// ��ɫ
#define PRINT_FONT_RED printf("\033[31m");
#define PRINT_FONT_GRE printf("\033[32m"); // ��ɫ
#define PRINT_FONT_YEL printf("\033[33m"); // ��ɫ
#define PRINT_FONT_BLU printf("\033[34m"); // ��ɫ
#define PRINT_FONT_PUR printf("\033[35m"); // ��ɫ
#define PRINT_FONT_CYA printf("\033[36m"); // ��ɫ
#define PRINT_FONT_WHI printf("\033[37m"); // ��ɫ

#define PRINT_ATTR_DEF printf("\033[0m"); // �����������Ե�ȱʡ����

#define os_dbgerr(args...)                                                \
	do                                                                    \
	{                                                                     \
		if (debug_level > 2)                                              \
		{                                                                 \
			PRINT_FONT_RED                                                \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
			PRINT_ATTR_DEF                                                \
		}                                                                 \
	} while (0)

#define os_dbgwarn(args...)                                               \
	do                                                                    \
	{                                                                     \
		if (debug_level > 2)                                              \
		{                                                                 \
			PRINT_FONT_YEL                                                \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
			PRINT_ATTR_DEF                                                \
		}                                                                 \
	} while (0)

#define os_dbghint(args...)                                               \
	do                                                                    \
	{                                                                     \
		if (debug_level > 2)                                              \
		{                                                                 \
			PRINT_FONT_GRE                                                \
			dump_print(stdout, __FILE__, __LINE__, __FUNCTION__, ##args); \
			PRINT_ATTR_DEF                                                \
		}                                                                 \
	} while (0)

#endif

#ifdef __cplusplus
}
#endif

#endif
