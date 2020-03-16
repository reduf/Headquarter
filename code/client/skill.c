#ifdef CORE_SKILL_C
#error "skill.c included more than once"
#endif
#define CORE_SKILL_C

static Agent      *get_agent_safe(GwClient *client, AgentId id);
static inline void agent_set_casting(Agent *agent, Skill *casting);

static Skillbar *get_skillbar_safe(GwClient *client, AgentId agent_id)
{
    if (!(client && client->world.hash))
        return NULL;
    Skillbar *sb;
    array_foreach(sb, client->world.skillbars) {
        if (sb->owner_agent_id == agent_id)
            return sb;
    }
    return NULL;
}

static int get_skill_position(GwClient *client, AgentId agent_id, uint32_t skill_id)
{
    Skillbar *sb = get_skillbar_safe(client, agent_id);
    if (!sb) return -1;
    for (int i = 0; i < 8; i++) {
        if (sb->skills[i].skill_id == skill_id)
            return i;
    }
    return -1;
}

void HandleSkillbarUpdateSkill(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        int8_t  position;
        int16_t skill_id;
        int32_t unk1;
    } UpdateSkill;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILLBAR_UPDATE_SKILL);
    assert(sizeof(UpdateSkill) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateSkill *pack = cast(UpdateSkill *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }
    skillbar_set_skill_id(sb, pack->position, pack->skill_id);
}

void HandleSkillbarUpdate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint32_t n_skills;
        int32_t skills[8];
        uint32_t n_pvp_masks;
        int32_t pvp_masks[8];
        int8_t unk1;
    } SkillbarUpdate;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILLBAR_UPDATE);
    assert(sizeof(SkillbarUpdate) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillbarUpdate *pack = cast(SkillbarUpdate *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb != NULL) {
        LogError("The agent %d already had a skillbar.", pack->agent_id);
        return;
    } else {
        sb = array_push(client->world.skillbars, 1);
    }

    sb->owner_agent_id = pack->agent_id;
    
    for (size_t i = 0; i < pack->n_skills; i++)
        skillbar_set_skill_id(sb, i, pack->skills[i]);
}

void HandleSkillAddToWindowsData(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
    } WindowsData;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILL_ADD_TO_WINDOWS_DATA);
    assert(sizeof(WindowsData) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    WindowsData *pack = cast(WindowsData *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

void HandleSkillAddToWindowsEnd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
    } WindowsEnd;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILL_ADD_TO_WINDOWS_END);
    assert(sizeof(WindowsEnd) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    WindowsEnd *pack = cast(WindowsEnd *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

void HandleSkillActivated(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        int16_t skill_id;
        int32_t unk1;
    } SkillCancel;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILL_ACTIVATED);
    assert(sizeof(SkillCancel) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillCancel *pack = cast(SkillCancel *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    skillbar_done_cast(sb, pack->skill_id);
}

void HandleSkillActivate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        int16_t skill_id;
        int32_t unk1;
    } SkillActivate;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILL_ACTIVATE);
    assert(sizeof(SkillActivate) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillActivate *pack = cast(SkillActivate *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    Skill *skill = skillbar_get_skill_by_id(sb, pack->skill_id);
    if (!skill) {
        LogError("Skill %d not found in skillbar of agent %d", pack->skill_id, pack->agent_id);
        return;
    }

    skill->casting_confirmed = true;
}

void HandleSkillRecharge(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        int16_t skill_id;
        int32_t unk1;
        int32_t recharge_sec;
    } SkillRecharge;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_SKILL_RECHARGE);
    assert(sizeof(SkillRecharge) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillRecharge *pack = cast(SkillRecharge *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    Skill *skill = skillbar_get_skill_by_id(sb, pack->skill_id);
    if (!skill) {
        LogError("Skill %d not found in skillbar of agent %d", pack->skill_id, pack->agent_id);
        return;
    }

    skill->recharging = true;
}

void HandleSkillRecharged(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        int16_t skill_id;
        int32_t unk1;
    } SkillRecharged;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_SKILL_RECHARGED);
    assert(sizeof(SkillRecharged) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillRecharged *pack = cast(SkillRecharged *)packet;
    assert(client && client->game_srv.secured);

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    Skill *skill = skillbar_get_skill_by_id(sb, pack->skill_id);
    if (!skill) {
        LogError("Skill %d not found in skillbar of agent %d", pack->skill_id, pack->agent_id);
        return;
    }

    skill->recharging = false;
}

