#ifdef TIMER_C
#error "timer.c included more than once"
#endif
#define TIMER_C

#include "timer.h"

static struct rbtree timer_rbtree;
static struct rbnode timer_sentinel;

void init_timers(void)
{
    rbtree_init(&timer_rbtree, &timer_sentinel);
}

void process_timers(void)
{
    struct rbnode *root, *node;
    struct rbnode *sentinel = &timer_sentinel;

    // @Enhancement: Maybe use something more accurate
    uint64_t current_time = time_get_ms();

    for (;;) {
        root = timer_rbtree.root;

        if (root == sentinel)
            return;

        node = rbtree_min(&timer_rbtree, root);

        if (current_time < node->key) {
            // the rbtree traversall is ordered, so we can early exit
            return;
        }

        rbtree_delete(&timer_rbtree, node);
        struct timer* timer_v = CAST_STRUCT_FROM_MEMBER(node, struct timer, rbnode);
        timer_v->timer_set = 0;
        timer_v->handler(timer_v);
    }
}

void timer_del(struct timer *timer)
{
    rbtree_delete(&timer_rbtree, &timer->rbnode);
    timer->timer_set = 0;
}

void timer_add(struct timer *timer, uint32_t timeout)
{
    rbtree_key_t key = time_get_ms() + timeout;

    if (timer->timer_set) {

        timer_del(timer);
    }

    timer->rbnode.key = key;

    rbtree_insert(&timer_rbtree, &timer->rbnode);
    timer->timer_set = true;
}
