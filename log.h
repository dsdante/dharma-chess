#ifndef LOG_H
#define LOG_H

extern int logging_level;

void break_debugger();

void log_emerg(const char *format, ...);
void log_alert(const char *format, ...);
void log_crit(const char *format, ...);
void log_err(const char *format, ...);
void log_warning(const char *format, ...);
void log_notice(const char *format, ...);
void log_info(const char *format, ...);
void log_debug(const char *format, ...);

#endif // LOG_H
