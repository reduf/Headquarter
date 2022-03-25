#ifdef CORE_PLAYER_C
#error "player.c included more than once"
#endif
#define CORE_PLAYER_C

void player_is_created(GwClient *client, AgentId agent_id)
{
    World *world = &client->world;
    client->player_agent_id = agent_id;

    assert(array_inside(&world->agents, agent_id));
    assert(array_at(&world->agents, agent_id));

    Agent* me = array_at(&world->agents, agent_id);
    if (!me->player_id)
        return;

    assert(array_inside(&world->players, me->player_id));
    client->player = array_at(&world->players, me->player_id);

    client->player->player_hero = &client->player_hero;
}

void HandlePlayerAttrSet(Connection *conn, size_t psize, Packet *packet)
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

    assert(packet->header == GAME_SMSG_PLAYER_ATTR_SET);
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

void HandlePlayerAttrMaxKurzick(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_ATTR_MAX_KURZICK);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.kurzick.max = pack->value;
}

void HandlePlayerAttrMaxLuxon(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_ATTR_MAX_LUXON);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.luxon.max = pack->value;
}

void HandlePlayerAttrMaxBalthazar(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.balthazar.max = pack->value;
}

void HandlePlayerAttrMaxImperial(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t value;
    } MaxFaction;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL);
    assert(sizeof(MaxFaction) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    MaxFaction *pack = cast(MaxFaction *)packet;
    assert(client && client->game_srv.secured);

    client->player_hero.imperial.max = pack->value;
}

void HandlePlayerAttrUpdate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t attr_id;
        int32_t value;
    } UpdateAttr;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PLAYER_ATTR_UPDATE);
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

    Agent* agent = get_agent_safe(client, client->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", client->player_agent_id);
        return;
    }

    GameSrv_LoadSkills(client, agent->agent_id, skill_ids);
}

void GameSrv_PlayerLoadAttributes(GwClient* client, ArrayAttribute attributes)
{
    assert(client && client->game_srv.secured);

    Agent* agent = get_agent_safe(client, client->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", client->player_agent_id);
        return;
    }

    GameSrv_LoadAttributes(client, agent->agent_id, attributes);
}

void GameSrv_PlayerChangeSecondary(GwClient* client, Profession profession)
{
    assert(client && client->game_srv.secured);

    Agent* agent = get_agent_safe(client, client->player_agent_id);
    if (!agent) {
        LogError("Can't get player agent '%d'", client->player_agent_id);
        return;
    }

    GameSrv_ChangeSecondary(client, agent->agent_id, profession);
}
