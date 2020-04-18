#ifdef OS_THREAD_C
#error "thread.c included more than once"
#endif
#define OS_THREAD_C

#include "../thread.h"

struct start_info {
    void *param;
    thread_start_t start;
};

static bool create_start_info(struct start_info **outinfo, thread_start_t start, void *param)
{
    struct start_info *info = (struct start_info*)malloc(sizeof(*info));
    if (!info)
        return false;
    info->param = param;
    info->start = start;
    *outinfo = info;
    return true;
}

static void *thread_entry(void *param)
{
    struct start_info *info = (struct start_info*)param;
    param = info->param;
    thread_start_t start = info->start;
    free(info);
    int retval = start(param);
    return (void *)((intptr_t)retval);
}

int thread_create(thread_t *thread, thread_start_t start, void *param)
{
    pthread_t handle;
    pthread_attr_t attr;

    struct start_info *info;
    if (!create_start_info(&info, start, param)) {
        return 1;
    }
    if (pthread_attr_init(&attr) != 0) {
        // @Enhancement: log?
        return 1;
    }
    if (pthread_create(&handle, &attr, thread_entry, info) != 0) {
        // @Enhancement: log?
        return 1;
    }

    thread->handle = handle;
    pthread_attr_destroy(&attr);
    return 0;
}

void thread_exit(int retval)
{
    pthread_exit((void *)((intptr_t)retval));
}

thread_t thread_self(void)
{
    thread_t thread;
    thread.handle = pthread_self();
    return thread;
}

int thread_detach(thread_t thread)
{
    pthread_t handle = thread.handle;
    int retval = pthread_detach(handle);
    return retval;
}

int thread_join(thread_t thread, int *retval)
{
    // @Cleanup:
    // It would seem that on linux, pthread_join will release the
    // ressources, if the thread is joined.
    // Need to confirm.
    void *rv;
    pthread_t handle = thread.handle;
    int error = pthread_join(handle, &rv);
    if (error == 0)
        *retval = (int)((intptr_t)rv);
    return error;
}

int thread_sleep(thread_t thread, const struct timespec *ts)
{
    pthread_t handle = thread.handle;
    struct timespec rem;
    int error = nanosleep(ts, &rem);
    // @Cleanup:
    // Deal with the error + logging?
    return error;
}

void thread_yield(void)
{
}

union mutex_impl {
    thread_mutex_t  mtx;
    pthread_mutex_t pmtx;
};
static_assert(sizeof(pthread_mutex_t) <= sizeof(thread_mutex_t), "thread_mutex implementation too small");

int thread_mutex_init(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    pthread_mutexattr_t mutexattr;
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);

    int error = pthread_mutex_init(&mtx->pmtx, &mutexattr);
    pthread_mutexattr_destroy(&mutexattr);
    return error;
}

int thread_mutex_destroy(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    return pthread_mutex_destroy(&mtx->pmtx);
}

int thread_mutex_lock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    return pthread_mutex_lock(&mtx->pmtx);
}

int thread_mutex_trylock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    return pthread_mutex_trylock(&mtx->pmtx);
}

int thread_mutex_unlock(thread_mutex_t *mutex)
{
    union mutex_impl *mtx = (union mutex_impl *)mutex;
    return pthread_mutex_unlock(&mtx->pmtx);
}
