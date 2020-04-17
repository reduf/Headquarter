typedef struct ApiGuildMember {
    uint16_t account_name[20];
    uint16_t player_name[20];
    uint8_t status;
    uint8_t type;
    uint64_t last_login_utc;
} ApiGuildMember;

typedef array(ApiGuildMember) ArrayApiGuildMember;

typedef struct ApiGuild {
    size_t          guild_id;
    uint16_t        tag[32];
    uint16_t        name[64];
} ApiGuild;
typedef array(ApiGuild) ArrayApiGuild;