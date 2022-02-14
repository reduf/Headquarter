#ifdef ASYNC_H
#error "async.h is already included"
#endif
#define ASYNC_H

#include "event.h"

#define async_completed(s) ((s).completed)

#define async_waitfor(s, f) _async_waitfor(s, f, &async_completed(s))

typedef struct AsyncState_WaitMapLoading {
    bool            completed;

    bool            event_triggered;
    CallbackEntry   map_enter;
    CallbackEntry   cant_travel;
} AsyncState_WaitMapLoading;

static void async_cb_map_enter(EventType event, void *args, void *param)
{
    assert(event == WORLD_MAP_ENTER);
    assert(param != NULL);
    bool *event_triggered = (bool *)param;
    *event_triggered = true;
}

static void async_cb_cant_travel(EventType event, void *args, void *param)
{
    assert(event == WORLD_CANT_TRAVEL);
    assert(param != NULL);
    bool *event_triggered = (bool *)param;
    *event_triggered = true;
}

void async_init_WaitMapLoading(AsyncState_WaitMapLoading *state)
{
    state->completed = false;
    state->event_triggered = false;

    CallbackEntry_Init(&state->map_enter, async_cb_map_enter, &state->event_triggered);
    CallbackEntry_Init(&state->cant_travel, async_cb_cant_travel, &state->event_triggered);

    RegisterEvent(WORLD_MAP_ENTER, &state->map_enter);
    RegisterEvent(WORLD_CANT_TRAVEL, &state->cant_travel);
}

void async_reset_WaitMapLoading(AsyncState_WaitMapLoading *state)
{
    UnRegisterEvent(&state->map_enter);
    UnRegisterEvent(&state->cant_travel);
}

void async_update_WaitMapLoading(AsyncState_WaitMapLoading *state)
{
    if (state->event_triggered) {
        async_reset_WaitMapLoading(state);
        state->completed = true;
    }
}

typedef struct AsyncState_RedirectMap {
    bool            completed;

    AsyncState_WaitMapLoading wait_map_loading;
} AsyncState_RedirectMap;

void async_init_RedirectMap(AsyncState_RedirectMap *state,
    uint32_t map_id, uint32_t type, District district, int district_number)
{
    state->completed = false;
    async_init_WaitMapLoading(&state->wait_map_loading);

    RedirectMap(map_id, type, district, district_number);
}

void async_reset_RedirectMap(AsyncState_RedirectMap *state)
{
    async_reset_WaitMapLoading(&state->wait_map_loading);
}

void async_update_RedirectMap(AsyncState_RedirectMap *state)
{
    async_update_WaitMapLoading(&state->wait_map_loading);
    if (state->wait_map_loading.completed) {
        async_reset_RedirectMap(state);
        state->completed = true;
    }
}

typedef struct AsyncState_Travel {
    bool            completed;

    AsyncState_WaitMapLoading wait_map_loading;
} AsyncState_Travel;

void async_init_Travel(AsyncState_Travel *state,
    uint32_t map_id, District district, int district_number)
{
    state->completed = false;
    async_init_WaitMapLoading(&state->wait_map_loading);

    Travel(map_id, district, district_number);
}

void async_reset_Travel(AsyncState_Travel *state)
{
    async_reset_WaitMapLoading(&state->wait_map_loading);
}

void async_update_Travel(AsyncState_Travel *state)
{
    async_update_WaitMapLoading(&state->wait_map_loading);
    if (state->wait_map_loading.completed) {
        async_reset_Travel(state);
        state->completed = true;
    }
}
typedef struct QuoteDetails {
    int             quote_item_id;
    uint32_t price;
} QuoteDetails;
typedef struct AsyncState_RequestQuote {
    QuoteDetails info;
    CallbackEntry   callback;
} AsyncState_RequestQuote;

static void async_cb_quote_received(EventType event, void* args, void* param)
{
    assert(event == EventType_ItemQuotePrice);
    assert(param);
    Event_ItemPrice* quoted_item = args;
    QuoteDetails* info = param;
    if (info->quote_item_id != quoted_item->item_id)
        return; // Different item...
    info->price = quoted_item->quote_price;
}

void async_init_RequestQuote(AsyncState_RequestQuote* state, uint32_t item_id)
{
    state->info.quote_item_id = item_id;
    state->info.price = 0;
    CallbackEntry_Init(&state->callback, async_cb_quote_received, &state->info);
    RegisterEvent(EventType_ItemQuotePrice, &state->callback);

    if (!RequestItemQuote(item_id)) {
        state->info.price = 0;
    }
}
void async_reset_RequestQuote(AsyncState_RequestQuote* state)
{
    UnRegisterEvent(&state->callback);
}
void async_update_RequestQuote(AsyncState_RequestQuote* state)
{
    if (state->info.price != 0) {
        async_reset_RequestQuote(state);
    }
}

typedef struct AsyncState_GotoNpc {
    bool            completed;

    bool            event_triggered;
    CallbackEntry   dialog_opened;
} AsyncState_GotoNpc;

static void async_cb_dialog_opened(EventType event, void *args, void *param)
{
    assert(event == DIALOG_OPENNED);
    assert(param != NULL);

    bool *event_triggered = (bool *)param;
    *event_triggered = true;
}

void async_init_GotoNpc(AsyncState_GotoNpc *state, uint32_t agent_id)
{
    // @Robustness:
    // Might be interesting to do extra check here, to ensure we don't get stuck.
    state->completed = false;

    state->event_triggered = false;
    CallbackEntry_Init(&state->dialog_opened, async_cb_dialog_opened, &state->event_triggered);
    RegisterEvent(DIALOG_OPENNED, &state->dialog_opened);
    
    InteractAgent(agent_id);
}

void async_reset_GotoNpc(AsyncState_GotoNpc *state)
{
    UnRegisterEvent(&state->dialog_opened);
}

void async_update_GotoNpc(AsyncState_GotoNpc *state)
{
    if (state->event_triggered) {
        async_reset_GotoNpc(state);
        state->completed = true;
    }
}

typedef struct AsyncState_MoveTo {
    bool            completed;

    Vec2f           dest;
    float           dist_to_reach;
} AsyncState_MoveTo;

void async_init_MoveTo(AsyncState_MoveTo *state, Vec2f dest, float dist_to_reach)
{
    state->completed = false;
    state->dest = dest;
    state->dist_to_reach = dist_to_reach;

    MoveToPoint(dest);
}

void async_reset_MoveTo(AsyncState_MoveTo *state)
{
}

// @Enhancement: @Robustness:
// We don't have to redefine dist2
static float MoveTo_dist2(Vec2f u, Vec2f v)
{
    float dx = u.x - v.x;
    float dy = u.y - v.y;
    return sqrtf((dx * dx) + (dy * dy));
}

void async_update_MoveTo(AsyncState_MoveTo *state)
{
    AgentId agent_id = GetMyAgentId();
    if (agent_id) {
        Vec2f pos = GetAgentPos(agent_id);
        float dist = MoveTo_dist2(pos, state->dest);
        if (dist <= state->dist_to_reach) {
            async_reset_MoveTo(state);
            state->completed = true;
        }
    }
}
