#ifdef CORE_EFFECT_H
#error "effect.h included more than once"
#endif
#define CORE_EFFECT_H

typedef struct Effect {
    uint32_t    effect_id; // sync id with the server
    uint32_t    skill_id;  // id defiened what is the effect
    uint32_t    type;      // 0=condition/shout, 8=stance, 11=maintained enchantment, 14=enchantment/nature ritual

    AgentId     agent_id;
    float       duration; // in sec
    msec_t      timestamp;

    float       remaining; // in sec
} Effect;

typedef struct Buff {
    int buff_id;
} Buff;

typedef array(Effect) ArrayEffect;
typedef array(Buff)   ArrayBuff;

void HandleEffectUpkeepAdded(Connection *conn, size_t psize, Packet *packet);
void HandleEffectUpkeepRemoved(Connection *conn, size_t psize, Packet *packet);
void HandleEffectUpkeepApplied(Connection *conn, size_t psize, Packet *packet);
void HandleEffectApplied(Connection *conn, size_t psize, Packet *packet);
void HandleEffectRenewed(Connection *conn, size_t psize, Packet *packet);
void HandleEffectRemoved(Connection *conn, size_t psize, Packet *packet);
