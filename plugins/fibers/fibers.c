#ifndef __STDC__
# define __STDC__ 1
#endif

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <Windows.h>

#include <common/time.h>
#include <common/thread.h>

#include <Headquarter.h>
#include "../gw-helper.c"

#define DllExport __declspec(dllexport)

#define MapID_House             77
#define MapID_Drazach           195
#define MapID_Anjeka            349

#define ModelID_KurBureaucrat   3410

#define ModeID_BreambelLong     868
#define ModeID_BreambelFlat     904
#define ModeID_BreambelHorn     906
#define ModeID_BreambelRecurve  934
#define ModeID_BreambelShort    957

#define ModelID_Amber           940
#define ModelID_Echovald        945
#define ModelID_Gothic          951
#define ModelID_Ornate          954
#define ModelID_DragonRoots     819
#define ModelID_DragonMoss      3718

#define SkillID_Whirling        450
#define SkillID_Winnowing       463
#define SkillID_QZ              475
#define SkillID_Return          770
#define SkillID_SF              826
#define SkillID_SoH             929
#define SkillID_Shroud          1031
#define SkillID_Dash            1043
#define SkillID_Storms          1474
#define SkillID_RadField        2414

static const char *Skillbar_Player  = "OgcTcZ88ZSgZCSn5AS4K0R4wtEA";
static const char *Skillbar_Hero    = "OwJSg5/NhuJ4255AAAAAAAA";
static msec_t      move_deadlock    = 15 * 1000;

static volatile bool    running;
static struct thread    bot_thread;
static void            *bot_module;

static HANDLE           mailslot;
static const char      *mailslot_name = "\\\\.\\mailslot\\DragonMossManager";

static char             charname[64];

static bool             resign_ready;
static int              runs_count;
static int              success_count;
static int              roots_count;


static void exit_with_status(const char *state, int exit_code);
static bool safe_move(float x, float y, float randomness);

static void manage_faction(void);
static void get_resign_ready(void);
static bool try_single_run(void);
static bool return_outpost();

static void pickup_loot(void);
static bool can_pickup(ApiItem *item);

static int main_bot(void *param)
{
    running = true;

    while (!travel_wait(MapID_Anjeka, DISTRICT_ASIA_KOREAN, 0)) {
        if (!GetIsIngame()) {
            exit_with_status("Error", 1);
        }
        time_sleep_sec(10);
    }

    SetDifficulty(Difficulty_Hard);

    while (running) {

        // manage_inventory();

        manage_faction();

        if (GetMapId() != MapID_Anjeka) {
            if (!random_travel(MapID_Anjeka))
                exit_with_status("Error/", 1);
            resign_ready = false;
        }

        if (!resign_ready) {
            time_sleep_ms(200); // @Temporary
            get_resign_ready();
            time_sleep_sec(3); // @Temporary
        }

        runs_count++;
        if (try_single_run()) {
            success_count++;
        } else {
            LogInfo("Failed run #%d", runs_count);
        }

        printf("\nStats:\n");
        printf("    Run:     %d\n", runs_count);
        printf("    Success: %d\n", success_count);
        printf("    Fails:   %d\n", runs_count - success_count);
        printf("    Roots:   %d\n\n", roots_count);
    }

    // CloseHandle(mailslot);
    FreePluginAndExitThread(bot_module, 0);
    return 0;
}

static bool safe_move(float x, float y, float randomness)
{
    Vec2f dest;
    dest.x = x + frand(-randomness, randomness);
    dest.y = y + frand(-randomness, randomness);

    AgentId my_id = GetMyAgentId();
    if (!my_id)
        return false;

    MoveToPoint(dest);
    msec_t deadlock_end = time_get_ms() + move_deadlock;

    Vec2f pos = GetAgentPos(my_id);
    // We want to quit when we enter the radius centered at the given point
    while (dist2(dest, pos) > FLT_EPSILON) {
        if (!GetIsIngame() || (time_get_ms() >= deadlock_end)) {
            return false;
        }
        time_sleep_ms(16);
        pos = GetAgentPos(my_id);
    }
    return true;
}

