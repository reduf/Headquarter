#ifdef CORE_WORLD_C
#error "world.c included more than once"
#endif
#define CORE_WORLD_C

void init_world(World *world, uint32_t hash)
{
    memset(world, 0, sizeof(*world));

    array_resize(&world->agents, 64);
    array_resize(&world->bags, 32);
    array_resize(&world->effects, 32);
    array_resize(&world->guilds, 32);
    array_resize(&world->items, 1024);
    array_resize(&world->parties, 8);
    array_resize(&world->players, 32);
    array_resize(&world->quests, 32);
    array_resize(&world->titles, 64);

    init_guildmember_update(&world->guild_member_update);

    world->player_count = 0;
    world->hash = hash;
}

void free_world(World *world)
{
    Item **item;
    array_foreach(item, &world->items) {
        free_item(*item);
    }

    Agent **agent;
    array_foreach(agent, &world->agents) {
        free(*agent);
    }

    Player **player;
    array_foreach(player, &world->players) {
        free(*player);
    }

    Bag *bag;
    array_foreach(bag, &world->bags) {
        array_reset(&bag->items);
    }

    array_reset(&world->bags);
    array_reset(&world->items);
    array_reset(&world->guilds);
    array_reset(&world->quests);
    array_reset(&world->agents);
    array_reset(&world->parties);
    array_reset(&world->players);
    array_reset(&world->effects);
    array_reset(&world->tmp_merchant_items);
    array_reset(&world->tmp_merchant_prices);
    array_reset(&world->merchant_items);
    free_dialog_info(&world->dialog);
    free_trade_session(&world->trade_session);
}

void reset_world(World *world)
{
    free_world(world);
    init_world(world, 0);
}

void world_update(World *world, msec_t diff)
{
    float diff_sec = diff / 1000.f;
    
    Effect *effect;
    ArrayEffect *effects = &world->effects;
    array_foreach(effect, effects) {
        if (effect->remaining == 0.f)
            continue;
        effect->remaining -= diff_sec;
        if (effect->remaining < 0.f)
            effect->remaining = 0.f;
    }

    update_agents_position(&world->agents, diff);
}
