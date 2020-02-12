#ifndef COMMON_LOG_H
#define COMMON_LOG_H

enum {
    LOG_FATAL,
    LOG_ERROR,
    LOG_WARN,
    LOG_NOTICE,
    LOG_INFO,
    LOG_DEBUG,
    
    LOG_LEVEL_COUNT,
};

void log_init(void);

int log_msg(unsigned int level, const char *format, ...);
int log_vmsg(unsigned int level, const char *format, va_list ap);

int log_info(const char *fmt, ...);
int log_debug(const char *fmt, ...);
int log_error(const char *fmt, ...);
int log_notice(const char *fmt, ...);
int log_warning(const char *fmt, ...);

int log_vinfo(const char *format, va_list ap);
int log_vdebug(const char *format, va_list ap);
int log_verror(const char *format, va_list ap);
int log_vnotice(const char *format, va_list ap);
int log_vwarning(const char *format, va_list ap);

#endif // COMMON_LOG_H
