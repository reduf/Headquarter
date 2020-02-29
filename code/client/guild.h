#ifdef CORE_GUILD_H
#error "guild.h included more than once"
#endif
#define CORE_GUILD_H

typedef struct Guild {
    size_t          guild_id;
    uuid_t          guild_uuid;

    FactionType     allegiance;
    uint32_t        faction_pts;

    struct kstr     tag;
    struct kstr     name;
    uint16_t        tag_buffer[32];
    uint16_t        name_buffer[64];
} Guild;
typedef array(Guild) ArrayGuild;

Guild *get_guild_safe(GwClient *client, uint32_t guild_id);