void GameSrv_UseSkill(GwClient *client, int skill_id, uint32_t flags, AgentId target_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t skill_id;
        int32_t flags; // check echo
        AgentId target;
        uint8_t unk1;
    } UseSkill;
#pragma pack(pop)

    assert(client && client->game_srv.secured);

    Agent *caster = get_agent_safe(client, client->player_agent_id);
    if (!caster) {
        LogError("Can't get player agent '%d'", client->player_agent_id);
        return;
    }

    Agent *target = get_agent_safe(client, target_id);
    Skillbar *sb = get_skillbar_safe(client, client->player_agent_id);
    skillbar_start_cast(sb, skill_id, caster, target);

    UseSkill packet = NewPacket(GAME_CMSG_USE_SKILL);
    packet.skill_id = skill_id;
    packet.flags    = flags;
    packet.target   = target_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void HandleHeroSkillStatus(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        uint8_t pos;
        uint8_t status; // 0=enable, 1=disable
    } SkillStatus;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_HERO_BEHAVIOR);
    assert(sizeof(SkillStatus) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillStatus *pack = cast(SkillStatus *)packet;
    assert(client && client->game_srv.secured);

    if (pack->pos < 0 && 7 < pack->pos) {
        LogError("Unvalid skill position %d", pack->pos);
        return;
    }

    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    sb->skills[pack->pos].disable = (pack->status == 1);
}

void HandleHeroSkillStatusBitmap(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        uint8_t bitmap;
    } SkillStatus;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_HERO_SKILL_STATUS_BITMAP);
    assert(sizeof(SkillStatus) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkillStatus *pack = cast(SkillStatus *)packet;
    assert(client && client->game_srv.secured);

    uint8_t bitmap = pack->bitmap;
    Skillbar *sb = get_skillbar_safe(client, pack->agent_id);
    if (sb == NULL) {
        LogError("The agent %d doesn't have a skillbar.", pack->agent_id);
        return;
    }

    for (int i = 0; i < 8; i++) {
        if (bitmap & 1)
            sb->skills[i].disable = true;
        bitmap = bitmap >> 1;
    }
}

void HandleTomeShowSkills(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t unk0;
        uint8_t  prof;
        uint8_t  elite; // 0=normal, 1=elite
    } TomeShowSkills;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TOME_SHOW_SKILLS);
    assert(sizeof(TomeShowSkills) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TomeShowSkills *pack = cast(TomeShowSkills *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

void HandleAgentUpdateAttribute(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint32_t n_data;
        int32_t data[48];
    } UpdateAttribute;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_AGENT_UPDATE_ATTRIBUTE);
    assert(sizeof(UpdateAttribute) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateAttribute *pack = cast(UpdateAttribute *)packet;
    assert(client && client->game_srv.secured);

    ArraySkillbar *skillbars = &client->world.skillbars;
    Skillbar *sb;
    array_foreach(sb, *skillbars) {
        if (sb->owner_agent_id == pack->agent_id)
            break;
    }

    if (sb == NULL) {
        LogError("HandleAgentUpdateAttribute: Skillbar not found for agent %d", pack->agent_id);
        return;
    }

    size_t   size = pack->n_data;
    int32_t *data = pack->data;

    size_t count = size / 3;
    if ((3 * count) != size) {
        LogError("HandleAgentUpdateAttribute: Bad size");
        return;
    }

    Attribute *attrs = array_push(sb->attributes, count);
    for (size_t i = 0; i < count; i++) {
        attrs[i].attribute_id = data[i + 0*count];
        attrs[i].rank_base    = data[i + 1*count];
        attrs[i].rank_bonus   = data[i + 2*count];
    }
}
