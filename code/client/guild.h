#ifdef CORE_GUILD_H
#error "guild.h included more than once"
#endif
#define CORE_GUILD_H

typedef struct Guild {
    size_t      guild_id;
    uuid_t      guild_uuid;

    FactionType allegiance;
    uint32_t    faction_pts;
    string      name;
    string      tag;

    char        name_buffer[64];
    char        tag_buffer[32];
} Guild;
typedef array(Guild) ArrayGuild;

Guild *get_guild_safe(struct GwClient *client, uint32_t guild_id);
