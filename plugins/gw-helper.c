#ifdef GW_HELPER_C_INC
#error "gw-helper.c is already included"
#endif
#define GW_HELPER_C_INC

#include <float.h> // FLT_EPSILON
#include <common/time.h>
#include <common/macro.h>

#include "async.h"

static int   irand(int min, int max);
static float frand(float min, float max);
static float dist2(Vec2f u, Vec2f v);
static bool  equ2f(Vec2f v1, Vec2f v2);
static Vec2f lerp2f(Vec2f a, Vec2f b, float t);

#define sec_to_ms(sec) (sec * 1000)

static bool is_dead(AgentId agent_id)
{
    AgentEffect effect = GetAgentEffects(agent_id);
    return (effect & AgentEffect_Dead) != 0;
}

static void get_all_agents(ArrayApiAgent *agents)
{
    array_clear(*agents);
    size_t count = GetAgents(NULL, 0);
    if (count) {
        array_reserve(*agents, count);
        agents->size = GetAgents(agents->data, agents->capacity);
    }
}

static bool go_to_npc(uint32_t agent_id)
{
    AsyncState_GotoNpc state;
    async_init_GotoNpc(&state, agent_id);

    AgentId my_id = GetMyAgentId();
    while (!async_completed(state)) {
        async_update_GotoNpc(&state);
        if (!GetIsIngame() || is_dead(my_id)) {
            async_reset_GotoNpc(&state);
            return false;
        }
        time_sleep_ms(1);
    }
    return true;
}


static uint32_t get_item_quote_deadlock(uint32_t model_id, bool is_sell, msec_t deadlock) {
    ApiItem merchant_items[1024];
    size_t merchant_items_length = GetMerchantItems(merchant_items, 1024);
    if (!merchant_items_length) {
        LogError("Invalid merchant item size of %d", merchant_items_length);
        return 0;
    }
    ApiItem* found_item = NULL;
    BagEnum bag;
    for (size_t j = 0; j < merchant_items_length && found_item == NULL; j++) {
        if (merchant_items[j].model_id != model_id)
            continue;
        bag = GetItemLocation(merchant_items[j].item_id, NULL);
        if (is_sell && bag == BagEnum_Invalid) {
            // Not sellable item
            continue;
        }
        if (!is_sell && bag != BagEnum_Invalid) {
            // Not buyable item
            continue;
        }
        found_item = &merchant_items[j];
    }
    if (!found_item) {
        LogError("Failed to find merchant item for model_id %d", model_id);
        return 0;
    }
    LogInfo("Fetching quote for item %d, %d", found_item->model_id, found_item->item_id);
    msec_t deadlock_break = time_get_ms() + deadlock;

    AsyncState_RequestQuote state;
    async_init_RequestQuote(&state, found_item->item_id);

    AgentId my_id = GetMyAgentId();
    while (state.info.price == 0) {
        async_update_RequestQuote(&state);
        if ((time_get_ms() >= deadlock_break) || !GetIsIngame() || is_dead(my_id)) {
            async_reset_RequestQuote(&state);
            return 0;
        }
        time_sleep_ms(1);
    }
    async_reset_RequestQuote(&state);
    //if (state.quote_item_id != found_item->item_id)
    //    return 0;
    return state.info.price;
}
static uint32_t get_sell_quote(uint32_t model_id) {
    return get_item_quote_deadlock(model_id, true, 10000);
}
static uint32_t get_buy_quote(uint32_t model_id) {
    return get_item_quote_deadlock(model_id, false, 10000);
}
static bool go_to_npc_deadlock(ApiAgent *agent, msec_t deadlock)
{
    // @Cleanup: This need to be rewritten
    msec_t deadlock_break = time_get_ms() + deadlock;

    AsyncState_GotoNpc state;
    async_init_GotoNpc(&state, agent->agent_id);

    AgentId my_id = GetMyAgentId();
    while (!async_completed(state)) {
        async_update_GotoNpc(&state);
        if ((time_get_ms() >= deadlock_break) || !GetIsIngame() || is_dead(my_id)) {
            async_reset_GotoNpc(&state);
            return false;
        }
        time_sleep_ms(1);
    }
    return true;
}

