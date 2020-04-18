#ifdef OS_THREAD_C
#error "thread.c included more than once"
#endif
#define OS_THREAD_C

// #include "win32.h"
#include "../thread.h"

struct start_info {
    void *param;
    thread_start_t start;
};

static bool create_start_info(struct start_info **outinfo, thread_start_t start, void *param)
{
    struct start_info *info = (struct start_info* )malloc(sizeof(*info));
    if (!info)
        return false;
    info->param = param;
    info->start = start;
    *outinfo = info;
    return true;
}

static DWORD WINAPI thread_entry(LPVOID lpParam)
{
    struct start_info *info = (struct start_info* )lpParam;
    void *param = info->param;
    thread_start_t start = info->start;
    free(info);
    int retval = start(param);
    return (DWORD)retval;
}

int thread_create(thread_t *thread, thread_start_t start, void *arg)
{
    struct start_info *info;
    if (!create_start_info(&info, start, arg)) {
        // @Enhancement:
        // We should probably return STATUS_NO_MEMORY or whatever equivalent there is.
        return 1;
    }
    thread->handle = CreateThread(NULL, 0, thread_entry, info, 0, 0);
    int error = 0;
    if (!thread->handle)
        error = GetLastError();
    return error;
}

void thread_exit(int retval)
{
    ExitThread((DWORD)retval);
}

thread_t thread_self(void)
{
    thread_t thread;
    thread.handle = GetCurrentThread();
    return thread;
}

int thread_detach(thread_t thread)
{
    CloseHandle(thread.handle);
    return 0;
}

int thread_join(thread_t thread, int *retval)
{
    DWORD reason = WaitForSingleObject(thread.handle, INFINITE);
    if (reason != WAIT_OBJECT_0)
        return GetLastError();
    if (retval) {
        DWORD rv;
        GetExitCodeThread(thread.handle, &rv);
        *retval = (int)rv;
    }
    return 0;
}

int thread_sleep(thread_t thread, const struct timespec *ts)
{
    DWORD sleep_time_ms;
    sleep_time_ms =  (DWORD)ts->tv_sec * 1000;
    sleep_time_ms += ts->tv_nsec / 1000000;
    Sleep(sleep_time_ms);
    return 0;
}

void thread_yield(void)
{
    SwitchToThread();
}

union mutex_impl {
    thread_mutex_t      mtx;
    CRITICAL_SECTION    sec;
};
static_assert(sizeof(CRITICAL_SECTION) <= sizeof(thread_mutex_t), "thread_mutex implementation too small");

int thread_mutex_init(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    InitializeCriticalSection(&mtx->sec);
    return 0;
}

int thread_mutex_destroy(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    DeleteCriticalSection(&mtx->sec);
    return 0;
}

int thread_mutex_lock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    EnterCriticalSection(&mtx->sec);
    return 0;
}

int thread_mutex_trylock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    BOOL could_enter = TryEnterCriticalSection(&mtx->sec);
    return !could_enter;
}

int thread_mutex_unlock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    LeaveCriticalSection(&mtx->sec);
    return 0;
}

int thread_event_init(thread_event_t *event)
{
    int error = 0;
    event->handle = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!event->handle)
        error = GetLastError();
    return error;
}

int thread_event_destroy(thread_event_t *event)
{
    CloseHandle(event->handle);
    return 0;
}

int thread_event_signal(thread_event_t *event)
{
    int error = 0;
    if (SetEvent(event->handle) != TRUE)
        error = GetLastError();
    return error;
}

int thread_event_reset(thread_event_t *event)
{
    ResetEvent(event->handle);
    return 0;
}

int thread_event_wait(thread_event_t *event)
{
    int error = 0;
    if (WaitForSingleObject(event->handle, INFINITE) == WAIT_FAILED)
        error = GetLastError();
    return error;
}

int thread_event_timedwait(thread_event_t *event, uint32_t ms)
{
    int error = 0;
    if (WaitForSingleObject(event->handle, ms) == WAIT_FAILED)
        error = GetLastError();
    return error;
}
