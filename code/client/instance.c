#ifdef CORE_INSTANCES_C
#error "instance.c included more than once"
#endif
#define CORE_INSTANCES_C

void begin_travel(GwClient *client)
{
    client->try_changing_zone = true;
    // GameSrv_Disconnect();
}

void start_loading_new_zone(GwClient *client, struct sockaddr *host,
    uint32_t map_id, uint32_t world_hash, uint32_t player_hash, bool record_packets)
{
    client->ingame = false;
    client->screen = SCREEN_LOADING_MAP;

    client->try_changing_zone = false;
    memzero(&client->inventory, sizeof(client->inventory));

    if (client->ingame) {
        // @Cleanup: Fix remove here
        // NetConn_Remove(client->game_srv);

        client->player_agent_id = 0;
        client->player = NULL;

        reset_world(&client->world, &client->object_mgr);
    }

    init_connection(&client->game_srv);
    client->game_srv.host = *host;

    // NEW WORLD -- START
    World *world = &client->world;
    init_world(world, world_hash);
    world->map_id = map_id;

    if (record_packets) {
        char file_path[255];
        snprintf(file_path, sizeof(file_path), "replays/replay-%d-%llu.gwr", map_id, time_get_ms());
        if (replay_open_record_file(&client->game_srv.replay_ctx, file_path)) {
            client->game_srv.flags |= NETCONN_RECORD;
        }

        Connection *conn = &client->game_srv;
        conn->broadcast = ConnectionAlloc(conn);
        if (!ObsvSrv_Connect(conn->broadcast)) {
            LogError("Couldn't connect to broadcast server");
            NetConn_Reset(conn->broadcast);
            free(conn->broadcast);
            conn->broadcast = NULL;
        }
    }

    Character *cc = client->current_character;
    GameSrv_RegisterCallbacks(&client->game_srv);

    if (!GameSrv_Connect(&client->game_srv, client->uuid, cc->uuid, world_hash, player_hash, map_id)) {
        LogInfo("Game handshake failed !");

        reset_world(world, &client->object_mgr);
        NetConn_Reset(&client->game_srv);
        client->screen = SCREEN_CHARACTER_SELECT;
        client->ingame = false;
        return;
    }
}

void HandleGameServerInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t request;
        uint32_t world_hash;
        uint32_t map_id;
        uint8_t host[24];
        uint32_t player_hash;
    } GameServerInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_GAME_SERVER_INFO);
    assert(sizeof(GameServerInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    GameServerInfo *pack = cast(GameServerInfo *)packet;
    assert(client);

    struct sockaddr host;
    memcpy(&host, pack->host, sizeof(host));
    start_loading_new_zone(client, &host, pack->map_id, pack->world_hash, pack->player_hash, true);
}

void HandleGameTransferInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t host[24];
        uint32_t world_hash;
        uint8_t  region;
        uint16_t map_id;
        uint8_t  is_explorable;
        uint32_t player_hash;
    } ServerInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRANSFER_GAME_SERVER_INFO);
    assert(sizeof(ServerInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ServerInfo *pack = cast(ServerInfo *)packet;
    assert(client && client->ingame);

    struct sockaddr host;
    memcpy(&host, pack->host, sizeof(host));
    start_loading_new_zone(client, &host, pack->map_id, pack->world_hash, pack->player_hash, true);
    broadcast_event(&client->event_mgr, WORLD_MAP_LEAVE, NULL);
}

void HandleInstanceCountdownStop(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_INSTANCE_COUNTDOWN_STOP);
    assert(sizeof(Packet) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->ingame);

    client->world.pvp_timer_start = 0;
    client->world.pvp_timer_duration = 0;
}

void HandleInstanceCountdown(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t text[122];
        int8_t unk1;
        int32_t unk2;
        int32_t time;
    } Countdown;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_COUNTDOWN);
    assert(sizeof(Countdown) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    Countdown *pack = cast(Countdown *)packet;
    assert(client && client->ingame);

    World *world = &client->world;

    world->pvp_timer_start = world->world_time;
    world->pvp_timer_duration = pack->time;
}

