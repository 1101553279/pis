#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define ECHO_COLOR_NONE         "\033[0;0m"
#define ECHO_COLOR_GREEN        "\033[0;32m"

#define dbg_out(fmt, args...)     \
        do{ printf(ECHO_COLOR_GREEN"##: " fmt "(file: %s, func: %s, line: %d)\n"ECHO_COLOR_NONE, ##args, __FILE__, __func__, __LINE__); }while( 0 )

#define out(fmt, args...)       \
        do { printf(fmt, ##args ); }while( 0 )


/* function debug function implementation */
#if 0
#include <stdio.h>
#include <stdarg.h>

#define ECHO_COLOR_NONE         "\033[0;0m"
#define ECHO_COLOR_GREEN        "\033[0;32m"

#define debug_print(message, ...)   debug_msg(message, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

void debug_type(char *fmt, char *file, const char *func, int line, va_list argp)
{
     char buffer[128] = { 0 };
 
     vsnprintf(buffer, sizeof(buffer), fmt, argp);
     printf(ECHO_COLOR_GREEN"Debug: %s(file: %s, func: %s, line: %d)\n"ECHO_COLOR_NONE, buffer, file, func, line);
}

void debug_msg(char *fmt, char *file, const char *func, int line, ...)
{
     va_list arg_list;
     va_start(arg_list, line);
     debug_type(fmt, file, func, line, arg_list);
     va_end(arg_list);
}
#endif

#endif
