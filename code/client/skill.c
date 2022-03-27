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
    array_foreach(sb, &client->world.skillbars) {
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
    if (sb == NULL) {
        sb = array_push(&client->world.skillbars, 1);
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

void GameSrv_Attack(GwClient* client, AgentId target_id, bool call_target) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId target;
        uint8_t call;
    } Attack;
#pragma pack(pop)

    assert(client&& client->game_srv.secured);

    Agent* player = get_agent_safe(client, client->player_agent_id);
    if (!player) {
        LogError("Can't get player agent '%d'", client->player_agent_id);
        return;
    }

    Agent* target = get_agent_safe(client, target_id);
    if (!target) {
        LogError("Can't get target agent '%d'", target_id);
        return;
    }

    Attack packet = NewPacket(GAME_CMSG_ATTACK_AGENT);
    packet.target = target_id;
    packet.call = call_target;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LoadSkills(GwClient* client, AgentId agent_id, uint32_t* skill_ids) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint32_t n_skill_ids;
        uint32_t skill_ids[8];
    } LoadSkills;
#pragma pack(pop)
    Agent* agent = get_agent_safe(client, agent_id);
    if (!agent) {
        LogError("Can't get agent '%d'", agent_id);
        return;
    }
    // @todo assert skill_ids are 8 elements - sizeof won't work because the array decays to a pointer, so we have to pass sizeof(skill_ids)?! I don't know C...

    LoadSkills packet = NewPacket(GAME_CMSG_SKILLBAR_LOAD);
    packet.agent_id = agent->agent_id;
    packet.n_skill_ids = 8;
    for (size_t i = 0; i < 8; i++) {
        packet.skill_ids[i] = skill_ids[i];
    }

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LoadAttributes(GwClient* client, AgentId agent_id, ArrayAttribute attributes) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint32_t n_attribute_ids;
        uint32_t attribute_ids[16];
        uint32_t n_attribute_values;
        uint32_t attribute_values[16];
    } LoadAttributes;
#pragma pack(pop)
    Agent* agent = get_agent_safe(client, agent_id);
    if (!agent) {
        LogError("Can't get agent '%d'", agent_id);
        return;
    }

    LoadAttributes packet = NewPacket(GAME_CMSG_ATTRIBUTE_LOAD);
    packet.agent_id = agent->agent_id;

    assert(0 <= attributes.size && attributes.size <= 16);

    packet.n_attribute_ids = attributes.size;
    packet.n_attribute_values = attributes.size;
    for (size_t i = 0; i < attributes.size; i++) {
        Attribute attribute = array_at(&attributes, i);
        packet.attribute_ids[i] = attribute.attribute_id;
        packet.attribute_values[i] = attribute.rank_base;
    }

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_ChangeSecondary(GwClient* client, AgentId agent_id, Profession profession) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
        uint8_t profession;
    } ChangeSecondary;
#pragma pack(pop)
    assert(client&& client->game_srv.secured);

    Agent* agent = get_agent_safe(client, agent_id);
    if (!agent) {
        LogError("Can't get agent '%d'", agent_id);
        return;
    }

    if (profession == Profession_None) {
        return;
    }

    ChangeSecondary packet = NewPacket(GAME_CMSG_CHANGE_SECOND_PROFESSION);
    packet.agent_id = agent->agent_id;
    packet.profession = profession;

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

    assert(packet->header == GAME_SMSG_HERO_SKILL_STATUS);
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
        uint32_t data[48];
    } UpdateAttribute;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_AGENT_UPDATE_ATTRIBUTE);
    assert(sizeof(UpdateAttribute) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateAttribute *pack = cast(UpdateAttribute *)packet;
    assert(client && client->game_srv.secured);

    ArraySkillbar *skillbars = &client->world.skillbars;
    Skillbar *sb;
    array_foreach(sb, skillbars) {
        if (sb->owner_agent_id == pack->agent_id)
            break;
    }

    if (sb == NULL) {
        LogError("HandleAgentUpdateAttribute: Skillbar not found for agent %d", pack->agent_id);
        return;
    }

    size_t   size = pack->n_data;
    uint32_t *data = pack->data;

    size_t count = size / 3;
    if ((3 * count) != size) {
        LogError("HandleAgentUpdateAttribute: Bad size");
        return;
    }

    Attribute *attrs = array_push(&sb->attributes, count);
    for (size_t i = 0; i < count; i++) {
        attrs[i].attribute_id = data[i + 0*count];
        attrs[i].rank_base    = data[i + 1*count];
        attrs[i].rank_bonus   = data[i + 2*count];
    }
}

