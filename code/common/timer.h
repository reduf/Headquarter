#ifndef COMMON_TIMER_H
#define COMMON_TIMER_H

#include <stdbool.h>

#include "rbtree.h"

struct timer;
typedef void (*timer_callback_pt)(struct timer *timer);
struct timer {
    struct rbnode       rbnode;
    bool                timedout;
    bool                timer_set;

    timer_callback_pt   handler;
};

void init_timers(void);
void process_timers(void);

void timer_del(struct timer *timer);
void timer_add(struct timer *timer, uint32_t timeout);

#endif // COMMON_TIMER_H