static void manage_faction(void)
{
    LogDebug("Checking faction");
    FactionPoint points = GetKurzickPoints();
    if (points.max - points.current < 500) {
        travel_gh_safe();

        ApiAgent kurzick = get_agent_with_model_id_deadlock(ModelID_KurBureaucrat, 5000);
        if (!kurzick.agent_id) {
            LogError("Kurzick Bureaucrat not found. (model_id: %d, map_id: %d)",
                ModelID_KurBureaucrat, GetMapId());
            return;
        }

        if (!go_to_npc_deadlock(&kurzick, 60 * 1000)) {
            LogError("Never reach the bureaucrat id: %d, pos: (%.2f, %.2f)",
                kurzick.agent_id, kurzick.position.x, kurzick.position.y);
            return;
        }

        int dep_count = points.current / 5000;
        for (int i = 0; i < dep_count; i++) {
            DonateFaction(FactionType_Kurzick);
            ping_sleep(500);
        }
    }
}

static void get_resign_ready(void)
{
    MoveToCoord(-11209.f, -23100.f);
    if (!wait_map_loading(MapID_Drazach))
        exit_with_status("Error/wait_map_loading(MapID_Drazach)", 1);
    MoveToCoord(-11229.f, 20150.f);
    if (!wait_map_loading(MapID_Anjeka))
        exit_with_status("Error/wait_map_loading(MapID_Anjeka)", 1);
    set_hard_mode();
    resign_ready = true;
}

static bool has_more_luxon(void)
{
    FactionPoint kur = GetKurzickPoints();
    FactionPoint lux = GetLuxonPoints();
    return (lux.current > kur.current);
}

static bool try_single_run(void)
{
    if (GetGoldCharacter() < 150) {
        LogDebug("Need gold");
        WithdrawGold(150);
        // sleep/wait_gold_change ?
    }

    LogDebug("Going outside");

    MoveToCoord(-11209.f, -23100.f);
    if (!wait_map_loading(MapID_Drazach)) {
        exit_with_status("Error/try_single_run", 1);
    }

    // time_sleep_ms(3000);
    AgentId player_id = GetMyAgentId();

    ApiAgent priest = get_agent_with_model_id_deadlock(3422, 5*1000);
    if (!priest.agent_id) {
        LogError("Kurzick priest never spawn");
        return false;
    }

    HeroUseSkill(0, SkillID_QZ, 0);
    UseSkill(SkillID_Dash, 0);

    use_skill_wait(SkillID_Return, priest.agent_id, 8000);
    if (!go_to_npc_deadlock(&priest, sec_to_ms(4))) {
        return false;
    }

    if (has_more_luxon()) {
        SendDialog(0x84);
        time_sleep_ms(500);
    }
    SendDialog(0x86);

    use_skill_wait(SkillID_Shroud, 0, 3000);
    use_skill_wait(SkillID_SF, 0, 3000);

    HeroUseSkill(0, SkillID_SoH, 0);
    HeroFlag((Vec2f){-8227.f, 18372.f}, 0);

    UseSkill(SkillID_Dash, 0);
    move_wait(-7924.f, 18281.f, 50.f);
    UseSkill(SkillID_Dash, 0);

    LogDebug("Balling dragons");
    move_wait(-7086.f, 17979.f, 50.f);
    UseSkill(SkillID_Storms, 0);
    move_wait(-6153.f, 16621.f, 50.f);

    HeroUseSkill(0, SkillID_Winnowing, 0);

    move_wait(-5404.f, 15538.f, 50.f);
    move_wait_coarse(-6144.f, 17280.f, 5.f, 10.f);

    UseSkill(SkillID_SoH, 0);

    move_wait(-6604.f, 18585.f, 5.f);

    time_sleep_ms(500);
    HeroFlag((Vec2f){-11271.f, 19390.f}, 0);
    HeroUseSkill(0, SkillID_Dash, 0);

    if (!is_dead(GetMyAgentId()))
        LogDebug("Killing");

    UseSkill(SkillID_Whirling, 0);
    use_skill_wait(SkillID_SF, 0, 3000);
    use_skill_wait(SkillID_RadField, 0, 3000);

    msec_t deadlock_break = time_get_ms() + sec_to_ms(20);
    Vec2f pos;
    do {
        time_sleep_ms(500);
        if (time_get_ms() >= deadlock_break)
            break;
        pos = GetAgentPos(player_id);
    } while (count_foes_in_range(Range_Adjacent, pos, ModelID_DragonMoss) != 0);

    time_sleep_ms(250);

    pickup_loot();

    if (is_dead(player_id)) {
        LogDebug("I failed");
        return_outpost();
        return false;
    } else {
        return return_outpost();
    }
}

