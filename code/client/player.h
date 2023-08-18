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
} PlayerHero;

typedef struct Player {
    uint32_t    player_id;
    AgentId     agent_id;

    struct kstr_hdr name;
    uint16_t name_buffer[64];

    Guild      *guild;
    Party      *party;
    PlayerHero *player_hero;

    // @Remark: to test if a profession is unlocked do ((1 << prof_id) & unlocked_profession) != 0
    int32_t unlocked_profession;
} Player;
typedef array(Player *) ArrayPlayer;

static void api_make_player(ApiPlayer *dest, Player *src)
{
    dest->player_id = src->player_id;
    dest->agent_id  = src->agent_id;
    dest->guild_id  = src->guild ? src->guild->guild_id : 0;
}

static void init_player(Player *player)
{
    kstr_hdr_init(&player->name, player->name_buffer, ARRAY_SIZE(player->name_buffer));
}