static const char _Base64ToValue[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [0,   16)
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // [16,  32)
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // [32,  48)
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // [48,  64)
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // [64,  80)
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // [80,  96)
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // [96,  112)
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // [112, 128)
};

static void _WriteBits(int val, char* buff) {
    buff[0] = ((val >> 0) & 1);
    buff[1] = ((val >> 1) & 1);
    buff[2] = ((val >> 2) & 1);
    buff[3] = ((val >> 3) & 1);
    buff[4] = ((val >> 4) & 1);
    buff[5] = ((val >> 5) & 1);
}

static int _ReadBits(char** str, int n) {
    int val = 0;
    char* s = *str;
    for (int i = 0; i < n; i++)
        val |= (*s++ << i);
    *str = s;
    return val;
}

static SkillTemplate* template_decode(const char* temp) {
    const int SKILLS_MAX = 3431;
    const int ATTRIBUTE_MAX = 44;

    SkillTemplate* result = cast(SkillTemplate*)malloc(sizeof(*result));

    if (result == NULL) {
        LogDebug("result is NULL");
    }

    size_t len = strlen(temp);

    const size_t bufSize = 1024;
    assert((len * 6) < bufSize);

    char bitStr[1024];

    for (size_t i = 0; i < len; i++) {
        int numeric_value = _Base64ToValue[temp[i]];
        if (numeric_value == -1) {
            // @todo invalid base64 character;
            LogError("Invalid base64 character");
            return NULL;
        }
        _WriteBits(numeric_value, bitStr + (6 * i));
    }

    char* it = bitStr;
    char* end = bitStr + 6 * len;

    int header = _ReadBits(&it, 4);
    if (header != 0 && header != 14) {
        // @todo template header not valid
        LogError("template header invalid");
        return NULL;
    }
    if (header == 14) _ReadBits(&it, 4);

    int bits_per_prof = 2 * _ReadBits(&it, 2) + 4;
    int profPrimary = _ReadBits(&it, bits_per_prof);
    int profSecondary = _ReadBits(&it, bits_per_prof);

    if (profPrimary <= Profession_None || profPrimary >= Profession_Count) return NULL;
    if (profSecondary < Profession_None || profSecondary >= Profession_Count) return NULL;

    int attribute_count = _ReadBits(&it, 4);

    if (attribute_count >= 16) {
        // @todo too many attributes
        LogError("too many attributes");
        return NULL;
    }
    
    array_init(&result->attributes);
    array_resize(&result->attributes, attribute_count);

    result->attributes.size = attribute_count;
    int bits_per_attr = _ReadBits(&it, 4) + 4;
    for (int i = 0; i < attribute_count; i++) {
        int attribute_id = _ReadBits(&it, bits_per_attr);
        int attribute_value = _ReadBits(&it, 4);
        if (attribute_id > ATTRIBUTE_MAX) {
            // @todo attribute id is out of range
            LogError("attribute id out of range");
            return NULL;
        }
        Attribute attribute = array_at(&result->attributes, i);
        attribute.attribute_id = attribute_id;
        attribute.rank_base = attribute_value;
        array_set(&result->attributes, i, attribute);
    }

    int bits_per_skill = _ReadBits(&it, 4) + 8;
    for (int i = 0; i < 8; i++) {
        if (it + bits_per_skill > end) {
            result->skills[i] = 0;
        }
        else {
            int skill_id = _ReadBits(&it, bits_per_skill);
            if (skill_id > SKILLS_MAX) {
                // @todo skill id is out of range
                LogError("skill id out of range");
                return NULL;
            }
            result->skills[i] = skill_id;
        }
    }

    result->primary = profPrimary;
    result->secondary = profSecondary;

    return result;
}
