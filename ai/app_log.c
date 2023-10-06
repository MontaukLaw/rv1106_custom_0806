#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

// const char *filename = "/userdata/rv1106_app.log";
const char *log_filename = "/userdata/rv1106_app.log";

void write_log(char *format, ...)
{
    FILE *log_file = fopen(log_filename, "a");
    if (log_file == NULL)
    {
        perror("Error opening log file");
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, format);

    time_t current_time;
    struct tm *time_info;
    char time_buffer[20];

    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(log_file, "[%s] ", time_buffer);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");

    va_end(args);

    fclose(log_file);
}