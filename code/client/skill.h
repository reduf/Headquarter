#ifdef CORE_SKILL_H
#error "skill.h included more than once"
#endif
#define CORE_SKILL_H

typedef struct Skill {
    uint32_t        skill_id;

    SkillInfo      *info;
    struct Agent   *caster;
    struct Agent   *target;

    bool            disable; // tell if hero skill is disabled (shift+click)

    bool            casting; // local only
    bool            casting_confirmed; // when we receive confirmation from server

    bool            recharging;
    msec_t          timestamp_cast;
    msec_t          timestamp_recast;
} Skill;

typedef struct Attribute {
    int32_t attribute_id;
    int32_t rank_base;
    int32_t rank_bonus; // with pcons, runes, etc.
} Attribute;
typedef array(Attribute) ArrayAttribute;

typedef struct Skillbar {
    // Agent          *owner;
    AgentId         owner_agent_id;
    ArrayAttribute  attributes;
    Profession      prof1;
    Profession      prof2;

    union {
        Skill skills[8];
        struct {
            Skill skill1;
            Skill skill2;
            Skill skill3;
            Skill skill4;
            Skill skill5;
            Skill skill6;
            Skill skill7;
            Skill skill8;
        };
    };
} Skillbar;
typedef array(Skillbar) ArraySkillbar;

#define skill_is_casting(s) ((s)->casting || (s)->casting_confirmed)

static inline void
skillbar_set_skill_id(Skillbar *sb, int pos, int skill_id)
{
    assert(0 <= pos && pos <= 7);
    memzero(&sb->skills[pos], sizeof(sb->skills[0]));
    sb->skills[pos].skill_id = skill_id;
}

static inline void
skillbar_set_skill(Skillbar *sb, int pos, Skill *skill)
{
    skillbar_set_skill_id(sb, pos, skill->skill_id);
}

static inline Skill *
skillbar_get_skill_by_id(Skillbar *sb, uint32_t skill_id)
{
    assert(sb);
    for (int i = 0; i < ARRAY_SIZE(sb->skills); i++)
        if (sb->skills[i].skill_id == skill_id)
            return &sb->skills[i];
    return NULL;
}

static inline bool
skillbar_skill_is_casting(Skillbar *sb, int pos)
{
    assert(sb && (1 <= pos) && (pos <= 8));
    Skill *skill = &sb->skills[pos];
    return (skill->casting || skill->casting_confirmed);
}

static inline void agent_set_casting(struct Agent *agent, Skill *casting);
static void skillbar_start_cast(Skillbar *sb, uint32_t skill_id, struct Agent *caster, struct Agent *target)
{
    assert(sb && sb->owner_agent_id);
    Skill *skill = skillbar_get_skill_by_id(sb, skill_id);
    if (skill == NULL) {
        LogError("Cann't find the skill '%d' in the skillbar of agent '%d'",
            skill_id, sb->owner_agent_id);
        return;
    }

    skill->casting = true;
    skill->caster = caster;
    skill->target = target;

    agent_set_casting(skill->caster, skill);
}

static void skillbar_done_cast(Skillbar *sb, uint32_t skill_id)
{
    assert(sb && sb->owner_agent_id);
    Skill *skill = skillbar_get_skill_by_id(sb, skill_id);
    if (skill == NULL) {
        LogError("Cann't find the skill '%d' in the skillbar of agent '%d'",
            skill_id, sb->owner_agent_id);
        return;
    }

    skill->casting = false;
    skill->casting_confirmed = false;

    if (skill->caster)
        agent_set_casting(skill->caster, NULL);
    skill->caster = NULL;
    skill->target = NULL;
}

static Skillbar *get_skillbar_safe(GwClient *client, AgentId agent_id);

static void skillbar_done_cast(Skillbar *sb,  uint32_t skill_id);
static void skillbar_start_cast(Skillbar *sb, uint32_t skill_id,
    struct Agent *caster, struct Agent *target);
