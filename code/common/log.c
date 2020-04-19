#ifdef CORE_LOG_C
#error "log.c included more than once"
#endif
#define CORE_LOG_C

#include "log.h"

#define LOG_MSG_SIZE    (1024)
#define LOG_BUFFER_SIZE (16 * LOG_MSG_SIZE)

static unsigned int     log_print_level;

static thread_event_t   log_event;
static thread_mutex_t   log_mutex;

static size_t           log_wpos = 0;
static char             log_buffer[LOG_BUFFER_SIZE];
static FILE            *log_file;

static const char *
log_print_level_s(unsigned int level)
{
    switch (level) {
        case LOG_FATAL:
            return "Fatal";
        case LOG_ERROR:
            return "Error";
        case LOG_WARN:
            return "Warning";
        case LOG_NOTICE:
            return "Notice";
        case LOG_INFO:
            return "Info";
        case LOG_DEBUG:
            return "Debug";
        default:
            abort();
    }
}

void log_init(void)
{
    int error;
#ifndef _DEBUG
    return;
#endif
#ifdef _NDEBUG
    log_print_level = LOG_INFO;
#else
    log_print_level = LOG_DEBUG;
#endif


    error = thread_mutex_init(&log_mutex);
    if (error) {
        assert(!"log_init: thread_mutex_init");
        return;
    }

    char timestamp[64];
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", ts);

    char file_path[1024];
    int length = dlldir(NULL, file_path, sizeof(file_path));
    snprintf(&file_path[length], sizeof(file_path) - length, "logs/%s_%d.txt", timestamp, getpid());

    log_file = fopen(file_path, "w");
    if (!log_file) {
        printf("Failed to open log find at %s", file_path);
        assert(!"log_init: fopen");
        return;
    }
}

static int log_time(char *buffer, size_t size)
{
    time_t t = time(NULL);
    struct tm *ts = localtime(&t);
    return strftime(buffer, size, "%Y-%m-%d %H:%M:%S", ts);
}

int log_vmsg(unsigned int level, const char *format, va_list ap);
int log_msg(unsigned int level, const char *format, ...)
{
    va_list ap;
    int ret;

    va_start(ap, format);
    ret = log_vmsg(level, format, ap);
    va_end(ap);

    return ret;
}

int log_vmsg(unsigned int level, const char *format, va_list ap)
{
#ifndef _DEBUG
    return 0;
#endif
    int nr_chars;
    char buffer[LOG_MSG_SIZE];

    if (!log_file || level > log_print_level) {
        return 0;
    }

    nr_chars = vsnprintf(buffer, sizeof(buffer), format, ap);
    if ((unsigned int)nr_chars >= sizeof(buffer)) {
        log_msg(level, "log: message too long");
        return 0;
    }

    thread_mutex_lock(&log_mutex);
    {
        char timestamp[64];
        log_time(timestamp, sizeof(timestamp));

        nr_chars = fprintf(stderr, "[%s] %7s: %s\n", timestamp, log_print_level_s(level), buffer);
        fprintf(log_file, "[%s] %7s: %s\n", timestamp, log_print_level_s(level), buffer);
        fflush(log_file);
    }
    thread_mutex_unlock(&log_mutex);

    return nr_chars;
}

int log_info(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = log_vinfo(fmt, ap);
    va_end(ap);
    return ret;
}

int log_debug(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = log_vdebug(fmt, ap);
    va_end(ap);
    return ret;
}

int log_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = log_verror(fmt, ap);
    va_end(ap);
    return ret;
}

int log_notice(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = log_vnotice(fmt, ap);
    va_end(ap);
    return ret;
}

int log_warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = log_vwarning(fmt, ap);
    va_end(ap);
    return ret;
}

int log_vinfo(const char *format, va_list ap)
{
    return log_vmsg(LOG_INFO, format, ap);
}

int log_vdebug(const char *format, va_list ap)
{
    return log_vmsg(LOG_DEBUG, format, ap);
}

int log_verror(const char *format, va_list ap)
{
    return log_vmsg(LOG_ERROR, format, ap);
}

int log_vnotice(const char *format, va_list ap)
{
    return log_vmsg(LOG_NOTICE, format, ap);
}

int log_vwarning(const char *format, va_list ap)
{
    return log_vmsg(LOG_WARN, format, ap);
}
