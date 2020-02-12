typedef struct ApiPlayer {
    uint32_t player_id;
    AgentId  agent_id;
    uint32_t guild_id;
} ApiPlayer;
typedef array(ApiPlayer) ArrayApiPlayer;