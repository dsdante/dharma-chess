#ifndef LOG_H
#define LOG_H

extern int logging_level;

void log_print(int level, const char *format, ...);

#endif // LOG_H
