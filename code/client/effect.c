#ifdef CORE_EFFECT_C
#error "effect.c included more than once"
#endif
#define CORE_EFFECT_C


static size_t array_find_effect_index(ArrayEffect *effects, uint32_t effect_id)
{
    for (size_t i = 0; i < effects->size; i++) {
        if (effects->data[i].effect_id == effect_id)
            return i;
    }
    return array_npos;
}

static Effect *array_find_effect(ArrayEffect *effects, uint32_t effect_id)
{
    size_t index = array_find_effect_index(effects, effect_id);
    if (index == array_npos)
        return NULL;
    return &array_at(effects, index);
}

void HandleEffectUpkeepAdded(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId target_id;
        AgentId agent_id;
        int16_t skill_id;
        int32_t effect_type;
        int32_t effect_id;
    } EffectAdded;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_UPKEEP_ADDED);
    assert(sizeof(EffectAdded) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleEffectUpkeepRemoved(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        int32_t effect_id;
    } EffectRemoved;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_UPKEEP_REMOVED);
    assert(sizeof(EffectRemoved) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleEffectUpkeepApplied(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId target_id;
        AgentId agent_id;
        int16_t skill_id;
        int32_t effect_type;
        int32_t effect_id;
    } EffectApplied;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_UPKEEP_APPLIED);
    assert(sizeof(EffectApplied) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleEffectApplied(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        AgentId  agent_id;
        uint16_t skill_id;
        uint32_t effect_type;
        uint32_t effect_id;
        uint32_t duration; // is a float
    } EffectApplied;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_APPLIED);
    assert(sizeof(EffectApplied) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    EffectApplied *pack = cast(EffectApplied *)packet;
    assert(client && client->game_srv.secured);

    ArrayEffect *effects = &client->world.effects;
    Effect *effect = array_push(effects, 1);

    effect->timestamp = client->world.world_time;
    effect->effect_id = pack->effect_id;
    effect->skill_id  = pack->skill_id;
    effect->type      = pack->effect_type;
    effect->agent_id  = pack->agent_id;
    memcpy(&effect->duration, &pack->duration, sizeof(effect->duration));

    effect->remaining = effect->duration;
}

void HandleEffectRenewed(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        AgentId  agent_id;
        uint32_t effect_type;
        uint32_t effect_id;
        uint32_t duration;
    } EffectRenewed;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_RENEWED);
    assert(sizeof(EffectRenewed) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    EffectRenewed *pack = cast(EffectRenewed *)packet;
    assert(client && client->game_srv.secured);

    Effect *effect = array_find_effect(&client->world.effects, pack->effect_id);
    effect->timestamp = client->world.world_time;
    effect->agent_id  = pack->agent_id;
    effect->type      = pack->effect_type;
    memcpy(&effect->duration, &pack->duration, sizeof(effect->duration));

    effect->remaining = effect->duration;
}

void HandleEffectRemoved(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        AgentId  agent_id;
        uint32_t effect_id;
    } EffectRemoved;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_EFFECT_REMOVED);
    assert(sizeof(EffectRemoved) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    EffectRemoved *pack = cast(EffectRemoved *)packet;
    assert(client && client->game_srv.secured);

    ArrayEffect *effects = &client->world.effects;
    size_t index = array_find_effect_index(effects, pack->effect_id);
    if (index == array_npos) {
        LogError("Didn't find effect_id: %u", pack->effect_id);
        return;
    }
    array_remove(effects, index);
}