static bool move_wait_coarse(float x, float y, float randomness, float dist_to_reach)
{
    Vec2f dest;
    if (randomness != 0.f) {
        dest.x = x + frand(-randomness, randomness);
        dest.y = y + frand(-randomness, randomness);
    } else {
        dest.x = x;
        dest.y = y;
    }

    AsyncState_MoveTo state;
    async_init_MoveTo(&state, dest, dist_to_reach);

    AgentId my_id = GetMyAgentId();
    while (!async_completed(state)) {
        async_update_MoveTo(&state);
        if (!GetIsIngame() || is_dead(my_id)) {
            async_reset_MoveTo(&state);
            return false;
        }
    }

    return true;
}

static bool move_wait(float x, float y, float randomness)
{
    return move_wait_coarse(x, y, randomness, FLT_EPSILON);
}

static void move_rand(float x, float y, float randomness)
{
    if (randomness != 0.f) {
        x += frand(-randomness, randomness);
        y += frand(-randomness, randomness);
    }
    MoveToCoord(x, y);
}

static int irand(int min, int max)
{
    assert(0 < (max - min) < RAND_MAX);
    int range = max - min;
    return (rand() % range) + min;
}

static float frand(float min, float max)
{
    assert(0 < (max - min) < RAND_MAX);
    float range = max - min;
    float random = (float)rand() / RAND_MAX;
    return (random * range) + min;
}

static float dist2(Vec2f u, Vec2f v)
{
    float dx = u.x - v.x;
    float dy = u.y - v.y;
    return sqrtf((dx * dx) + (dy * dy));
}

static bool equ2f(Vec2f v1, Vec2f v2)
{
    if (v1.x != v2.x)
        return false;
    if (v1.y != v2.y)
        return false;
    return true;
}

static Vec2f lerp2f(Vec2f a, Vec2f b, float t)
{
    Vec2f pos;
    pos.x = ((1.f - t) * a.x) + (t * b.x);
    pos.y = ((1.f - t) * a.y) + (t * b.y);
    return pos;
}

static ApiAgent get_agent_with_model_id(int npc_id)
{
    ApiAgent res;
    res.agent_id = 0;
    if (!npc_id) return res;
    ArrayApiAgent agents = {0};
    get_all_agents(&agents);
    for (size_t i = 0; i < agents.size; i++) {
        ApiAgent *agent = &agents.data[i];
        if (agent->type != AgentType_Living)
            continue;
        uint32_t id = GetNpcIdOfAgent(agent->agent_id);
        if (id && (id == npc_id)) {
            res = *agent;
            break;
        }
    }
    array_reset(agents);
    return res;
}

static ApiAgent get_agent_with_model_id_deadlock(int model_id, msec_t deadlock)
{
    msec_t deadlock_break = time_get_ms() + deadlock;
    ApiAgent agent = get_agent_with_model_id(model_id);
    while (!agent.agent_id) {
        time_sleep_ms(16);
        if (time_get_ms() >= deadlock_break)
            break;
        if (!GetIsIngame())
            break;
        agent = get_agent_with_model_id(model_id);
    }
    return agent;
}

static int count_foes_in_range(float range, Vec2f pos, int npc_id)
{
    int count = 0;
    if (!npc_id) return 0;
    ApiAgent *agent;
    ArrayApiAgent agents = {0};
    get_all_agents(&agents);
    array_foreach(agent, agents) {
        if (agent->type != AgentType_Living) continue;
        if (agent->effects & AgentEffect_Dead) continue;
        uint32_t id = GetNpcIdOfAgent(agent->agent_id);
        if (id != npc_id) continue;
        if (dist2(pos, agent->position) > range)
            continue;
        count++;
    }
    return count;
}

static ArrayApiItem get_items_in_bag(BagEnum bag)
{
    ArrayApiItem items = {0};
    size_t count = GetBagItems(bag, NULL, 0);
    array_reserve(items, count);
    items.size = GetBagItems(bag, items.data, items.capacity);
    return items;
}

static size_t count_bag_free_slot(BagEnum bag)
{
    size_t capacity = GetBagCapacity(bag);
    ArrayApiItem items = get_items_in_bag(bag);
    size_t count = capacity - items.size;
    array_reset(items);
    return count;
}

