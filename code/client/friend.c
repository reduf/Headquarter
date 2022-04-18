#ifdef CORE_FRIEND_C
#error "friend.c included more than once"
#endif
#define CORE_FRIEND_C

Friend *get_friend(const uint8_t *uuid, const uint16_t *name)
{
    Friend *gwfriend;
    if (uuid) {
        array_foreach(gwfriend, &client->friends) {
            if (memcmp(uuid, gwfriend->uuid, 16) == 0)
                return gwfriend;
        }
    }
    if (name) {
        size_t length = 0;
        for (length = 0; length < 20 && name[length]; length++) {}

        const struct kstr name_kstr = {
            .length = length,
            .capacity = length,
            .buffer = (uint16_t *)name,
        };

        array_foreach(gwfriend, &client->friends) {
            if (kstr_compare(&gwfriend->name, &name_kstr) == 0 || kstr_compare(&gwfriend->account, &name_kstr) == 0)
                return gwfriend;
        }
    }
    return NULL;
}

Friend *get_or_create_friend(uint8_t *uuid, uint16_t *name)
{
    Friend *gwfriend = get_friend(uuid, name);
    if (gwfriend)
        return gwfriend;
    gwfriend = array_push(&client->friends, 1);
    if (!gwfriend) {
        return NULL;
    }
    init_friend(gwfriend);
    return gwfriend;
}

void HandleFriendUpdateInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t status;
        uint32_t type;
        uint8_t  uuid[16]; // confirm
        uint16_t account[20];
    } UpdateInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_FRIEND_UPDATE_INFO);
    assert(sizeof(UpdateInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateInfo *pack = cast(UpdateInfo *)packet;
    assert(client);

    Friend *gwfriend = get_or_create_friend(pack->uuid, pack->account);
    assert(gwfriend);
    kstr_read(&gwfriend->account, pack->account, ARRAY_SIZE(pack->account));
    uuid_dec_le(pack->uuid, gwfriend->uuid);

    gwfriend->type = pack->type;
    if (gwfriend->status < 1) {
        // Offline, reset.
        gwfriend->status = 0;
        gwfriend->name.length = 0;
        gwfriend->name.buffer[0] = 0;
        gwfriend->zone = 0;
    }

    //Event_FriendStatus event;
    //api_make_friend(&event.gwfriend, gwfriend);
    //broadcast_event(&client->event_mgr, EventType_FriendUpdated, &event);
    //LogInfo("Friend info updated: %ls (%ls), status %d, type %d, map %d", friend->name_buffer, friend->account_buffer, friend->status, friend->type, friend->zone);
}

void HandleFriendUpdateStatus(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t status;
        uint8_t  uuid[16]; // confirm
        uint16_t account[20];
        uint16_t played[20];
    } UpdateStatus;
#pragma pack(pop)
    
    assert(packet->header == AUTH_SMSG_FRIEND_UPDATE_STATUS);
    assert(sizeof(UpdateStatus) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateStatus *pack = cast(UpdateStatus *)packet;
    assert(client);
    assert(pack->status >= 0 && pack->status <= 3);
    Friend *gwfriend = get_or_create_friend(pack->uuid, NULL);
    assert(gwfriend);
    gwfriend->status = pack->status;
    if (gwfriend->status < 1) {
        // Offline, reset.
        gwfriend->status = 0;
        gwfriend->name.length = 0;
        gwfriend->name.buffer[0] = 0;
        gwfriend->zone = 0;
    } else {
        // Online, read in player name
        kstr_read(&gwfriend->name, pack->played, ARRAY_SIZE(pack->played));
    }

    if (!list_empty(&client->event_mgr.callbacks[EventType_FriendUpdated])) {
        Event event;
        Event_Init(&event, EventType_FriendUpdated);

        api_make_friend(&event.FriendStatus.gwfriend, gwfriend);
        broadcast_event(&client->event_mgr, &event);
    }
}

void HandleFriendUpdateLocation(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t map_id; // map_id
        int32_t type;
        uint16_t account[20];
    } UpdateLocation;
#pragma pack(pop)
    
    assert(packet->header == AUTH_SMSG_FRIEND_UPDATE_LOCATION);
    assert(sizeof(UpdateLocation) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateLocation *pack = cast(UpdateLocation *)packet;
    assert(client);
    Friend *gwfriend = get_or_create_friend(NULL, pack->account);
    if (!gwfriend) {
        LogError("Couldn't create or find a new friend (3)");
        return;
    }

    gwfriend->zone = pack->map_id;
    if (!list_empty(&client->event_mgr.callbacks[EventType_FriendUpdated])) {
        Event event;
        Event_Init(&event, EventType_FriendUpdated);
        api_make_friend(&event.FriendStatus.gwfriend, gwfriend);
        broadcast_event(&client->event_mgr, &event);
    }
    //LogInfo("Friend location updated: %ls (%ls), status %d, type %d, map %d", friend->name_buffer, friend->account_buffer, friend->status, friend->type, friend->zone);
}

void HandleFriendStreamEnd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t unk1;
        uint32_t unk2;
    } FriendStreamEnd;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_FRIEND_STREAM_END);
    assert(sizeof(FriendStreamEnd) == psize);

    (void)conn;
}
