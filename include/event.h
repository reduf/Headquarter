#ifndef EVENT_H
#define EVENT_H

#include <common/list.h>

typedef enum EventType {
    EventType_ChatMessage,

    EventType_Disconnect,

    EventType_TradeRequest,
    EventType_TradeRequestCancel,

    EventType_WorldEnter,
    EventType_WorldLeave,

    EventType_Count,

    AUTH_DISCONNECT,

    PARTY_INVITE_REQUEST,
    PARTY_MEMBERS_CHANGED,
    PARTY_LEADER_CHANGED,
    PARTY_MEMBER_DISCONNECT,
    PARTY_DIFFICULTY_CHANGED,

    WORLD_MAP_ENTER,
    WORLD_MAP_LEAVE,
    WORLD_CANT_TRAVEL,

    QUEST_ACCEPTED,

    CINEMATIC_PLAY,
    CINEMATIC_STOP,

    TRADE_REQUEST,
    TRADE_REQUEST_CANCEL,
    TRADE_CLOSED,
    TRADE_ACCEPT_UPDATE,
    TRADE_OFFER_RECEIVED,

    PLAYER_DEAD,
    PLAYER_ALIVE,

    AGENT_FOCUS,

    DIALOG_OPENNED,

    N_EVENT
} EventType;

typedef void (__cdecl *Callback)(EventType event, void *args, void *param);
typedef struct CallbackEntry {
    struct list node;
    bool        registered;

    Callback    callback;
    void       *param;
} CallbackEntry;

static inline void
CallbackEntry_Init(CallbackEntry *entry, Callback cb, void *param)
{
    entry->registered = false;
    entry->callback = cb;
    entry->param = param;
}

typedef struct Event_ChatMessage {
    Channel             channel;

    struct {
        size_t          length;
        const uint16_t *buffer;
    } message;

    struct {
        size_t          length;
        const uint16_t *buffer;
    } sender;
} Event_ChatMessage;

typedef struct Event_AgentFocus {
    uint32_t player_id; // player that issued the focus
    uint32_t agent_id;  // id of the focused mob
} Event_AgentFocus;

typedef struct Event_DialogOpenned {
    uint32_t sender_agent_id;
} Event_DialogOpenned;

typedef struct Event_PartyInviteRequest {
    uint32_t party_id;
} Event_PartyInviteRequest;

#endif // EVENT_H