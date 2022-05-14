#ifdef CORE_GUILD_C
#error "guild.c included more than once"
#endif
#define CORE_GUILD_C

Guild *get_guild_safe(GwClient *client, uint32_t guild_id)
{
    if (!(client && client->ingame && client->world.hash))
        return NULL;
    ArrayGuild guilds = client->world.guilds;
    if (!array_inside(&guilds, guild_id))
        return NULL;
    Guild *guild = &array_at(&guilds, guild_id);
    if (guild->guild_id != guild_id)
        return NULL;
    return guild;
}

void init_guildmember_update(GwClient *client)
{
    GuildMemberUpdate *gmu = &client->guild_member_update;
    kstr_init(&gmu->player_name, gmu->player_name_buffer, 0, ARRAY_SIZE(gmu->player_name_buffer));
    reset_guildmember_update(client);
}

void calc_last_login(GuildMember *member, uint32_t minutes_since_login)
{
    uint64_t last_login_utc = get_time_since_epoch();
    if (minutes_since_login) {
        last_login_utc -= ((uint64_t)minutes_since_login * 60);
    }
    member->last_login_utc = last_login_utc;
}

GuildMember *complete_guildmember_update(GwClient *client, uint16_t *account_name, size_t capacity)
{
    GuildMemberUpdate* gmu = &client->guild_member_update;
    GuildMember* member = NULL;
    //if (!gmu->pending)
   //     goto leave;
    assert(gmu->pending && client->player->guild);
    
    size_t length = 0;
    for (length = 0; length < capacity && account_name[length]; length++) {}

    assert(length);

    struct kstr account_name_kstr;
    kstr_init(&account_name_kstr, account_name, length, capacity);
    
    array_foreach(member, &client->player->guild->members) {
        if (kstr_compare(&member->account_name, &account_name_kstr) != 0)
            continue;
        if (gmu->status >= 0) {
            // Status changed e.g. online
            member->status = gmu->status;
            if (member->status == 0)
                member->player_name.length = 0;
            else
                kstr_copy(&member->player_name, &gmu->player_name);
            calc_last_login(member, gmu->minutes_since_login);
            member->status = gmu->status;
        }
           
        if (gmu->member_type >= 0) {
            // Member type changed e.g. promoted
            member->member_type = gmu->member_type;
        }
        goto leave;
    }
    member = NULL;
leave:
    reset_guildmember_update(client);
    return member;
}

void reset_guildmember_update(GwClient *client)
{
    GuildMemberUpdate* gmu = &client->guild_member_update;
    gmu->pending = false;
    gmu->member_type = 0;
    gmu->status = 0;
    gmu->player_name.length = 0;
    gmu->minutes_since_login = 0;
}

void init_guild(Guild *guild)
{
    guild->guild_id = 0;
    uuid_clear(&guild->guild_uuid);
    guild->allegiance = FactionType_Unknow;
    guild->faction_pts = 0;

    kstr_init(&guild->tag, guild->tag_buffer, 0, ARRAY_SIZE(guild->tag_buffer));
    kstr_init(&guild->name, guild->name_buffer, 0, ARRAY_SIZE(guild->name_buffer));

    guild->members.capacity = 0;
}

void init_guildmember(GuildMember* member)
{
    //member->join_date = 0;
    member->status = 0;
    member->member_type = 0;
    member->last_login_utc = 0;

    kstr_init(&member->account_name, member->account_name_buffer, 0, ARRAY_SIZE(member->account_name_buffer));
    //kstr_init(&member->invited_by, member->invited_by_buffer, 0, ARRAY_SIZE(member->invited_by_buffer));
    kstr_init(&member->player_name, member->player_name_buffer, 0, ARRAY_SIZE(member->player_name_buffer));
}

void HandleGuildPlayerRole(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int16_t guild_id;
        uint8_t guild_uuid[16];
        uint8_t member_type;
    } PlayerRole;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_PLAYER_ROLE);
    assert(sizeof(PlayerRole) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PlayerRole *pack = cast(PlayerRole *)packet;
    assert(client && client->game_srv.secured);

    ArrayGuild *guilds = &client->world.guilds;
    if (!array_inside(guilds, pack->guild_id)) {
        LogError("The player guild (id: %d) is out of bound of the guild array (size: %d)",
            pack->guild_id, guilds->size);
        return;
    }

    Guild *guild = &array_at(guilds, pack->guild_id);
    if (client->player) client->player->guild = guild;
    if (pack->member_type > 2) {
        // We can see this guild's members - prepare the array to receive the feed.
        array_init(&guild->members);
    }
}

void HandleGuildChangePlayerStatus(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t minutes_since_login;
        uint32_t status;
    } GuildPlayerStatusChange;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_CHANGE_PLAYER_STATUS);
    // NOTE: Followed by GAME_SMSG_GUILD_PLAYER_CHANGE_SUBJECT
    assert(sizeof(GuildPlayerStatusChange) == psize);

    GwClient *client = cast(GwClient*)conn->data;
    GuildPlayerStatusChange* pack = cast(GuildPlayerStatusChange*)packet;
    assert(client&& client->game_srv.secured);
    //reset_guildmember_update(&client);
    client->guild_member_update.minutes_since_login = pack->minutes_since_login;
    client->guild_member_update.status = pack->status;
    client->guild_member_update.pending = true;
}

