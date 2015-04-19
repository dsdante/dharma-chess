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
int logging_level = 5;
#endif

void log_print(int level, const char *format, ...)
{
    char message[2048];

    time_t timestamp;
    time(&timestamp);
    struct tm time = *localtime(&timestamp);
    strftime(message, 2048, "%F %T %Z  ", &time);

    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 1024, format, args);
    va_end(args);
    strcat(message, buffer);

    if (level <= logging_level)
        puts(message);

    FILE *f;
    f = fopen(log_filename, "a");
    fprintf(f, "%s\n", message);
    fclose(f);
}
