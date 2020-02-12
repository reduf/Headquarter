typedef enum AgentType {
    AgentType_Unknow    = 0,
    AgentType_Living    = 1,
    AgentType_Gadget    = 2,
    AgentType_Inanimate = 3,
    AgentType_Item      = 4,
} AgentType;

typedef enum AgentMarker {
    AgentMarker_None            = 0,
    AgentMarker_Challenge       = 1,
    AgentMarker_MissionNf       = 2,
    AgentMarker_Reward1         = 3,
    AgentMarker_Reward2         = 4,
    AgentMarker_Quest           = 5,
    AgentMarker_MissionEotn     = 6,
    AgentMarker_Dungeon         = 7,
} AgentMarker;

typedef enum AgentEffect {
    AgentEffect_None            = 0x0000,
    AgentEffect_Bleeding        = 0x0001,
    AgentEffect_Condition       = 0x0002,
    AgentEffect_ExploitedCorpse = 0x0004,
    AgentEffect_Crippled        = 0x0008,
    AgentEffect_Dead            = 0x0010,
    AgentEffect_Wounded         = 0x0020,
    AgentEffect_Poisoned        = 0x0040,
    AgentEffect_Enchanted       = 0x0080,
    AgentEffect_FreezePlayer    = 0x0100,
    AgentEffect_GameMaster      = 0x0200,
    AgentEffect_DegenHexed      = 0x0400,
    AgentEffect_Hexed           = 0x0800,
    AgentEffect_Spawn           = 0x1000,
    AgentEffect_Sit1            = 0x2000,
    AgentEffect_Sit2            = 0x4000,
    AgentEffect_WeaponSpell     = 0x8000,
} AgentEffect;

typedef enum AgentAllegiance {
    AgentAllegiance_None    = 0,
    AgentAllegiance_Ally    = 1,
    AgentAllegiance_Enemy   = 2,
} AgentAllegiance;

typedef struct ApiAgent {
    AgentId         agent_id;
    AgentType       type;
    AgentAllegiance allegiance;
    uint32_t        effects;

    Vec2f           position;
    Vec2f           velocity;
    float           rotation;
} ApiAgent;
typedef array(ApiAgent) ArrayApiAgent;