static bool return_outpost()
{
    LogDebug("Resigning");
    SendChat(Channel_Emote, "resign");
    while (!GetPartyIsDefeated())
        time_sleep_ms(16);
    ReturnToOutpost();
    return wait_map_loading(MapID_Anjeka);
}

static void pickup_item_safe(ApiItem *item, ApiAgent *agent)
{
    if (!(item && agent)) return;
    assert(agent->type == AgentType_Item);
    AgentId my_id = GetMyAgentId();
    if (!my_id) return;
    move_wait_coarse(agent->position.x, agent->position.y, 25.f, 50.f);
    Vec2f pos = GetAgentPos(my_id);
    if (dist2(pos, agent->position) > 50.f) return;
    PickupItem(item->item_id);
    // @Cleanup: Should wait by waiting for pickup
    ping_sleep(500);
}

static void pickup_loot(void)
{
    ArrayApiAgent agents = {0};
    array_reserve(agents, GetAgents(NULL, 0));
    agents.size = GetAgents(agents.data, agents.capacity);

    AgentId my_id = GetMyAgentId();
    LogDebug("Pick up loots");

    ApiAgent *agent;
    array_foreach(agent, agents) {
        if (!agent->type == AgentType_Item)
            continue;
        ApiItem item;
        if (!GetItemOfAgent(&item, agent->agent_id))
            continue;
        if (!can_pickup(&item)) continue;
        Vec2f pos = GetAgentPos(my_id);
        if (dist2(pos, agent->position) > 2000)
            continue;
        pickup_item_safe(&item, agent);
    }
    array_reset(agents);
}

static bool can_pickup(ApiItem *item)
{
    if (!item) return false;

    switch (item->model_id) {
    case ModelID_DragonRoots:
        roots_count++;
        return true;
    }
    switch (item->type) {
    case ItemType_Coins:
    case ItemType_Consumable:
        return true;
    }

    return true;
}

static void exit_with_status(const char *state, int exit_code)
{
    CloseHandle(mailslot);
    LogCritical("%s (code=%d)", state, exit_code);
    exit(exit_code);
}

static on_party_invite(EventType e, void *args, void *param)
{
    uint32_t *party_id = (uint32_t *)args;
    AcceptInvite(*party_id);
}
static CallbackEntry party_invite;

static void OnPluginUnload(PluginObject *plugin)
{
    running = false;
}

DllExport bool PluginEntry(PluginObject *plugin)
{
    bot_module = plugin->module;
    plugin->PluginUnload = OnPluginUnload;

    size_t length = GetCharacterName(charname, sizeof(charname));
    if (!length) {
        LogError("Couldn't get the player name");
        return false;
    }

    for (size_t i = 0; i < length; i++) {
        if (charname[i] & ~0x7f) {
            LogError("The character name contain non-ascii character.");
            return false;
        }
    }

    party_invite.callback = on_party_invite;
    RegisterEvent(PARTY_INVITE_REQUEST, &party_invite);

    thread_create(&bot_thread, main_bot, NULL);
    thread_detach(&bot_thread);
    return true;
}
