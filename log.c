#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

const char log_filename[] = "dchess.log";

const char *log_color[] = {
    ANSI_COLOR_RED,
    ANSI_COLOR_RED,
    ANSI_COLOR_RED,
    ANSI_COLOR_DARK_RED,
    ANSI_COLOR_DARK_YELLOW,
    ANSI_COLOR_DARK_GREEN,
    ANSI_COLOR_DARK_GREEN,
    ANSI_COLOR_DARK_GREEN,
};

#ifdef NDEBUG
int logging_level = 3;
#else
int logging_level = 7;
#endif

void log_print(int level, const char *format, va_list args)
{
    char message[2048];

    time_t timestamp;
    time(&timestamp);
    struct tm time = *localtime(&timestamp);
    strftime(message, 2048, "%F %T %Z  ", &time);

    char buffer[1536];
    vsnprintf(buffer, 1536, format, args);
    strcat(message, buffer);
    strcat(message, "\n");

    if (level <= logging_level)
        fprintf(stderr, "%s%s%s", log_color[level], message, ANSI_COLOR_DEFAULT);

    FILE *f;
    f = fopen(log_filename, "a");
    fputs(message, f);
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
