#ifndef LOG_H
#define LOG_H

#define ANSI_COLOR_RED          "\x1b[91m"
#define ANSI_COLOR_GREEN        "\x1b[92m"
#define ANSI_COLOR_YELLOW       "\x1b[93m"
#define ANSI_COLOR_BLUE         "\x1b[94m"
#define ANSI_COLOR_MAGENTA      "\x1b[95m"
#define ANSI_COLOR_CYAN         "\x1b[96m"
#define ANSI_COLOR_WHITE        "\x1b[97m"
#define ANSI_COLOR_DARK_RED     "\x1b[31m"
#define ANSI_COLOR_DARK_GREEN   "\x1b[32m"
#define ANSI_COLOR_DARK_YELLOW  "\x1b[33m"
#define ANSI_COLOR_DARK_BLUE    "\x1b[34m"
#define ANSI_COLOR_DARK_MAGENTA "\x1b[35m"
#define ANSI_COLOR_DARK_CYAN    "\x1b[36m"
#define ANSI_COLOR_GREY         "\x1b[37m"
#define ANSI_COLOR_DEFAULT      "\x1b[39m"

extern int logging_level;

void log_emerg(const char *format, ...);
void log_alert(const char *format, ...);
void log_crit(const char *format, ...);
void log_err(const char *format, ...);
void log_warning(const char *format, ...);
void log_notice(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);

#endif // LOG_H
