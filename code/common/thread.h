#ifndef COMMON_THREAD_H
#define COMMON_THREAD_H

#include <time.h>

#include "align.h"
#include "noreturn.h"

#if !defined(__clang__) && defined(_MSC_VER)
# define _Thread_local  __declspec(thread)
#endif
#if defined(__GNUC__)
# define _Thread_local  __thread
#endif
#define thread_local _Thread_local

#ifdef _WIN64
# define _mtx_internal_imp_size         40
# define _mtx_internal_imp_alignment    8
#else
# define _mtx_internal_imp_size         24
# define _mtx_internal_imp_alignment    4
#endif

typedef struct thread {
    void *handle;
} thread_t;

typedef int (*thread_start_t)(void *);

int thread_create(thread_t *thread, thread_start_t start, void *arg);
_Noreturn void thread_exit(int retval);
thread_t thread_self(void);

int  thread_detach(thread_t thread);
int  thread_join(thread_t thread, int *retval);
int  thread_sleep(thread_t thread, struct timespec *ts);
void thread_yield(void);

typedef struct thread_mutex {
    alignas(_mtx_internal_imp_alignment)
        unsigned char storage[_mtx_internal_imp_size];
} thread_mutex_t;

int thread_mutex_init(thread_mutex_t *mutex);
int thread_mutex_destroy(thread_mutex_t *mutex);
int thread_mutex_lock(thread_mutex_t *mutex);
int thread_mutex_trylock(thread_mutex_t *mutex);
int thread_mutex_unlock(thread_mutex_t *mutex);

typedef struct thread_event {
    void *handle;
} thread_event_t;

int thread_event_init(thread_event_t *event);
int thread_event_destroy(thread_event_t *event);
int thread_event_signal(thread_event_t *event);
int thread_event_reset(thread_event_t *event);
int thread_event_wait(thread_event_t *event);
int thread_event_timedwait(thread_event_t *event, uint32_t ms);

#endif // COMMON_THREAD_H
