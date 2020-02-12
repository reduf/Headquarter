#ifdef CORE_AUTH_C
#error "auth.c included more than once"
#endif
#define CORE_AUTH_C

void HandleAccountSettings(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uint32_t n_data;
        uint8_t  data[2048];
    } AccountSettings;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_ACCOUNT_SETTINGS);
    assert(sizeof(AccountSettings) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    AccountSettings *pack = cast(AccountSettings *)packet;
    assert(client);
    (void)pack;
}

void HandleErrorMessage(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uint32_t code;
    } ErrorMessage;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_ERROR_MESSAGE);
    assert(sizeof(ErrorMessage) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ErrorMessage *pack = cast(ErrorMessage *)packet;
    assert(client);

    // @Cleanup: We should handle error code like code 13
    const char *error_s = get_error_s(pack->code);
    if (pack->code != 0) {
        LogInfo("(Code=%03d) %s", pack->code, error_s);
    }

    uint32_t trans_id = pack->trans_id;
    if (trans_id < 0) {
        LogError("'HandleErrorMessage' received bad transaction id %d", trans_id);
        return;
    }

    TransactionArray *transactions = &client->transactions;
    if (!array_inside(*transactions, trans_id)) {
        LogError("'HandleErrorMessage' received transaction %d, but maximum is %d",
            trans_id, array_size(*transactions));
        return;
    }

    Transaction *trans = &array_at(*transactions, trans_id);
    if (!trans->enable) {
        LogError("'HandleErrorMessage' received unexpected transaction '%d'", trans_id);
        return;
    }

    trans->error_code = pack->code;
    thread_event_signal(&trans->event);
}

void HandleServerReponse(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uint32_t code;
    } ServerReponse;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_SERVER_RESPONSE);
    assert(sizeof(ServerReponse) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ServerReponse *pack = cast(ServerReponse *)packet;
    assert(client);
    (void)pack;
}

void HandleSessionInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t server_salt;
        uint32_t unk0;
        uint32_t unk1;
        uint32_t unk2;
    } SessionInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_SESSION_INFO);
    assert(sizeof(SessionInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SessionInfo *pack = cast(SessionInfo *)packet;
    assert(client);

    client->static_salt = pack->server_salt;
    client->screen = SCREEN_LOGIN_SCREEN;
}

void HandleAccountInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t trans_id;
        uint32_t territory;
        uint32_t unk0;
        uint8_t unk1[8];
        uint8_t unk2[8];
        uuid_t account_uuid;
        uuid_t character_uuid;
        uint32_t unk3;
        uint32_t n_unk4;
        uint8_t unk4[200];
        uint8_t eula_accepted;
        int32_t unk5;
    } AccountInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_ACCOUNT_INFO);
    assert(sizeof(AccountInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    AccountInfo *pack = cast(AccountInfo *)packet;
    assert(client);

    uuid_t char_uuid;
    uuid_dec_le(pack->account_uuid, client->uuid);
    uuid_dec_le(pack->character_uuid, char_uuid);

    Character *character;
    array_foreach(character, client->characters) {
        if (uuid_equals(character->uuid, char_uuid)) {
            client->current_character = character;
            break;
        }
    }

    string name = client->current_character->name;
    LogInfo("Current character: %.*s", name.count, name.bytes);
}

void HandleCharacterInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uuid_t   uuid;
        uint32_t unk0;
        uint16_t name[20];
        uint32_t n_extended;
        uint8_t  extended[64];
    } CharacterInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_CHARACTER_INFO);
    assert(sizeof(CharacterInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    CharacterInfo *pack = cast(CharacterInfo *)packet;
    assert(client);

    // PrintBytes(p->name, pack->extended.data, 64);
    Character *new_character = array_push(client->characters, 1);
    if (!new_character) {
        LogError("Couldn't create a new character");
        return;
    }

    // @Cleanup: Support utf-8
    // new_character.map = *cast(u16 *)(pack->extended.data + 2);
    size_t written = unicode16_to_utf8(new_character->name_buffer,
        sizeof(new_character->name_buffer), pack->name, -1);
    new_character->name.bytes = cast(uint8_t *)new_character->name_buffer;
    new_character->name.count = written - 1;
    uuid_dec_le(pack->uuid, new_character->uuid);
}

void AuthSrv_HeartBeat(Connection *conn, msec_t tick)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t tick;
    } HeartBeat;
