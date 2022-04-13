#ifndef EVENT_H
#define EVENT_H

#include <common/list.h>

typedef enum EventType {
    EventType_PartySearchAdvertisement,
    EventType_GuildMember_Updated,
    EventType_Friend_Updated,
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
    EventType_DialogOpenned,
    EventType_MerchantWindowOpened,

    EventType_Count,
} EventType;

typedef void (*Callback)(EventType event, void *args, void *param);
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
    uint16_t extra_id;
    struct {
        size_t          length;
        const uint16_t *buffer;
    } message;

    struct {
        size_t          length;
        const uint16_t *buffer;
    } sender;
} Event_ChatMessage;

/*
    typedef struct {
        Header header;
        uint16_t ps_id;
        uint16_t district;
        uint8_t  unk1;
        uint8_t  party_size;
        uint8_t  hero_count;
        uint8_t  search_type;
        uint8_t  unk2;
        uint16_t message[32];
        uint16_t player_name[20];
        uint8_t  unk3;
        uint8_t  unk4;
        uint8_t  unk5;
        uint32_t unk6;
    } PacketType;
*/
typedef struct Event_PartySearchAdvertisement {
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
} Event_PartySearchAdvertisement;

typedef struct Event_ItemPrice {
    int32_t     item_id;
    uint32_t    quote_price;
} Event_ItemPrice;

typedef struct Event_GuildMemberUpdated {
    ApiGuildMember member;
} Event_GuildMemberUpdated;

typedef struct Event_FriendStatus {
    ApiFriend gwfriend;
} Event_FriendStatus;

typedef struct Event_AgentFocus {
    uint32_t player_id; // player that issued the focus
    uint32_t agent_id;  // id of the focused mob
} Event_AgentFocus;

typedef struct Event_Error {
    uint32_t type;
    uint32_t code;
} Event_Error;

typedef struct Event_DialogOpenned {
    uint32_t sender_agent_id;
} Event_DialogOpenned;

typedef struct Event_PartyInviteRequest {
    uint32_t party_id;
} Event_PartyInviteRequest;

#endif // EVENT_H