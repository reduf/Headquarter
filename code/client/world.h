#ifdef CORE_WORLD_H
#error "world.h included more than once"
#endif
#define CORE_WORLD_H

typedef struct World {
    uint32_t            hash;

    BagArray            bags;
    ArrayItem           items;
    array(uint32_t)     item_mod_structs;
    ArrayParty          parties;
    ArrayGuild          guilds;
    ArrayQuest          quests;
    ArrayAgent          agents;
    ArrayPlayer         players;
    ArrayEffect         effects;

    ArraySkillbar       skillbars;

    int32_t             map_id;
    int32_t             district;
    DistrictRegion      region;
    DistrictLanguage    language;

    size_t              player_count;

    size_t              objective_count;

    msec_t              world_time; // time updated at every frame
    msec_t              time_server; // time updated by server packet
    msec_t              load_time;

    msec_t              pvp_timer_start;
    msec_t              pvp_timer_duration;

    bool                in_cinematic;
    int                 cinematic_skip_count;
    int                 cinematic_member_count;
} World;

void world_update_after_input(World *world, msec_t diff);
void world_update_before_input(World *world, msec_t diff);

static void init_world(World *world, uint32_t hash);
static void reset_world(World *world);
