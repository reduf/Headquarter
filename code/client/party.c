#ifdef CORE_PARTY_C
#error "party.c included more than once"
#endif
#define CORE_PARTY_C

Player *get_player_safe(GwClient *client, uint32_t player_id)
{
    if (!(client && client->ingame && client->world.hash))
        return NULL;

    ArrayPlayer *players = &client->world.players;
    if (!array_inside(players, player_id))
        return NULL;
    return array_at(players, player_id);
}

Party *get_party_safe(GwClient *client, uint32_t party_id)
{
    if (!(client && client->ingame && client->world.hash))
        return NULL;
    ArrayParty *parties = &client->world.parties;
    if (!array_inside(parties, party_id))
        return NULL;
    return &array_at(parties, party_id);
}

PartyPlayer *get_party_player(Party *party, uint32_t player_id)
{
    assert(party);
    PartyPlayer *it;
    array_foreach(it, &party->players) {
        if (it->player_id == player_id)
            return it;
    }
    return NULL;
}

PartyHero *get_party_hero_agent(Party *party, AgentId agent_id)
{
    assert(party);
    PartyHero *it;
    array_foreach(it, &party->heroes) {
        if (it->agent_id == agent_id)
            return it;
    }
    return NULL;
}

void HandlePartySetDifficulty(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t mode;
    } SetMode;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SET_DIFFICULTY);
    assert(psize == sizeof(SetMode));

    GwClient *client = cast(GwClient *)conn->data;
    SetMode *pack = cast(SetMode *)packet;
    assert(client && client->game_srv.secured);

    if (!(client->player && client->player->party)) {
        LogError("The player is not int a party yet.");
        return;
    }

    Party *party = client->player->party;
    switch ((Difficulty)pack->mode) {
        case Difficulty_Normal:
        case Difficulty_Hard:
            break;
        default:
            LogError("Invalid 'Difficulty' value %hhu", pack->mode);
            return;
    }

    party->difficulty = (Difficulty)pack->mode;
}

void HandlePartyHeroAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
        uint16_t player_id; // owner
        uint16_t agent_id;
        uint8_t hero_id;
        uint8_t level;
    } HeroAdd;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_HERO_ADD);
    assert(psize == sizeof(HeroAdd));

    GwClient *client = cast(GwClient *)conn->data;
    HeroAdd *pack = cast(HeroAdd *)packet;
    assert(client && client->game_srv.secured);

    assert(array_inside(&client->world.parties, pack->party_id));
    Party *party = &array_at(&client->world.parties, pack->party_id);

    PartyHero *p_hero = array_push(&party->heroes, 1);
    if (!p_hero) {
        LogError("HandlePartyHeroAdd: Couldn't array_push");
        return;
    }

    p_hero->agent_id = pack->agent_id;
    p_hero->owner_id = pack->player_id;
    p_hero->hero_id  = pack->hero_id;
    p_hero->level    = pack->level;
}

void HandlePartyHeroRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
        uint16_t player_id;
        uint16_t agent_id;
    } HeroRemove;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_HERO_REMOVE);
    assert(psize == sizeof(HeroRemove));

    GwClient *client = cast(GwClient *)conn->data;
    HeroRemove *pack = cast(HeroRemove *)packet;
    assert(client && client->game_srv.secured);

    Party *party = get_party_safe(client, pack->party_id);
    if (!party) {
        LogError("Party %d doesn't exist.", pack->party_id);
        return;
    }

    PartyHero *p_hero = get_party_hero_agent(party, pack->agent_id);
    if (!p_hero) {
        LogError("Couldn't find hero '%d' from party '%d' (owner_id: %d)",
            pack->agent_id, pack->player_id, pack->agent_id);
        return;
    }

    // @Cleanup: Check if it's standard. (e.g. diff between 2 ptr of same type)
    size_t index = p_hero - party->heroes.data;
    array_remove_ordered(&party->heroes, index);
}

void HandlePartyInviteAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } InviteAdd;
#pragma pack(pop)

    // This packet is received when we send an invite and it is confirmed by the server.

    assert(packet->header == GAME_SMSG_PARTY_INVITE_ADD);
    assert(psize == sizeof(InviteAdd));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartyJoinRequest(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } JoinRequest;
#pragma pack(pop)

    // This packet is received when we receive an invite.
    assert(packet->header == GAME_SMSG_PARTY_JOIN_REQUEST);
    assert(psize == sizeof(JoinRequest));

    GwClient *client = cast(GwClient *)conn->data;
    JoinRequest *pack = cast(JoinRequest *)packet;
    assert(client && client->game_srv.secured);

    Party *party = get_party_safe(client, pack->party_id);
    if (party == NULL) {
        LogError("Received a party invite from unknow party. (party_id: %d)", pack->party_id);
        return;
    }

    Event event;
    Event_Init(&event, EventType_PartyInviteRequest);
    event.PartyInviteRequest.party_id = pack->party_id;
    broadcast_event(&client->event_mgr, &event);
}

