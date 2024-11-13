#ifdef CORE_PLAYER_C
#error "player.c included more than once"
#endif
#define CORE_PLAYER_C

void player_is_created(GwClient *client, AgentId agent_id)
{
    World *world = get_world_or_abort(client);
    world->player_agent_id = agent_id;

    assert(array_inside(&world->agents, agent_id));
    assert(array_at(&world->agents, agent_id));

    Agent* me = array_at(&world->agents, agent_id);
    if (!me->player_id)
        return;

    assert(array_inside(&world->players, me->player_id));
    Player *player = world->players.data[me->player_id];
    player->player_hero = client->player_hero; 
    world->player_id = player->player_id;
}

void HandlePlayerUpdateFactions(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t experience;
        int32_t kurzick_current;
        int32_t kurzick_total;
        int32_t luxon_current;
        int32_t luxon_total;
        int32_t imperial_current;
        int32_t imperial_total;
        int32_t data1;
        int32_t data2;
        int32_t level;
        int32_t morale;
        int32_t balthazar_current;
        int32_t balthazar_total;
        int32_t skill_current;
        int32_t skill_total;
    } PlayerAttr;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_UPDATE_FACTIONS);
    assert(sizeof(PlayerAttr) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PlayerAttr *pack = cast(PlayerAttr *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.experience = pack->experience;
    client->player_hero.level = pack->level;
    // client->morale = pack->morale;

    client->player_hero.skillpoint_current = pack->skill_current;
    client->player_hero.skillpoint_spent = pack->skill_total - pack->skill_current;
    client->player_hero.skillpoint_total = pack->skill_total;

    client->player_hero.imperial.total    = pack->imperial_total;
    client->player_hero.imperial.current  = pack->imperial_current;
    client->player_hero.balthazar.total   = pack->balthazar_total;
    client->player_hero.balthazar.current = pack->balthazar_current;
    client->player_hero.kurzick.total     = pack->kurzick_total;
    client->player_hero.kurzick.current   = pack->kurzick_current;
    client->player_hero.luxon.total       = pack->luxon_total;
    client->player_hero.luxon.current     = pack->luxon_current;
}

void HandlePlayerFactionMaxKurzick(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_FACTION_MAX_KURZICK);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.kurzick.max = pack->value;
}

void HandlePlayerFactionMaxLuxon(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_FACTION_MAX_LUXON);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.luxon.max = pack->value;
}

void HandlePlayerFactionMaxBalthazar(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_FACTION_MAX_BALTHAZAR);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.balthazar.max = pack->value;
}

void HandlePlayerFactionMaxImperial(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_PLAYER_FACTION_MAX_IMPERIAL);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.imperial.max = pack->value;
}

void HandlePlayerFactionUpdate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t attr_id;
        int32_t value;
    } UpdateAttr;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_FACTION_UPDATE);
    assert(sizeof(UpdateAttr) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateAttr *pack = cast(UpdateAttr *)packet;
    assert(client && client->game_srv.secured);

    PlayerHero *hero = &client->player_hero;

    switch (pack->attr_id) {
        case  0: hero->experience           += pack->value; break;
        case  1: hero->kurzick.current      += pack->value; break;
        case  2: hero->kurzick.total        += pack->value; break;
        case  3: hero->luxon.current        += pack->value; break;
        case  4: hero->luxon.total          += pack->value; break;
        case  5: hero->imperial.current     += pack->value; break;
        case  6: hero->imperial.total       += pack->value; break;
        // case  7:
        // case  8:
        // case  9:
        case 10: break; // @Cleanup: received, but always 0

        // @Cleanup: Need confirmation if those are also += & -=
        case 11: hero->balthazar.current    += pack->value; break;
        case 12: hero->balthazar.total      += pack->value; break;
        case 13: hero->skillpoint_current   += pack->value; break;
        case 14: hero->skillpoint_total     += pack->value; break;

        default: LogError("Unknow player attribute id: %d, value: %d", pack->attr_id, pack->value);
    }
}

void HandlePlayerUnlockedAreas(Connection* conn, size_t psize, Packet* packet) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t missions_completed_size;
        uint32_t missions_completed_buffer[32];
        uint32_t unlocked2_size;
        uint32_t unlocked2[32];
        uint32_t unlocked3_size;
        uint32_t unlocked3[32];
        uint32_t unlocked4_size;
        uint32_t unlocked4[32];
        uint32_t maps_unlocked_size;
        uint32_t maps_unlocked_buf[32];
    } MapsUnlocked;
#pragma pack(pop)
    assert(packet->header == GAME_SMSG_MAPS_UNLOCKED);
    assert(sizeof(MapsUnlocked) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    MapsUnlocked* pack = cast(MapsUnlocked*)packet;
    assert(client && client->game_srv.secured);

    array_resize(&client->player_hero.maps_unlocked, pack->maps_unlocked_size);
    for (uint32_t i = 0; i < pack->maps_unlocked_size; i++) {
        array_set(&client->player_hero.maps_unlocked, i, pack->maps_unlocked_buf[i]);
    }
}

void GameSrv_DonateFaction(GwClient *client, FactionType faction, int amount)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t unk0;
        int8_t faction; // k=0, l=1
        int32_t amount;
    } DepositFaction;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    assert((faction == FactionType_Kurzick) || (faction == FactionType_Luxon));

    DepositFaction packet = NewPacket(GAME_CMSG_DEPOSIT_FACTION);
    packet.amount = (int32_t)amount;
    packet.faction = (int8_t)faction;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PlayerLoadSkills(GwClient* client, uint32_t* skill_ids) {
    assert(client && client->game_srv.secured);

    World *world = get_world_or_abort(client);
    Agent* agent = get_agent_safe(world, world->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", world->player_agent_id);
        return;
    }

    GameSrv_LoadSkills(client, agent->agent_id, skill_ids);
}

void GameSrv_PlayerLoadAttributes(GwClient* client, ArrayAttribute attributes)
{
    assert(client && client->game_srv.secured);

    World *world = get_world_or_abort(client);
    Agent* agent = get_agent_safe(world, world->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", world->player_agent_id);
        return;
    }

    GameSrv_LoadAttributes(client, agent->agent_id, attributes);
}

void GameSrv_PlayerChangeSecondary(GwClient* client, Profession profession)
{
    assert(client && client->game_srv.secured);

    World *world = get_world_or_abort(client);
    Agent* agent = get_agent_safe(world, world->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", world->player_agent_id);
        return;
    }

    GameSrv_ChangeSecondary(client, agent->agent_id, profession);
}
