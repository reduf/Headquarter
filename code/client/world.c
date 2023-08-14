#ifdef CORE_WORLD_C
#error "world.c included more than once"
#endif
#define CORE_WORLD_C

static void init_world(World *world, uint32_t hash)
{
    array_init(&world->agents);
    array_init(&world->bags);
    array_init(&world->effects);
    array_init(&world->guilds);
    array_init(&world->items);
    array_init(&world->parties);
    array_init(&world->players);
    array_init(&world->quests);

    array_resize(&world->agents, 64);
    array_resize(&world->bags, 32);
    array_resize(&world->effects, 32);
    array_resize(&world->guilds, 32);
    array_resize(&world->items, 1024);
    array_resize(&world->parties, 8);
    array_resize(&world->players, 32);
    array_resize(&world->quests, 32);

    // @Enhancement:
    // This would break if we had more than 8 skillbars.
    // We use pointers to element in a array of Skillbar.
    array_init(&world->skillbars);

    world->player_count = 0;
    world->hash = hash;
}

static void reset_world(World *world)
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

    world->player_count = 0;
    world->hash = 0;
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