void HandlePartyInviteCancel(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } InviteCancel;
#pragma pack(pop)

    // This packet is received when we cancel an invite and it is confirmed by the server.
    assert(packet->header == GAME_SMSG_PARTY_HERO_REMOVE);
    assert(psize == sizeof(InviteCancel));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartyRequestCancel(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } RequestCancel;
#pragma pack(pop)

    // This packet is received when we cancel an invite.

    assert(packet->header == GAME_SMSG_PARTY_REQUEST_CANCEL);
    assert(psize == sizeof(RequestCancel));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartyYouAreLeader(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint8_t is_loaded;
    } YouAreLeader;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_YOU_ARE_LEADER);
    assert(psize == sizeof(YouAreLeader));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    Event event;
    Event_Init(&event, EventType_PartyLeaderChanged);
    broadcast_event(&client->event_mgr, &event);
}

void HandlePartyPlayerAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
        uint16_t player_id;
        uint8_t is_loaded;
    } PlayerAdd;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_PLAYER_ADD);
    assert(psize == sizeof(PlayerAdd));

    GwClient *client = cast(GwClient *)conn->data;
    PlayerAdd *pack = cast(PlayerAdd *)packet;
    assert(client && client->game_srv.secured);

    Party *party = get_party_safe(client, pack->party_id);
    Player *player = get_player_safe(client, pack->player_id);
    assert(player && party);

    player->party = party;

    PartyPlayer *party_player = array_push(&party->players, 1);
    if (!party_player) {
        LogError("HandlePartyPlayerAdd: Couldn't array_push");
        return;
    }

    party_player->agent_id  = player->agent_id;
    party_player->player_id = pack->player_id;
    party_player->connected = pack->is_loaded == 1;
    party->player_count++;

    Event event;
    Event_Init(&event, EventType_PartyMembersChanged);
    event.PartyMembersChanged.party_id = pack->party_id;
    broadcast_event(&client->event_mgr, &event);
}

void HandlePartyPlayerRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
        uint16_t player_id;
    } PlayerRemove;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_PLAYER_REMOVE);
    assert(psize == sizeof(PlayerRemove));

    GwClient *client = cast(GwClient *)conn->data;
    PlayerRemove *pack = cast(PlayerRemove *)packet;
    assert(client && client->game_srv.secured);

    Party *party = get_party_safe(client, pack->party_id);
    Player *player = get_player_safe(client, pack->player_id);
    assert(player && party);

    size_t index = 0;
    PartyPlayer *it;
    array_foreach(it, &party->players) {
        if (it->player_id == player->player_id)
            break;
        index++;
    }

    array_remove_ordered(&party->players, index);
    player->party = NULL;
    party->player_count--;

    Event event;
    Event_Init(&event, EventType_PartyMembersChanged);
    event.PartyMembersChanged.party_id = pack->party_id;
    broadcast_event(&client->event_mgr, &event);
}

void HandlePartyPlayerReady(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
        uint16_t player_id;
        uint8_t  is_ready;
    } PlayerReady;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_PLAYER_READY);
    assert(psize == sizeof(PlayerReady));

    GwClient *client = cast(GwClient *)conn->data;
    PlayerReady *pack = cast(PlayerReady *)packet;
    assert(client && client->game_srv.secured);

    Player *player = get_player_safe(client, pack->player_id);
    assert(player && player->party);

    PartyPlayer *p_player = get_party_player(player->party, pack->player_id);
    assert(p_player);
    p_player->ready = true;
}

void HandlePartyCreate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } PartyInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_CREATE);
    assert(psize == sizeof(PartyInfo));

    GwClient *client = cast(GwClient *)conn->data;
    PartyInfo *pack = cast(PartyInfo *)packet;
    assert(client && client->game_srv.secured);

    ArrayParty *parties = &client->world.parties;
    if (!array_inside(parties, pack->party_id)) {
        array_resize(parties, pack->party_id + 1);
        parties->size = parties->capacity;
    }

    Party *party = &array_at(parties, pack->party_id);
    memzero(party, sizeof(*party));

    party->party_id = pack->party_id;
    array_init(&party->heroes);
    array_init(&party->players);
    array_init(&party->henchmans);
}

void HandlePartyMemberStreamEnd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } MemberStreamEnd;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_MEMBER_STREAM_END);
    assert(psize == sizeof(MemberStreamEnd));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartyDefeated(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_PARTY_DEFEATED);
    assert(psize == sizeof(Packet));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    if (!(client->player && client->player->party)) {
        LogError("Player party got defeated before it was created.");
        return;
    }

    client->player->party->defeated = true;
}