#pragma pack(pop)

    HeartBeat packet = NewPacket(AUTH_CMSG_HEARTBEAT);
    packet.tick = tick & 0xFFFFFF;
    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_AskServerResponse(Connection *conn, uint32_t trans_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
    } AskServerResponse;
#pragma pack(pop)

    AskServerResponse packet = NewPacket(AUTH_CMSG_ASK_SERVER_RESPONSE);
    packet.trans_id = trans_id;
    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_ComputerInfo(Connection *conn)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t username[32];
        uint16_t pcname[32];
    } ComputerInfo;
#pragma pack(pop)

#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t version;
        uint8_t  hash[16];
    } ComputerHash;
#pragma pack(pop)

    ComputerInfo info = NewPacket(AUTH_CMSG_SEND_COMPUTER_INFO);
    utf8_to_unicode16(info.username, 32, "Wyatt", -1);
    utf8_to_unicode16(info.pcname,   32, "Wyatt-PC", -1);

    ComputerHash hash = NewPacket(AUTH_CMSG_SEND_COMPUTER_HASH);
    hash.version = GUILD_WARS_VERSION;
    memcpy(hash.hash, "\x19\x0D\xB0\x37\xE6\x05\x13\x6B\x86\x18\x66\x28\x45\x7E\xDD\xB5", 16);

    SendPacket(conn, sizeof(info), &info);
    SendPacket(conn, sizeof(hash), &hash);
}

void AuthSrv_HardwareInfo(Connection *conn)
{
    // @Remark: Holy fuck c++ is retarded. Why the fuck it force you to add a null-terminated ?
    uint8_t hash[/* 16 */] = "\x9B\x99\x78\x8D\x2F\x01\xFA\x4F\x99\x82\xD3\xB8\xBD\x83\xCD\xF6";
    uint8_t info[/* 92 */] = "\x03\x00\x5C\x00\x04\x00\xDE\x03\x46\x0D\xC3\x06\x47\x65\x6E\x75\x69\x6E\x65\x49\x6E\x74\x65\x6C\x06\x02\x00\x00\x04\x00\x00\x00\xC0\x11\x00\x00\x60\x11\x00\x00\xDE\x10\x00\x00\xC6\x07\x20\x90\x55\x6E\x6B\x6E\x6F\x77\x6E\x20\x44\x65\x76\x69\x63\x65\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x55\x6E\x6B\x6E\x6F\x77\x6E\x20\x56\x65\x6E\x64\x6F\x72\x00\x00\x00\x00\x00\x00";

#pragma pack(push, 1)
    typedef struct {
        Header header;
        size_t n_info;
        uint8_t info[92];
        uint8_t hash[16];
    } HardwareInfo;
#pragma pack(pop)

    HardwareInfo packet = NewPacket(AUTH_CMSG_SEND_HARDWARE_INFO);
    packet.n_info = 92;
    memcpy(packet.info, info, 92);
    memcpy(packet.hash, hash, 16);

    SendPacket(conn, sizeof(packet), &packet);
}

// 0 = main town
// 1 = main explorable
// 2 = guild hall
// 3 = outpost/mission outpost
// 4 = mission explorable
// 5 = arena outpost
// 6 = arena explorable
// 7 = HA outpost
// 11 = character creation
void AuthSrv_RequestInstance(Connection *conn, uint32_t trans_id,
    uint32_t map_id, uint32_t type, uint32_t region, uint32_t district, uint32_t language)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uint32_t map_type;
        uint32_t map_id;
        uint32_t region;
        uint32_t district;
        uint32_t language;
    } CharacterPlayInfo;
