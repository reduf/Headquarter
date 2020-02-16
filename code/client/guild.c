#ifdef CORE_GUILD_C
#error "guild.c included more than once"
#endif
#define CORE_GUILD_C

Guild *get_guild_safe(GwClient *client, uint32_t guild_id)
{
    if (!(client && client->state.ingame && client->world.hash))
        return NULL;
    ArrayGuild guilds = client->world.guilds;
    if (!array_inside(guilds, guild_id))
        return NULL;
    Guild *guild = &array_at(guilds, guild_id);
    if (guild->guild_id != guild_id)
        return NULL;
    return guild;
}

void HandleGuildPlayerRole(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int16_t guild_id;
        uuid_t  guild_uuid;
        uint8_t member_type;
    } PlayerRole;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_PLAYER_ROLE);
    assert(sizeof(PlayerRole) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PlayerRole *pack = cast(PlayerRole *)packet;
    assert(client && client->game_srv.secured);

    ArrayGuild *guilds = &client->world.guilds;
    if (!array_inside(*guilds, pack->guild_id)) {
        LogError("The player guild (id: %d) is out of bound of the guild array (size: %d)",
            pack->guild_id, guilds->size);
        return;
    }

    Guild *guild = &array_at(*guilds, pack->guild_id);
    if (client->player) client->player->guild = guild;
}

void HandleGuildGeneralInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t guild_id;
        uuid_t uuid;
        uint16_t name[32];
        uint16_t tag[6];
        uint8_t feature;
        uint8_t territory;
        uint8_t cape_bg_color;
        uint8_t cape_pattern_color;
        uint8_t cape_emblem_color;
        uint8_t cape_shape;
        uint8_t cape_pattern;
        int16_t cape_emblem;
        uint8_t cape_trim;
        uint8_t allegiance;
        int32_t guild_faction;
        int32_t qualifier_pts;
        int32_t rating;
        int32_t rank;
        uint8_t unk1;
    } GuildInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_GENERAL_INFO);
    assert(sizeof(GuildInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    GuildInfo *pack = cast(GuildInfo *)packet;
    assert(client && client->game_srv.secured);

    ArrayGuild *guilds = &client->world.guilds;
    if ((size_t)pack->guild_id >= guilds->size) {
        array_grow_to(*guilds, pack->guild_id + 1);
        guilds->size = guilds->capacity;
    }

    Guild *guild = &array_at(*guilds, pack->guild_id);
    uuid_copy(guild->guild_uuid, pack->uuid);
    guild->guild_id = pack->guild_id;

    if (pack->allegiance < FactionType_Count) {
        guild->allegiance = (FactionType)pack->allegiance;
    } else {
        LogError("Guild allegiance doesn't fit in FactionType enum, val: %lu, max: %u",
            pack->allegiance, FactionType_Count);
    }

    guild->faction_pts = pack->guild_faction;
    // @Cleanup: Need utf8-support
    size_t written = unicode16_to_utf8(guild->name_buffer, sizeof(guild->name_buffer), pack->name, 32);
    guild->name.bytes = guild->name_buffer;
    guild->name.count = written - 1;
    if (!guild->name.count) {
        LogError("'HandleGuildGeneralInfo' missing utf-8 support");
    }

    written = unicode16_to_utf8(guild->tag_buffer, sizeof(guild->tag_buffer), pack->tag, 6);
    guild->tag.bytes = guild->tag_buffer;
    guild->tag.count = written;
    if (!guild->tag.count) {
        LogError("'HandleGuildGeneralInfo' missing utf-8 support");
    }
}

void HandleGuildChangeFaction(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t guild_id;
        uint8_t allegiance;
        int32_t guild_faction;
    } Payload;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_CHANGE_FACTION);
    assert(sizeof(Payload) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    Payload *pack = cast(Payload *)packet;
    assert(client && client->game_srv.secured);

    ArrayGuild *guilds = &client->world.guilds;
    if (!array_inside(*guilds, pack->guild_id)) {
        LogError("The guild (id: %d) is out of bound (size: %d)", pack->guild_id, guilds->size);
        return;
    }

    Guild *guild = &array_at(client->world.guilds, pack->guild_id);
    if (pack->allegiance < FactionType_Count) {
        guild->allegiance = (FactionType)pack->allegiance;
    } else {
        LogError("Guild allegiance doesn't fit in FactionType enum, val: %lu, max: %u",
            pack->allegiance, FactionType_Count);
    }
    guild->faction_pts = pack->guild_faction;
}

void HandleGuildInviteReceived(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t guild_uuid[16];
        uint16_t name[32];
        uint16_t tag[6];
        uint16_t inviter[20];
        int32_t date;
        int32_t minutes;
        int8_t type; // host=1, invite=?
    } GuildInvite;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_INVITE_RECEIVED);
    assert(sizeof(GuildInvite) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    GuildInvite *pack = cast(GuildInvite *)packet;
    assert(client && client->game_srv.secured);

    LogInfo("%S sent you a guild invite '%S [%S]' of type '%d'", pack->inviter, pack->name,
        pack->tag, pack->type);
}