static size_t count_inventory_free_slot(void)
{
    size_t free_slot = 0;
    free_slot += count_bag_free_slot(BagEnum_Backpack);
    free_slot += count_bag_free_slot(BagEnum_BeltPouch);
    free_slot += count_bag_free_slot(BagEnum_Bag1);
    free_slot += count_bag_free_slot(BagEnum_Bag2);
    return free_slot;
}

static int count_bag_item(BagEnum bag, int model_id)
{
    ArrayApiItem items = get_items_in_bag(bag);
    int count = 0;
    ApiItem *item;
    array_foreach(item, items) {
        if (item->model_id == model_id)
            count += item->quantity;
    }
    array_reset(items);
    return count;
}

static int count_inventory_item(int model_id)
{
    int count = 0;
    count += count_bag_item(BagEnum_Backpack, model_id);
    count += count_bag_item(BagEnum_BeltPouch, model_id);
    count += count_bag_item(BagEnum_Bag1, model_id);
    count += count_bag_item(BagEnum_Bag2, model_id);
    return count;
}

static bool get_item_with_model_id(uint32_t model_id, ApiItem *item)
{
    ApiItem items[20];
    size_t  n_items;
    for (BagEnum bag_id = BagEnum_Backpack; bag_id <= BagEnum_Bag2; bag_id++) {
        n_items = GetBagItems(bag_id, items, ARRAY_SIZE(items));
        for (size_t i = 0; i < n_items; i++) {
            if (items[i].model_id == model_id) {
                if (item) *item = items[i];
                return true;
            }
        }
    }
    return false;
}

static ArrayApiQuest get_all_quests(void)
{
    ArrayApiQuest quests = {0};
    size_t count = GetQuests(NULL, 0);
    if (!count) return quests;
    array_reserve(quests, count);
    quests.size = GetQuests(quests.data, quests.capacity);
    return quests;
}

static ApiQuest get_quest_with_id(int quest_id)
{
    ApiQuest quest;
    quest.quest_id = 0;
    ArrayApiQuest quests = get_all_quests();
    ApiQuest *it;
    array_foreach(it, quests) {
        if (it->quest_id == quest_id)
            return *it;
    }
    array_reset(quests);
    return quest;
}

static bool player_has_quest(int quest_id)
{
    ApiQuest quest = get_quest_with_id(quest_id);
    return (quest.quest_id != 0);
}

static bool wait_quest_deadlock(int quest_id, msec_t deadlock)
{
    msec_t deadlock_break = time_get_ms() + deadlock;
    while (!player_has_quest(quest_id)) {
        time_sleep_ms(16);
        if (time_get_ms() >= deadlock_break)
            return false;
    }
    return true;
}

static void ping_sleep(msec_t ms)
{
    time_sleep_ms((unsigned int)(GetPing() + ms));
}

#if 0
static void buy_rare_material_item(Item *item)
{
    if (!item->quote_price) return;

    TransactionInfo send_info = {0};
    TransactionInfo recv_info = {0};

    recv_info.item_count = 1;
    recv_info.item_ids[0] = item->item_id;
    recv_info.item_quants[0] = 1;

    // @TODO
    // BuyMaterials(TRANSACT_TYPE_TraderBuy, item->quote_price, &send_info, 0, &recv_info);
}
#endif

static int get_rare_material_trader_id(int map_id)
{
    switch(map_id) {
    case 4:
    case 5:
    case 6:
    case 52:
    case 176:
    case 177:
    case 178:
    case 179:
        return 205;
    case 275:
    case 276:
    case 359:
    case 360:
    case 529:
    case 530:
    case 537:
    case 538:
        return 192;
    case 109:
        return 1997;
    case 193:
        return 3621;
    case 194:
    case 250:
    case 857:
        return 3282;
    case 242:
        return 3281;
    case 376:
        return 5388;
    case 398:
    case 433:
        return 5667;
    case 414:
        return 5668;
    case 424:
        return 5387;
    case 438:
        return 5613;
    case 49:
        return 2038;
    case 491:
    case 818:
        return 4723;
    case 449:
        return 4729;
    case 492:
        return 4722;
    case 638:
        return 6760;
    case 640:
        return 6759;
    case 641:
        return 6060;
    case 642:
        return 6045;
    case 643:
        return 6386;
    case 644:
        return 6385;
    case 652:
        return 6228;
    case 77:
        return 3410;
    case 81:
        return 2083;
    default:
        return 0;
    }
}

