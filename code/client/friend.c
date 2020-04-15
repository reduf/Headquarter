#ifdef CORE_FRIEND_C
#error "friend.c included more than once"
#endif
#define CORE_FRIEND_C

Friend* get_friend(uint8_t* uuid, uint16_t* name) {
    Friend* gwfriend;
    if (uuid) {
        array_foreach(gwfriend, client->friends) {
            if (memcmp(uuid, gwfriend->uuid, 16) == 0)
                return gwfriend;
        }
    }
    if (name) {
        struct kstr name_kstr;
        size_t length = 0;
        for (length = 0; length < 20 && name[length]; length++) {};
        kstr_init(&name_kstr, name, length, 20);
        array_foreach(gwfriend, client->friends) {
            if (kstr_compare(&gwfriend->name, &name_kstr) == 0 || kstr_compare(&gwfriend->account, &name_kstr) == 0)
                return gwfriend;
        }
    }
    return NULL;
}
Friend* get_or_create_friend(uint8_t* uuid, uint16_t* name) {
    Friend* gwfriend = get_friend(uuid, name);
    if (gwfriend)
        return gwfriend;
    gwfriend = array_push(client->friends, 1);
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

    Friend* gwfriend = get_or_create_friend(pack->uuid, pack->account);
    if (!gwfriend) {
        LogError("Couldn't create or find a new friend (1)");
        return;
    }
    kstr_read(&gwfriend->account, pack->account, ARRAY_SIZE(pack->account));
    uuid_dec_le(pack->uuid, gwfriend->uuid);

    gwfriend->status = pack->status;
    gwfriend->type = pack->type;

    Event_FriendStatus event;
    api_make_friend(&event.gwfriend, gwfriend);
    broadcast_event(&client->event_mgr, EventType_Friend_Updated, &event);
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
    Friend* gwfriend = get_or_create_friend(pack->uuid, pack->account);
    if (!gwfriend) {
        LogError("Couldn't create or find a new friend (2)");
        return;
    }
    kstr_read(&gwfriend->name, pack->played, ARRAY_SIZE(pack->played));

    gwfriend->status = pack->status;

    Event_FriendStatus event;
    api_make_friend(&event.gwfriend, gwfriend);
    broadcast_event(&client->event_mgr, EventType_Friend_Updated, &event);
    //LogInfo("Friend status updated: %ls (%ls), status %d, type %d, map %d", friend->name_buffer, friend->account_buffer, friend->status, friend->type, friend->zone);
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
    Friend* gwfriend = get_or_create_friend(NULL, pack->account);
    if (!gwfriend) {
        LogError("Couldn't create or find a new friend (3)");
        return;
    }

    gwfriend->zone = pack->map_id;
    gwfriend->type = pack->type;

    Event_FriendStatus event;
    api_make_friend(&event.gwfriend, gwfriend);
    broadcast_event(&client->event_mgr, EventType_Friend_Updated, &event);
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

    GwClient *client = cast(GwClient *)conn->data;
    FriendStreamEnd *pack = cast(FriendStreamEnd *)packet;
}
