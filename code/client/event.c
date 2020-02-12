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

void broadcast_event(EventManager *mgr, EventType e, void *args)
{
    assert(0 <= e && e < N_EVENT);
    CallbackEntry *entry;
    struct list   *it;
    list_for_each(&mgr->callbacks[e], it) {
        entry = list_entry(it, CallbackEntry, node);
        if (entry->callback) {
            entry->callback(e, args, entry->param);
        }
    }
}
