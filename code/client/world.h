#ifdef CORE_WORLD_H
#error "world.h included more than once"
#endif
#define CORE_WORLD_H

typedef struct SalvageSession {
    uint16_t    salvage_session_id;
    bool        is_open;

    uint32_t    n_upgrades;
    Item       *upgrades[3];
} SalvageSession;

typedef struct World {
    uint32_t            hash;

    BagArray            bags;
    ArrayItem           items;
    array_uint32_t      item_mod_structs;
    ArrayParty          parties;
    ArrayGuild          guilds;
    ArrayQuest          quests;
    ArrayAgent          agents;
    ArrayPlayer         players;
    ArrayEffect         effects;
    ArrayTitle          titles;
    Inventory           inventory;
    ArraySkillbar       skillbars;
    GuildMemberUpdate   guild_member_update;
    ArrayItem           tmp_merchant_items;
    array_uint32_t      tmp_merchant_prices;
    ArrayItem           merchant_items;
    AgentId             merchant_agent_id;
    AgentId             interact_with;
    DialogInfo          dialog;
    TradeSession        trade_session;
    SalvageSession      salvage_session;

    AgentId             player_agent_id;
    uint32_t            player_id;
    uint32_t            map_id;
    uint32_t            district;
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

void init_world(World *world, uint32_t hash);
void free_world(World *world);
void reset_world(World *world);
