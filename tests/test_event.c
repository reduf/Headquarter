#include <assert.h>
#include <stdint.h>

#include "utest.h"

// @Cleanup: Big hack, but I need to fix str.h
#define STR_H_INC
#include "../include/client/Headquarter.h"

#include "event.h"
#include "event.c"

UTEST_MAIN();

static CallbackEntry unregister_event_from_handler_cb1;
static void unregister_event_from_handler_func1(EventType event, void *args, void *param)
{
    EventManager *mgr = (EventManager *)param;
    events_rem_entry(mgr, &unregister_event_from_handler_cb1);
}

static CallbackEntry unregister_event_from_handler_cb_before;
static CallbackEntry unregister_event_from_handler_cb_after;
static void unregister_event_from_handler_increment(EventType event, void *args, void *param)
{
    int *res = (int *)param;
    *res += 1;
}

UTEST(client_event, unregister_event_from_handler)
{
    EventManager mgr;
    init_event_manager(&mgr);

    CallbackEntry_Init(
        &unregister_event_from_handler_cb1,
        unregister_event_from_handler_func1,
        &mgr);

    int result = 0;
    CallbackEntry_Init(
        &unregister_event_from_handler_cb_before,
        unregister_event_from_handler_increment,
        &result);
    CallbackEntry_Init(
        &unregister_event_from_handler_cb_after,
        unregister_event_from_handler_increment,
        &result);

    events_add_entry(&mgr, EventType_Error, &unregister_event_from_handler_cb_before);
    events_add_entry(&mgr, EventType_Error, &unregister_event_from_handler_cb1);
    events_add_entry(&mgr, EventType_Error, &unregister_event_from_handler_cb_after);

    broadcast_event(&mgr, EventType_Error, NULL);

    ASSERT_EQ(result, 2);
}