void HandleGuildChangePlayerType(Connection* conn, size_t psize, Packet* packet) {
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t member_type;
    } GuildPlayerTypeChange;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_CHANGE_PLAYER_TYPE);
    // NOTE: Followed by GAME_SMSG_GUILD_PLAYER_CHANGE_SUBJECT
    assert(sizeof(GuildPlayerTypeChange) == psize);

    GwClient *client = cast(GwClient*)conn->data;
    GuildPlayerTypeChange* pack = cast(GuildPlayerTypeChange*)packet;
    assert(client&& client->game_srv.secured);
    //reset_guildmember_update(&client);
    client->guild_member_update.member_type = pack->member_type;
    client->guild_member_update.pending = true;
}

void HandleGuildChangePlayerContext(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t context_date;
        uint16_t context[64];
    } GuildPlayerStatusChange;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_CHANGE_PLAYER_CONTEXT);
    assert(sizeof(GuildPlayerStatusChange) == psize);

    (void)conn;
    (void)packet;

    // NOTE: Followed by GAME_SMSG_GUILD_PLAYER_CHANGE_SUBJECT
    // Not used atm
}

void HandleGuildPlayerChangeComplete(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t account_name[20];
    } GuildPlayerStatusChange;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_PLAYER_CHANGE_COMPLETE);
    // Follows GAME_SMSG_GUILD_PLAYER_CHANGE_CONTEXT or GAME_SMSG_GUILD_PLAYER_CHANGE_STATUS
    assert(sizeof(GuildPlayerStatusChange) == psize);

    GwClient *client = cast(GwClient*)conn->data;
    GuildPlayerStatusChange* pack = cast(GuildPlayerStatusChange*)packet;
    assert(client&& client->game_srv.secured);

    GuildMember* member = complete_guildmember_update(client, pack->account_name, ARRAY_SIZE(pack->account_name));
    if (member && !list_empty(&client->event_mgr.callbacks[EventType_GuildMemberUpdated])) {
        Event event;
        Event_Init(&event, EventType_GuildMemberUpdated);
        api_make_guild_member(&event.GuildMemberUpdated.member, member);
        broadcast_event(&client->event_mgr, &event);
    }
}

void HandleGuildPlayerInfo(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t account_name[20];
        uint16_t player_name[20];
        uint16_t invited_by[20];
        uint16_t context_info[64];
        int32_t unk; // Something to do with promoted date?
        uint32_t minutes_since_login;
        int32_t join_date; // This is seconds, but since when? 1471089600 ?
        int32_t status;
        int8_t member_type;
    } GuildPlayerInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GUILD_PLAYER_INFO);
    assert(sizeof(GuildPlayerInfo) == psize);

    GwClient *client = cast(GwClient*)conn->data;
    GuildPlayerInfo* pack = cast(GuildPlayerInfo*)packet;

    //wchar_t invited_name[40]
    assert(client && client->game_srv.secured);
    assert(client->player->guild);
    GuildMember* member;
    GuildMember* found = NULL;
    struct kstr invname_kstr;
    size_t length;
    for (length = 0; length < sizeof(pack->account_name) && pack->account_name[length]; length++) {}
    kstr_init(&invname_kstr, pack->account_name, length, ARRAY_SIZE(pack->account_name));
    array_foreach(member, &client->player->guild->members) {
        if (found)
            break;
        if (kstr_compare(&invname_kstr, &member->account_name) != 0)
            break;
        found = member;
    }
    if (!found) {
        found = array_push(&client->player->guild->members,1);
        init_guildmember(found);
        //found->join_date = pack->join_date;
        kstr_copy(&found->account_name, &invname_kstr);
    }
    kstr_read(&found->player_name, pack->player_name, ARRAY_SIZE(pack->player_name));
    found->status = pack->status;
    found->member_type = pack->member_type;
    calc_last_login(found, pack->minutes_since_login);
    //found->last_login_date = 0;
    //LogInfo("Guild member added: %ls (%ls), unk %d, type %d, status %d", 
    //    found->account_name_buffer, found->player_name_buffer, pack->unk, found->member_type, found->status);
    if (!list_empty(&client->event_mgr.callbacks[EventType_GuildMemberUpdated])) {
        Event event;
        Event_Init(&event, EventType_GuildMemberUpdated);
        api_make_guild_member(&event.GuildMemberUpdated.member, found);
        broadcast_event(&client->event_mgr, &event);
    }
}

void HandleGuildGeneralInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t guild_id;
        uint8_t uuid[16];
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
        array_resize(guilds, pack->guild_id + 1);
        guilds->size = guilds->capacity;
    }

    Guild *guild = &array_at(guilds, pack->guild_id);
    init_guild(guild);

    uuid_dec_le(pack->uuid, &guild->guild_uuid);
    guild->guild_id = pack->guild_id;

    if (pack->allegiance < FactionType_Count) {
        guild->allegiance = (FactionType)pack->allegiance;
    } else {
        LogError("Guild allegiance doesn't fit in FactionType enum, val: %lu, max: %u",
            pack->allegiance, FactionType_Count);
    }

    guild->faction_pts = pack->guild_faction;

    kstr_read(&guild->tag, pack->tag, ARRAY_SIZE(pack->tag));
    kstr_read(&guild->name, pack->name, ARRAY_SIZE(pack->name));
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
    if (!array_inside(guilds, pack->guild_id)) {
        LogError("The guild (id: %d) is out of bound (size: %d)", pack->guild_id, guilds->size);
        return;
    }

    Guild *guild = &array_at(&client->world.guilds, pack->guild_id);
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
    assert(client && client->game_srv.secured);

    // @Cleanup:
    // This function is bugged on Linux, because %ls will be a pointer to wchar_t
    // and it's a pointer to uint16_t.
    // It will also make the custom client crash, so we disabled the log for now.
    //
    // LogInfo("%ls sent you a guild invite '%ls [%ls]' of type '%d'",
    //     pack->inviter, pack->name, pack->tag, pack->type);
}
