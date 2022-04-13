#ifdef EVENTS_C
#error "events.c included more than once"
#endif
#define EVENTS_C

void init_event_manager(EventManager *mgr)
{
    for (size_t i = 0; i < N_EVENT; i++) {
        list_init(&mgr->callbacks[i]);
    }
}

void events_add_entry(EventManager *mgr, EventType e, CallbackEntry *entry)
{
    assert(mgr && entry && entry->callback);
    assert(0 <= e && e < N_EVENT);
    struct list *list = &mgr->callbacks[e];
    list_insert_head(list, &entry->node);
    entry->registered = true;
}

void events_rem_entry(EventManager *mgr, CallbackEntry *entry)
{
    assert(mgr && entry);
    struct list *node = &entry->node;
    assert(node->next && node->prev);
    list_remove(node);
#if !defined(NDEBUG)
    node->next = NULL;
    node->prev = NULL;
#endif
    entry->registered = false;
}

bool is_event_subscribed(EventManager* mgr, EventType e)
{
    return 0 <= e && e < N_EVENT && !list_empty(&mgr->callbacks[e]);
}

void broadcast_event(EventManager *mgr, EventType e, void *args)
{
    assert(0 <= e && e < N_EVENT);

    struct list *it = list_first(&mgr->callbacks[e]);
    while (!list_end(&mgr->callbacks[e], it)) {
        CallbackEntry *entry = list_entry(it, CallbackEntry, node);

        // We increment the iterator before processing the callback, because
        // the callback can unregister itself. By incrementing the `it` right
        // away, it will work fine.
        it = list_next(it);

        if (entry->callback) {
            entry->callback(e, args, entry->param);
        }
    }
}
