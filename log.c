#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

const char log_filename[] = "dchess.log";

#ifdef NDEBUG
int logging_level = 3;
#else
int logging_level = 6;
#endif

void log_print(int level, char *format, va_list args)
{
    char message[2048];

    time_t timestamp;
    time(&timestamp);
    struct tm time = *localtime(&timestamp);
    strftime(message, 2048, "%F %T %Z  ", &time);

    char buffer[1536];
    vsnprintf(buffer, 1536, format, args);
    strcat(message, buffer);

    if (level <= logging_level)
        puts(message);

    FILE *f;
    f = fopen(log_filename, "a");
    fprintf(f, "%s\n", message);
    fclose(f);
}

void log_emerg(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(0, format, args);
    va_end(args);
}

void log_alert(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(1, format, args);
    va_end(args);
}

void log_crit(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(2, format, args);
    va_end(args);
}

void log_err(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(3, format, args);
    va_end(args);
}

void log_warning(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(4, format, args);
    va_end(args);
}

void log_notice(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(5, format, args);
    va_end(args);
}

void log_info(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(6, format, args);
    va_end(args);
}

void log_debug(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log_print(7, format, args);
    va_end(args);
}