static bool wait_map_loading(int map_id);
static bool travel_wait(int map_id, District district, int district_number)
{
    if ((GetMapId() == map_id) && (GetDistrict() == district)
        && (GetDistrictNumber() == district_number)) {
        return true;
    }
    Travel(map_id, district, district_number);
    return wait_map_loading(map_id);
}

static bool random_travel(int map_id)
{
    District districts[] = {
        DISTRICT_ASIA_KOREAN,
        DISTRICT_ASIA_CHINESE,
        DISTRICT_ASIA_JAPANESE
    };

    int r = irand(0, ARRAY_SIZE(districts));
    District district = districts[r];
    Travel(map_id, district, 0);
    return wait_map_loading(map_id);
}

static bool travel_gh_safe(void)
{
    // @Enhancement: Theoricly, they could add a gh, but yeah...
    static int gh_map_ids[] = {
        4, 5, 6, 52, 176, 177, 178, 179, 275, 276, 359, 360, 529, 530, 537, 538
    };
    int map_id = GetMapId();
    for (int i = 0; i < ARRAY_SIZE(gh_map_ids); i++) {
        if (map_id == gh_map_ids[i])
            return true;
    }
    uint32_t guild_id = GetMyGuildId();
    TravelHall(guild_id);
    return wait_map_loading(0);
}

static ArrayApiPlayer get_all_players(void)
{
    ArrayApiPlayer players = {0};
    size_t count = GetPlayers(NULL, 0);
    if (!count) return players;
    array_reserve(players, count);
    players.size = GetPlayers(players.data, players.capacity);
    return players;
}

static bool get_player_with_name(const char *name, ApiPlayer *player)
{
    size_t length;
    char buffer[64];
    ArrayApiPlayer players = get_all_players();

    ApiPlayer *it;
    array_foreach(it, players) {
        length = GetPlayerName(it->player_id, buffer, sizeof(buffer));
        if (!strncmp(name, buffer, length)) {
            if (player) *player = *it;
            array_reset(players);
            return true;
        }
    }

    array_reset(players);
    return false;
}

static size_t get_skill_position(int skill_id)
{
    uint32_t skill_ids[8] = {0};
    GetSkillbar(skill_ids, 0);
    for (size_t i = 0; i < 8; i++) {
        if (skill_ids[i] == skill_id)
            return i + 1;
    }
    return 0;
}

static bool use_skill_wait(int skill_id, AgentId target, msec_t deadlock)
{
    msec_t deadlock_break = time_get_ms() + deadlock;
    size_t position = get_skill_position(skill_id);
    if (position == 0)
        return false;
    UseSkill(skill_id, target);
    ping_sleep(500); // We should set the skill as casting even if we didn't get the confirmation from the server
    while (GetSkillCasting(position, NULL)) {
        time_sleep_ms(16);
        if (deadlock && (time_get_ms() >= deadlock_break))
            return false;
        if (!GetIsIngame())
            return false;
    }
    return true;
}

static void set_hard_mode(void)
{
    if (GetDifficulty() != Difficulty_Hard)
        SetDifficulty(Difficulty_Hard);
}

static void set_normal_mode(void)
{
    if (GetDifficulty() != Difficulty_Normal)
        SetDifficulty(Difficulty_Normal);
}

static int32_t get_guild_faction(void)
{
    uint32_t guild_id = GetMyGuildId();
    if (!guild_id) return 0;
    return GetGuildFaction(guild_id, NULL);
}

static bool wait_map_loading(int map_id)
{
    AsyncState_WaitMapLoading state;
    async_init_WaitMapLoading(&state);

    while (!async_completed(state)) {
        async_update_WaitMapLoading(&state);
        time_sleep_ms(1);
    }
    // @Cleanup:
    // We shouldn't have that here.
    while (!GetMyAgentId()) {
        time_sleep_ms(16);
    }
    return (GetMapId() == map_id);
}
