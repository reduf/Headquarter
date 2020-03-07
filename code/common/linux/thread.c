#ifdef OS_THREAD_C
#error "thread.c included more than once"
#endif
#define OS_THREAD_C

#include "../thread.h"

static void *start_routine(void *param)
{
    struct thread *thread = param;
    int retval = thread->start(thread, thread->param);
    thread->retval = retval;
    return thread;
}

int thread_create(struct thread *thread, thread_start_t start, void *param)
{
    int error;
    pthread_t handle;
    pthread_attr_t attr;

    thread->start = start;
    thread->param = param;
    thread->retval = 0;

    error = pthread_attr_init(&attr);
    if (error != 0) {
        // @Enhancement: log?
        return error;
    }
    
    error = pthread_create(&handle, &attr, start_routine, thread);
    if (error != 0) {
        // @Enhancement: log?
        return error;
    }

    thread->handle = handle;
    pthread_attr_destroy(&attr);
    return 0;
}

_Noreturn void thread_exit(void)
{
    pthread_exit(NULL);
}

int thread_detach(struct thread *thread)
{
    pthread_t handle = thread->handle;
    int retval = pthread_detach(handle);
    thread->handle = 0;
    return retval;
}

int thread_join(struct thread *thread, int *retval)
{
    // @Cleanup:
    // It would seem that on linux, pthread_join will release the
    // ressources, if the thread is joined.
    // Need to confirm.
    pthread_t handle = thread->handle;
    int error = pthread_join(handle, NULL);
    if (error == 0 && retval != NULL)
        *retval = thread->retval;
    return error;
}

int thread_sleep(struct thread *thread, const struct timespec *ts)
{
    pthread_t handle = thread->handle;
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
