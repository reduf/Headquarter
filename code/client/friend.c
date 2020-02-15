#ifdef CORE_FRIEND_C
#error "friend.c included more than once"
#endif
#define CORE_FRIEND_C

void HandleFriendUpdateInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t status;
        uint32_t type;
        uint8_t  uuid[16]; // confirm
        uint16_t name[20];
    } UpdateInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_FRIEND_UPDATE_INFO);
    assert(sizeof(UpdateInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateInfo *pack = cast(UpdateInfo *)packet;
    assert(client);
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
}

void HandleFriendUpdateLocation(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t unk1; // map_id
        int16_t unk2;
        int8_t  unk3; // type
        int8_t  unk4;
        uint16_t name[20];
    } UpdateLocation;
#pragma pack(pop)
    
    assert(packet->header == AUTH_SMSG_FRIEND_UPDATE_LOCATION);
    assert(sizeof(UpdateLocation) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateLocation *pack = cast(UpdateLocation *)packet;
    assert(client);
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