void GameSrv_FlagHero(GwClient *client, Vec2f pos, AgentId hero_agent_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        Vec2f   pos;
        int32_t plane; // need confirmation
    } FlagHero;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    FlagHero packet = NewPacket(GAME_CMSG_HERO_FLAG_SINGLE);
    packet.agent_id = hero_agent_id;
    packet.pos = pos;
    packet.plane = 0;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_FlagAllHero(GwClient *client, Vec2f pos)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        Vec2f  pos;
        int32_t plane; // need confirmation
    } FlagHero;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    FlagHero packet = NewPacket(GAME_CMSG_HERO_FLAG_ALL);
    packet.pos = pos;
    packet.plane = 0;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_AcceptInvite(GwClient *client, int party_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } AcceptInvite;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    AcceptInvite packet = NewPacket(GAME_CMSG_PARTY_ACCEPT_INVITE);
    packet.party_id = (int16_t)party_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LeaveParty(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_PARTY_LEAVE_GROUP);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_RefuseInvite(GwClient *client, int party_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t party_id;
    } RefuseInvite;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    RefuseInvite packet = NewPacket(GAME_CMSG_PARTY_ACCEPT_REFUSE);
    packet.party_id = (int16_t)party_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PartySetTick(GwClient *client, bool ticked)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t ticked;
    } SetTick;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SetTick packet = NewPacket(GAME_CMSG_PARTY_READY_STATUS);
    packet.ticked  = ticked;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SetDifficulty(GwClient *client, Difficulty mode)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t mode;
    } SetDifficulty;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SetDifficulty packet = NewPacket(GAME_CMSG_PARTY_SET_DIFFICULTY);
    packet.mode = mode;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_AddHero(GwClient *client, HeroID hero_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t hero_id;
    } HeroPacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    HeroPacket packet = NewPacket(GAME_CMSG_HERO_ADD);
    packet.hero_id = hero_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_KickHero(GwClient *client, HeroID hero_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t hero_id;
    } HeroPacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    HeroPacket packet = NewPacket(GAME_CMSG_HERO_KICK);
    packet.hero_id = hero_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

/*
 * Party Search
 */

void HandlePartySearchRequestJoin(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_from;
        uint16_t ps_host;
    } RequestJoin;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_REQUEST_JOIN);
    assert(sizeof(RequestJoin) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartySearchRequestDone(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_from;
        uint16_t ps_host;
    } RequestDone;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_REQUEST_DONE);
    assert(sizeof(RequestDone) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartySearchAdvertisement(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
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
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_ADVERTISEMENT);
    assert(sizeof(PacketType) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PacketType *pack = cast(PacketType *)packet;
    assert(client && client->game_srv.secured);

    Event params;
    Event_Init(&params, EventType_PartySearchAdvertisement);

    params.PartySearchAdvertisement.party_id = pack->ps_id;
    params.PartySearchAdvertisement.district = pack->district;
    params.PartySearchAdvertisement.party_size = pack->party_size;
    params.PartySearchAdvertisement.hero_count = pack->hero_count;
    params.PartySearchAdvertisement.search_type = pack->search_type;
    params.PartySearchAdvertisement.sender.buffer = pack->player_name;
    params.PartySearchAdvertisement.sender.length = u16len(pack->player_name, ARRAY_SIZE(pack->player_name));
    params.PartySearchAdvertisement.message.buffer = pack->message;
    params.PartySearchAdvertisement.message.length = u16len(pack->message, ARRAY_SIZE(pack->message));
    broadcast_event(&client->event_mgr, &params);
}

void HandlePartySearchSeek(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
    } SeekParty;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_SEEK);
    assert(sizeof(SeekParty) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartySearchRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
    } PacketType;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_REMOVE);
    assert(sizeof(PacketType) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartySearchSize(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
        uint8_t party_size;
        uint8_t hero_count;
    } PacketType;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_SIZE);
    assert(sizeof(PacketType) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandlePartySearchType(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
        uint8_t type; // 0=hunting, 1=mission, 2=quest, 3=trade, 4=guild
        uint8_t difficulty; // 0=normal, 1=hard
    } PacketType;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PARTY_SEARCH_TYPE);
    assert(sizeof(PacketType) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void GameSrv_PS_SeekParty(GwClient *client, PartySearchType type, struct kstr *msg)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t type;
        uint16_t msg[32];
        uint16_t unk2;
    } PacketSeekParty;
#pragma pack(pop)
    
    assert(client && client->game_srv.secured);
    PacketSeekParty packet = NewPacket(GAME_CMSG_PARTY_SEARCH_SEEK);
    packet.type = type;
    kstr_write(msg, packet.msg, ARRAY_SIZE(packet.msg));

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PS_CancelSeek(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_PARTY_SEARCH_CANCEL);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PS_RequestJoin(GwClient *client, uint16_t party_search_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
    } PacketType;
#pragma pack(pop)
    
    assert(client && client->game_srv.secured);
    PacketType packet = NewPacket(GAME_CMSG_PARTY_SEARCH_REQUEST_JOIN);
    packet.ps_id = party_search_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PS_RequestReply(GwClient *client, uint16_t party_search_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t ps_id;
    } PacketType;
#pragma pack(pop)
    
    assert(client && client->game_srv.secured);
    PacketType packet = NewPacket(GAME_CMSG_PARTY_SEARCH_REQUEST_REPLY);
    packet.ps_id = party_search_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_PS_ChangeType(GwClient *client, PartySearchType type)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t type;
    } PacketType;
#pragma pack(pop)
    
    assert(client && client->game_srv.secured);
    PacketType packet = NewPacket(GAME_CMSG_PARTY_SEARCH_TYPE);
    packet.type = type;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
