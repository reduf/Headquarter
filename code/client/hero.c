#ifdef CORE_HERO_C
#error "hero.c included more than once"
#endif
#define CORE_HERO_C

void GameSrv_HeroUseSkill(GwClient *client, AgentId hero_id,
    int skill_id, uint32_t flags, AgentId target_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId hero;
        uint32_t skill_id;
        uint32_t flags; // check echo
        AgentId target;
    } UseSkill;
#pragma pack(pop)

    assert(client && client->game_srv.secured);

    Agent *caster = get_agent_safe(client, hero_id);
    if (!caster) {
        LogError("Can't get hero agent '%d' when using skill %u", hero_id, skill_id);
        return;
    }

    Agent *target = get_agent_safe(client, target_id);
    Skillbar *sb = get_skillbar_safe(client, hero_id);
    skillbar_start_cast(sb, skill_id, caster, target);

    UseSkill packet = NewPacket(GAME_CMSG_HERO_USE_SKILL);
    packet.hero     = hero_id;
    packet.skill_id = skill_id;
    packet.flags    = flags;
    packet.target   = target_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_HeroLoadSkills(GwClient* client, uint32_t hero_index, uint32_t skill_ids[8]) {
    assert(client && client->game_srv.secured);

    ArrayPartyHero heroes = client->player->party->heroes;
    if (!array_inside(&heroes, hero_index))
        return;
    AgentId agent_id = heroes.data[hero_index].agent_id;

    GameSrv_LoadSkills(client, agent_id, skill_ids);
}

void GameSrv_HeroLoadAttributes(GwClient* client, uint32_t hero_index, ArrayAttribute attributes) {
    assert(client && client->game_srv.secured);

    ArrayPartyHero heroes = client->player->party->heroes;
    if (!array_inside(&heroes, hero_index))
        return;
    AgentId agent_id = heroes.data[hero_index].agent_id;

    GameSrv_LoadAttributes(client, agent_id, attributes);
}

void GameSrv_HeroChangeSecondary(GwClient* client, uint32_t hero_index, Profession profession) {
    assert(client && client->game_srv.secured);

    ArrayPartyHero heroes = client->player->party->heroes;
    if (!array_inside(&heroes, hero_index))
        return;
    AgentId agent_id = heroes.data[hero_index].agent_id;

    GameSrv_ChangeSecondary(client, agent_id, profession);
}

void GameSrv_HeroSetBehavior(GwClient *client, AgentId agent_id, HeroBehavior behavior)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint32_t behavior;
    } SetBehavior;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SetBehavior packet = NewPacket(GAME_CMSG_HERO_BEHAVIOR);
    packet.agent_id = agent_id;
    packet.behavior = behavior;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_HeroSkillToggle(GwClient *client, AgentId agent_id, int skill_slot)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        int32_t skill_slot;
    } SkillToggle;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SkillToggle packet = NewPacket(GAME_CMSG_HERO_SKILL_TOGGLE);
    packet.agent_id    = agent_id;
    packet.skill_slot  = skill_slot;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void HandleHeroBehavior(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        int32_t behavior;
    } HeroBehavior;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_HERO_BEHAVIOR);
    assert(sizeof(HeroBehavior) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}