#pragma pack(pop)

    CharacterPlayInfo packet = NewPacket(AUTH_CMSG_REQUEST_INSTANCE);
    packet.trans_id = trans_id;
    packet.map_type = type;
    packet.map_id = map_id;
    packet.region = region;
    packet.district = district;
    packet.language = language;

    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_ChangeCharacter(Connection *conn, uint32_t trans_id, string name)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t trans_id;
        uint16_t name[20];
    } ChangeCharacter;
#pragma pack(pop)

    // @Cleanup: Support utf-8
    ChangeCharacter packet = NewPacket(AUTH_CMSG_CHANGE_PLAY_CHARACTER);
    packet.trans_id = trans_id;
    if (!utf8_to_unicode16(packet.name, 20, name.bytes, name.count)) {
        LogError("Lacking of utf-8 support in AuthSrv_ChangeCharacter");
        return;
    }
    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_SetPlayerStatus(Connection *conn, PlayerStatus status)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t status;
    } SetPlayerStatus;
#pragma pack(pop)

    SetPlayerStatus packet = NewPacket(AUTH_CMSG_SET_PLAYER_STATUS);
    packet.status = status;
    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_SendPacket35(Connection *conn)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t status;
    } Packet35;
#pragma pack(pop)

    Packet35 packet = NewPacket(35);
    packet.status = 0;
    SendPacket(conn, sizeof(packet), &packet);
}

void AuthSrv_RegisterCallbacks(Connection *conn)
{
    array_init2(conn->handlers, AUTH_SMSG_COUNT);
    MsgHandler *handlers = conn->handlers.data;

    handlers[AUTH_SMSG_ERROR_MESSAGE]           = HandleErrorMessage;
    handlers[AUTH_SMSG_SERVER_RESPONSE]         = HandleServerReponse;

    handlers[AUTH_SMSG_SESSION_INFO]            = HandleSessionInfo;
    handlers[AUTH_SMSG_ACCOUNT_INFO]            = HandleAccountInfo;
    handlers[AUTH_SMSG_CHARACTER_INFO]          = HandleCharacterInfo;
    handlers[AUTH_SMSG_GAME_SERVER_INFO]        = HandleGameServerInfo;
    handlers[AUTH_SMSG_ACCOUNT_SETTINGS]        = HandleAccountSettings;

    handlers[AUTH_SMSG_FRIEND_UPDATE_INFO]      = HandleFriendUpdateInfo;
    handlers[AUTH_SMSG_FRIEND_UPDATE_STATUS]    = HandleFriendUpdateStatus;
    handlers[AUTH_SMSG_FRIEND_UPDATE_LOCATION]  = HandleFriendUpdateLocation;

    handlers[AUTH_SMSG_WHISPER_RECEIVED]        = HandleWhisperReceived;

    conn->last_tick_time = 0;
}

bool init_auth_connection(GwClient *client, const char *host)
{
    Connection *conn = &client->auth_srv;
    init_connection(conn);
    AuthSrv_RegisterCallbacks(conn);

    conn->data = client;
    if (host != NULL) {
        char buffer[256];

        // @Robustness: It doesn't support IPv6 and it won't give any error
        // if such an address is passed in arguments.
        strncpy_s(buffer, sizeof(buffer), host, sizeof(buffer) - 1);
        char *port = strrchr(buffer, ':');
        if (!port) port = "6112";

        // @Enhancement: We should print an error if he doesn't find the host.
        if (!IPv4ToAddr(buffer, port, &conn->host)) {
            LogError("Couldn't resolve the host address '%s'", host);
            return false;
        }
    }

    if (!AuthSrv_Connect(conn)) {
        LogInfo("Auth handshake failed !");
        return false;
    }

    if (!conn->secured) {
        LogError("Couldn't connect to Auth serveur");
        return false;
    }

    AuthSrv_ComputerInfo(conn);
    return true;
}
