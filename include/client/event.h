#ifndef EVENT_H
#define EVENT_H

#include <common/list.h>

typedef enum EventType {
    EventType_PartySearchAdvertisement,
    EventType_GuildMemberUpdated,
    EventType_FriendUpdated,
    EventType_Error,
    EventType_ItemQuotePrice, // Item quote received.
    EventType_ChatMessage,
    EventType_FriendListMessage,
    EventType_Disconnect,
    EventType_AuthDisconnect,
    EventType_PartyInviteRequest,
    EventType_PartyMembersChanged,
    EventType_PartyLeaderChanged,
    EventType_PartyMemberDisconnect,
    EventType_PartyDifficultyChanged,
    EventType_WorldMapEnter,
    EventType_WorldMapLeave,
    EventType_WorldCantTravel,
    EventType_QuestAccepted,
    EventType_CinematicPlay,
    EventType_CinematicStop,
    EventType_TradeRequest,
    EventType_TradeRequestCancel,
    EventType_TradeClosed,
    EventType_TradeAcceptUpdate,
    EventType_TradeOfferReceived,
    EventType_SalvageSessionStart,
    EventType_PlayerDead,
    EventType_PlayerAlive,
    EventType_AgentFocus,
    EventType_DialogOpen,
    EventType_MerchantWindowOpened,

    EventType_Count
} EventType;

typedef struct Event {
    EventType type;
    union {
        struct {
            Channel             channel;
            uint16_t            extra_id;
            struct {
                size_t          length;
                const uint16_t *buffer;
            } message;

            struct {
                size_t          length;
                const uint16_t *buffer;
            } sender;
        } ChatMessage;
        struct {
            uint16_t            party_id;
            uint8_t             party_size;
            uint8_t             hero_count;
            uint8_t             search_type;
            uint16_t            district;
            struct {
                size_t          length;
                const uint16_t* buffer;
            } message;
            struct {
                size_t          length;
                const uint16_t* buffer;
            } sender;
        } PartySearchAdvertisement;
        struct {
            int32_t     item_id;
            uint32_t    quote_price;
        } ItemPrice;
        struct {
            ApiGuildMember member;
        } GuildMemberUpdated;
        struct {
            ApiFriend gwfriend;
        } FriendStatus;
        struct {
            uint32_t player_id; // player that issued the focus
            uint32_t agent_id;  // id of the focused mob
        } AgentFocus;
        struct {
            uint32_t type;
            uint32_t code;
        } Error;
        struct {
            uint32_t sender_agent_id;
        } DialogOpen;
        struct {
            uint32_t item_id;
        } SalvageSessionStart;
        struct {
            uint32_t party_id;
        } PartyInviteRequest;
        struct {
            uint32_t party_id;
        } PartyInviteChanged;
        struct {
            int8_t value;
        } WorldCantTravel;
    };
} Event;

void Event_Init(Event *event, EventType type)
{
    memset(event, 0, sizeof(*event));
    event->type = type;
}

typedef void (*Callback)(Event *event, void *param);
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

#endif // EVENT_H
