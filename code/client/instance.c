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
    uint32_t map_id, uint32_t world_id, uint32_t player_id)
{
    LogDebug("start_loading_new_zone {map_id: %lu, player_id: %lu}", map_id, player_id);

    if (!NetConn_IsShutdown(&client->game_srv)) {
        // @Remark:
        // GameSrv_Disconnect will set the state to "AwaitGameServerDisconnect".
        GameSrv_Disconnect(client);
    } else {
        // @Remark:
        // If we don't disconnect, we just wait for the Game Server Transfer.
        client->state = AwaitGameServerTransfer;
    }

    client->server_transfer.map_id = map_id;
    client->server_transfer.world_id = world_id;
    client->server_transfer.player_id = player_id;
    client->server_transfer.host = *host;
}

void TransferGameServer(GwClient *client)
{
    assert(NetConn_IsShutdown(&client->game_srv));
    assert(client->state == AwaitGameServerTransfer);

    Character *cc = client->current_character;
    GameServerTransfer *transfer = &client->server_transfer;

    // @Cleanup:
    // We might need to reset all the world state here.
    init_world(&client->world, transfer->world_id);
    client->inventory.gold_character = 0;
    client->inventory.gold_storage = 0;
    client->game_srv.host = transfer->host;

    if (!GameSrv_Connect(&client->game_srv, &client->uuid, &cc->uuid,
        transfer->world_id, transfer->player_id, transfer->map_id)) {

        LogError("Game handshake failed !");
        reset_world(&client->world);
        NetConn_Reset(&client->game_srv);
        client->state = AwaitNothing;
        return;
    }
}

void HandleGameServerInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t request;
        uint32_t world_id;
        uint32_t map_id;
        uint8_t host[24];
        uint32_t player_id;
    } GameServerInfo;
#pragma pack(pop)

    assert(packet->header == AUTH_SMSG_GAME_SERVER_INFO);
    assert(sizeof(GameServerInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    GameServerInfo *pack = cast(GameServerInfo *)packet;
    assert(client);

    assert(client->state == AwaitGameServerInfo);

    struct sockaddr host;
    memcpy(&host, pack->host, sizeof(host));
    start_loading_new_zone(client, &host, pack->map_id, pack->world_id, pack->player_id);
}

void HandleGameTransferInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t host[24];
        uint32_t world_id;
        int8_t  region;
        uint16_t map_id;
        uint8_t  is_explorable;
        uint32_t player_id;
    } ServerInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRANSFER_GAME_SERVER_INFO);
    assert(sizeof(ServerInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ServerInfo *pack = cast(ServerInfo *)packet;
    assert(client && client->game_srv.secured);

    client->world.region = pack->region;
    struct sockaddr host;
    memcpy(&host, pack->host, sizeof(host));
    start_loading_new_zone(client, &host, pack->map_id, pack->world_id, pack->player_id);

    Event event;
    Event_Init(&event, EventType_WorldMapLeave);
    broadcast_event(&client->event_mgr, &event);
}

void HandleInstanceCountdownStop(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_INSTANCE_COUNTDOWN_STOP);
    assert(sizeof(Packet) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

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
    assert(client && client->game_srv.secured);

    World *world = &client->world;

    world->pvp_timer_start = world->world_time;
    world->pvp_timer_duration = pack->time;
}

void GameSrv_TravelGH(GwClient *client, const struct uuid *guild_uuid)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t uuid[16];
        int8_t unk1;
    } TravelHall;
#pragma pack(pop)
    
    assert(client && client->game_srv.secured);
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

    assert(client && client->game_srv.secured);
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

void GameSrv_Travel(GwClient *client, uint16_t map_id, District district, uint16_t district_number)
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

    assert(client && client->game_srv.secured);
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
    assert(client && client->game_srv.secured);
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
    assert(client && client->game_srv.secured);

    client->ingame = true;
}

void HandleInstanceLoadFinish(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_FINISH);
    assert(sizeof(Packet) == psize);
    
    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    client->loading = false;

    Event event;
    Event_Init(&event, EventType_WorldMapEnter);
    broadcast_event(&client->event_mgr, &event);
}

void GameSrv_ReturnToOutpost(GwClient *client)
{
    assert(client && client->game_srv.secured);
    begin_travel(client);
    
    Packet packet = NewPacket(GAME_CMSG_PARTY_RETURN_TO_OUTPOST);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
