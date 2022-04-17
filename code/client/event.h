#ifdef EVENTS_H
#error "events.h included more than once"
#endif
#define EVENTS_H

typedef struct EventManager {
    struct list callbacks[EventType_Count];
} EventManager;

void init_event_manager(EventManager *mgr);
void events_add_entry(EventManager *mgr, EventType e, CallbackEntry *entry);
void events_rem_entry(EventManager *mgr, CallbackEntry *entry);
void broadcast_event(EventManager *mgr, Event *event);
bool is_event_subscribed(EventManager* mgr, EventType e);