void GameSrv_TravelGH(GwClient *client, const uuid_t guild_uuid)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uuid_t uuid;
        int8_t unk1;
    } TravelHall;
#pragma pack(pop)
    
    assert(client && client->ingame);
    begin_travel(client);

    TravelHall packet = NewPacket(GAME_CMSG_PARTY_ENTER_GUILD_HALL);
    uuid_enc_le(packet.uuid, guild_uuid);

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LeaveGH(GwClient *client)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t unk1;
    } LeaveHall;
#pragma pack(pop)

    assert(client && client->ingame);
    LeaveHall packet = NewPacket(GAME_CMSG_PARTY_LEAVE_GUILD_HALL);
    packet.unk1 = 1;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void extract_district(World *world,
    District district, DistrictRegion *region, DistrictLanguage *language)
{
    switch (district) {
        case DISTRICT_INTERNATIONAL:
            *region = DistrictRegion_International;
            *language = DistrictLanguage_Default;
            break;
        case DISTRICT_AMERICAN:
            *region = DistrictRegion_America;
            *language = DistrictLanguage_Default;
            break;
        case DISTRICT_EUROPE_ENGLISH:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_English;
            break;
        case DISTRICT_EUROPE_FRENCH:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_French;
            break;
        case DISTRICT_EUROPE_GERMAN:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_German;
            break;
        case DISTRICT_EUROPE_ITALIAN:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_Italian;
            break;
        case DISTRICT_EUROPE_SPANISH:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_Spanish;
            break;
        case DISTRICT_EUROPE_POLISH:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_Polish;
            break;
        case DISTRICT_EUROPE_RUSSIAN:
            *region = DistrictRegion_Europe;
            *language = DistrictLanguage_Russian;
            break;
        case DISTRICT_ASIA_KOREAN:
            *region = DistrictRegion_Korea;
            *language = DistrictLanguage_Default;
            break;
        case DISTRICT_ASIA_CHINESE:
            *region = DistrictRegion_China;
            *language = DistrictLanguage_Default;
            break;
        case DISTRICT_ASIA_JAPANESE:
            *region = DistrictRegion_Japanese;
            *language = DistrictLanguage_Default;
            break;
        case DISTRICT_CURRENT:
        default:
            *region = world ? world->region : 0;
            *language = world ? world->language : 0;
            break;
    }
}

void GameSrv_Travel(GwClient *client, int map_id, District district, int district_number)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint16_t map_id;
        int8_t   region; // signed because international is -2
        uint16_t district;
        uint8_t  language;
        uint8_t  unk1;
    } TravelInfo;
#pragma pack(pop)

    assert(client && client->ingame);
    begin_travel(client);

    TravelInfo packet = NewPacket(GAME_CMSG_PARTY_TRAVEL);

    DistrictRegion region;
    DistrictLanguage language;
    extract_district(&client->world, district, &region, &language);

    packet.map_id   = map_id;
    packet.region   = region;
    packet.district = district_number;
    packet.language = language;
    packet.unk1 = 0;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void HandleInstanceTravelTimer(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint16_t time_in_sec;
        uint8_t  unk2;
        uint16_t msg[122];
    } PartyTravel;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_TRAVEL_TIMER);
    assert(sizeof(PartyTravel) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    PartyTravel *pack = cast(PartyTravel *)packet;
    assert(client && client->ingame);
}

void HandleInstanceLoaded(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t unk1;
    } InstanceLoaded;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_LOADED);
    assert(sizeof(InstanceLoaded) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    InstanceLoaded *pack = cast(InstanceLoaded *)packet;
    assert(client && client->ingame);

    client->ingame = true;
    client->screen = SCREEN_INGAME;
}

void HandleInstanceLoadFinish(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_FINISH);
    assert(sizeof(Packet) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->ingame);

    broadcast_event(&client->event_mgr, WORLD_MAP_ENTER, NULL);
}

void GameSrv_ReturnToOutpost(GwClient *client)
{
    assert(client && client->ingame);
    begin_travel(client);
    
    Packet packet = NewPacket(GAME_CMSG_PARTY_RETURN_TO_OUTPOST);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
