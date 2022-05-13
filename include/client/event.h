#ifndef EVENT_H
#define EVENT_H

#include <common/list.h>

typedef enum EventType {
    EventType_AgentFocus,
    EventType_ChatMessage,
    EventType_CinematicPlay,
    EventType_CinematicStop,
    EventType_DialogOpen,
    EventType_DialogButton,
    EventType_AuthError,
    EventType_FriendStatus,
    EventType_GuildMemberUpdated,
    EventType_ItemQuotePrice,
    EventType_PartyInviteRequest,
    EventType_PartyLeaderChanged,
    EventType_PartyMembersChanged,
    EventType_PartySearchAdvertisement,
    EventType_SalvageSessionStart,
    EventType_WorldCantTravel,
    EventType_WorldMapEnter,
    EventType_WorldMapLeave,

    EventType_Count
} EventType;

typedef struct Event {
    EventType type;
    union {
        struct {
            uint32_t player_id; // player that issued the focus
            uint32_t agent_id;  // id of the focused mob
        } AgentFocus;
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
            uint32_t sender_agent_id;
        } DialogOpen;
        struct {
            int button_id;
            int icon_id;
        } DialogButton;
        struct {
            uint32_t type;
            uint32_t code;
        } AuthError;
        struct {
            ApiFriend friend;
        } FriendStatus;
        struct {
            ApiGuildMember member;
        } GuildMemberUpdated;
        struct {
            int32_t     item_id;
            uint32_t    quote_price;
        } ItemQuotePrice;
        struct {
            uint32_t party_id;
        } PartyInviteRequest;
        struct {
            uint32_t party_id;
        } PartyMembersChanged;
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
            uint32_t item_id;
        } SalvageSessionStart;
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
