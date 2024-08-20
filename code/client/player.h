#ifdef CORE_PLAYER_H
#error "player.h included more than once"
#endif
#define CORE_PLAYER_H

typedef struct PlayerHero {
    FactionPoint imperial;
    FactionPoint balthazar;
    FactionPoint kurzick;
    FactionPoint luxon;

    int experience;
    int level;
    int skillpoint_current;
    int skillpoint_spent;
    int skillpoint_total;
    array_uint32_t maps_unlocked;
} PlayerHero;

typedef struct Player {
    uint32_t        player_id;
    AgentId         agent_id;
    struct kstr_hdr name;
    uint16_t        name_buffer[64];
    size_t          guild_id;
    size_t          party_id;
    PlayerHero      player_hero;

    // @Remark: to test if a profession is unlocked do ((1 << prof_id) & unlocked_profession) != 0
    int32_t unlocked_profession;
} Player;
typedef array(Player *) ArrayPlayer;

void api_make_player(ApiPlayer *dest, Player *src)
{
    dest->player_id = src->player_id;
    dest->agent_id  = src->agent_id;
    dest->guild_id  = src->guild_id;
}

void init_player(Player *player)
{
    kstr_hdr_init(&player->name, player->name_buffer, ARRAY_SIZE(player->name_buffer));
}
