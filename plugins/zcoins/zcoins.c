#include <Headquarter.h>

#include <time.h>
#include <os/time.h>
#include <os/thread.h>

#include "common.c"

typedef enum QuestType {
    TYPE_JadeQuarry,
    TYPE_CodexArena,
    TYPE_HeroesAscent,
    TYPE_GuildVersusGuild,
    TYPE_AllianceBattles,
    TYPE_FortAspenwood,
    TYPE_RandomArena,
} QuestType;

typedef struct DailyQuest {
    QuestType type;
    int       id;
} DailyQuest;

static DailyQuest combat_quests[28] = {
    {TYPE_JadeQuarry, 0x45B},
    {TYPE_CodexArena, 0x468},
    {TYPE_HeroesAscent, 0x45E},
    {TYPE_GuildVersusGuild, 0x45D},
    {TYPE_AllianceBattles, 0x444},
    {TYPE_HeroesAscent, 0x446},
    {TYPE_GuildVersusGuild, 0x445},
    {TYPE_CodexArena, 0x465},
    {TYPE_FortAspenwood, 0x442},
    {TYPE_JadeQuarry, 0x443},
    {TYPE_RandomArena, 0x43F},
    {TYPE_CodexArena, 0x466},
    {TYPE_GuildVersusGuild, 0x44D},
    {TYPE_JadeQuarry, 0x44B},
    {TYPE_AllianceBattles, 0x44C},
    {TYPE_HeroesAscent, 0x44E},
    {TYPE_RandomArena, 0x447},
    {TYPE_FortAspenwood, 0x44A},
    {TYPE_JadeQuarry, 0x453},
    {TYPE_RandomArena, 0x44F},
    {TYPE_FortAspenwood, 0x452},
    {TYPE_HeroesAscent, 0x456},
    {TYPE_AllianceBattles, 0x454},
    {TYPE_GuildVersusGuild, 0x455},
    {TYPE_CodexArena, 0x467},
    {TYPE_RandomArena, 0x457},
    {TYPE_FortAspenwood, 0x45A},
    {TYPE_AllianceBattles, 0x45C}
};

static void do_job(void);
static void do_farm(void);
static void do_end_day(void);

static DailyQuest get_quest_of_the_day(void);
static bool is_combat_quest_farmable(DailyQuest *d_quest);
static void update_success_run_needed_and_such(DailyQuest *d_quest);

static void claims_reward(int questid);
static void manage_zcoins(bool end_day);

static void check_faction();
static void manage_faction(FactionType faction);

static void get_kurzick(void);
static void get_luxon(void);
static void get_zkeys(void);

static void take_zaishen_combat_quest(int expected_quest_id);
static bool ugly_get_daily_quest_completed(void);

static void manage_money(void);
static void buy_crafter_item(int send_model_id, int recv_model_id,
    int ratio, int cost_per_unit);
static int request_quote_wait(Item *item);

static void trade_everything_to(const char *name);
static void trade_up_to_7_items(Player *player);
static bool have_item_to_trade(void);
static bool can_trade(Item *item);

static FactionType get_guild_allegiance(void);

static thread_t deadlock_thread;
static int deadlock_main(void *param);

static thread_t bot_thread;
static volatile bool running = true;

static int faction_total     = 0;
static int success_run_total = 0;
static int success_run_cycle = 0;

static DailyQuest daily_quest;

// if the player doesn't have a guild, he won't depose the point. `farm_faction` tell us that
static bool         farm_faction = true;
static unsigned int zkey_acquired = 0;
static FactionType  farmed_faction = FactionType_Balthazar;

static int    arena_id = 0;
static int    success_run_needed = 0;
static msec_t expected_wait = 0;

static int main_bot(void *param)
{
    running = true;
    assert(character_name.count);
    LogInfo("ZCoins farmer v%s starting...", version);

#if defined(OS_WINDOWS)
    mgr_mail_slot = CreateFileA(mgr_mail_slot_name, GENERIC_WRITE,
        FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (mgr_mail_slot == INVALID_HANDLE_VALUE) {
        LogWarn("Couldn't open the manager mail slot '%s' (%lu)",
            mgr_mail_slot_name, GetLastError());
    }
#endif

    daily_quest = get_quest_of_the_day();
    if (!is_combat_quest_farmable(&daily_quest)) {
        LogInfo("Can't farm this quest (type: %d, id: %d)", daily_quest.type, daily_quest.id);
        LogInfo("We are waiting until we can farm the quest.");

        mail_slot_send("State", "Waiting");

        // @Cleanup: Maybe check if that fail
        travel_gh_safe();
        while (!is_combat_quest_farmable(&daily_quest)) {
            time_sleep_sec(10);
            daily_quest = get_quest_of_the_day();
        }

        safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 0);
    }

    assert(is_combat_quest_farmable(&daily_quest));

    // @Enhancement:
    // We could take rewards, but in practice we will never abandon a quest.
    // Could use agent marker for instance.
    for (int i = 0; i < countof(combat_quests); i++) {
        DailyQuest d_quest = combat_quests[i];
        if (get_quest_with_id(d_quest.id) != NULL)
            AbandonQuest(d_quest.id);
    }

    if (count_inventory_free_slot() < 20) {
        LogWarn("You have less than 20 free slots in your inventory !");
    }

    take_zaishen_combat_quest(daily_quest.id);
    update_success_run_needed_and_such(&daily_quest);

    mail_slot_send("State", "Running");
    SendChat(Channel_Alliance, "Farmer professionnel au rapport chef !");

    while (running) {
        do_job();
    }

    return true;
}

static void do_job(void)
{
    check_faction();

    // @Cleanup:
    // When we are sure `quest_completed` is right we can replace
    // `success_run_cycle >= success_run_needed` by it.
    bool quest_completed = ugly_get_daily_quest_completed();
    if (success_run_cycle >= success_run_needed) {
        // assert(quest_completed);
        if (!quest_completed) {
            // if we never see this message, we can assume `quest_completed` is what we want instead of `success_run_cycle >= success_run_needed` by it.
            // LogInfo("Quest is not completed with respect of the encoded string received from the server. This is likelly caused on our part, need more investigation.");
        }

        claims_reward(daily_quest.id);

        DailyQuest d_quest = get_quest_of_the_day();
        if (daily_quest.id != d_quest.id) {
            daily_quest = d_quest;

            if (!is_combat_quest_farmable(&daily_quest)) {
                do_end_day();
                return;
            }

            update_success_run_needed_and_such(&daily_quest);
        }

        safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 0);
        take_zaishen_combat_quest(daily_quest.id);

        success_run_cycle = 0;
        manage_zcoins(false);
        manage_money();

        printf("\nStats:\n");
        printf("  Total runs: %d\n", success_run_total);
        printf("  Total factions: %d\n", faction_total);
        printf("  Total zkeys: %d (today: %d)\n\n",
            count_inventory_item(ITEM_ID_Zaishen_Key), zkey_acquired);
    }

    do_farm();
}

static void do_farm(void)
{
    safe_redirect(arena_id, 6, DISTRICT_AMERICAN, 0);
    assert(GetMapId() == arena_id);

    msec_t start_time = GetAppTime();
    msec_t max_allowed_time = start_time + expected_wait + (30 * 1000);

    assert(GetMatchState() == 0);
    while (GetMatchState() == 0) {
        // @Remark: Since WSAPoll is broken, we cann't know if the connection timedout
        // so we fallback on a heuristic check which should alway be good. =(
        if (!IsIngame() || (GetAppTime() >= max_allowed_time))
            exit_with_status("Error", 1);
        SleepFrame();
    }

    time_sleep_ms(4000);

    msec_t time_at_end = GetAppTime();
    msec_t elapsed = time_at_end - start_time;

    success_run_total += 1;
    success_run_cycle += 1;
    LogInfo("Farm done: map '%d', took %ds, run %d out of %d",
        arena_id, elapsed / 1000, success_run_cycle, success_run_needed);
}

static void do_end_day(void)
{
    manage_zcoins(true);

    manage_money();
    travel_gh_safe();

    SendChat(Channel_Alliance, "Y a rien a faire chef =( Barbecue au hall");
    mail_slot_send("State", "Sleeping");

    running = false;
}

static DailyQuest get_quest_of_the_day(void)
{
    time_t time_at_start_of_cycle = 1256227200;
    time_t current_time = time(NULL);
    time_t diff = current_time - time_at_start_of_cycle;

    time_t seconds_in_day = 86400; // 24 * 60 * 60
    int number_of_day_in_cycle = countof(combat_quests);

    int zq_day = (diff / seconds_in_day) % number_of_day_in_cycle;
    DailyQuest d_quest = combat_quests[zq_day];
    return d_quest;
}

static bool is_combat_quest_farmable(DailyQuest *d_quest)
{
    QuestType t = d_quest->type;
    if ((t == TYPE_JadeQuarry) || (t == TYPE_AllianceBattles) ||
        (t == TYPE_FortAspenwood))
    {
        return true;
    }
    return false;
}

static void update_success_run_needed_and_such(DailyQuest *d_quest)
{
    switch (d_quest->type) {
    case TYPE_AllianceBattles:
        arena_id = MapID_EtnaranKeys;
        success_run_needed = opti_faction ? 2 : 6;
        expected_wait = 90000;
        break;

    case TYPE_FortAspenwood:
        arena_id = MapID_FortAspenwoodArena;
        success_run_needed = opti_faction ? 3 : 9;
        expected_wait = 60000;
        break;

    case TYPE_JadeQuarry:
        arena_id = MapID_JadeQuarryArena;
        success_run_needed = opti_faction ? 3 : 9;
        expected_wait = 60000;
        break;
    }
}

static void claims_reward(int quest_id)
{
    safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 0);

    Agent *zehnchu = get_agent_with_model_id_deadlock(ModelID_Zehnchu, pnj_spawn_deadlock);
    if (!zehnchu) {
        // if the pnj didn't spawn after 5 seconds, we have a big problem.
        LogError("Zehnchu (rewards pnj) didn't spawn. (model_id: %d, map: %d)",
            ModelID_Zehnchu, GetMapId());
        exit_with_status("Error", 1);
    }

    assert(zehnchu);
    if ((zehnchu->marker != AgentMarker_Reward1) && (zehnchu->marker != AgentMarker_Reward2)) {
        LogInfo("Zehnchu doesn't seem to have any rewards to give.");
        // return;
    }

    safe_move_wait(-5218.f, -5445.f, 25.f);
    
    if (!go_to_npc_deadlock(zehnchu, 10000)) {
        safe_move_wait(-5302.f, -5557.f, 50.f);
        safe_go_to_npc(zehnchu);
    }

    // @Cleanup: The sleep here is a little bit bad, but no good way to check right now.
    AcceptReward(quest_id);
    ping_sleep(750);
}

static void manage_zcoins(bool end_day)
{
    // @Remark: We can only trade 16 stacks at the time (4000 zcoins)
    int free_slot = count_inventory_free_slot();
    if (end_day || (free_slot < 3)
        || (count_inventory_item(ITEM_ID_Copper_Zaishen_Coin) >= 3000))
    {
        buy_silver_zcoins();
    }

    if (end_day) {
        while (exchange_gold_zcoins && count_inventory_item(ITEM_ID_Silver_Zaishen_Coin) >= 10) {
            buy_gold_zcoins();
            wait_gold_change_deadlock(GetPing() + 500);
        }

        while (exchange_zkeys && count_inventory_item(ITEM_ID_Gold_Zaishen_Coin) >= 1) {
            buy_zaishen_keys();
            wait_gold_change_deadlock(GetPing() + 500);
        }
    }
}

static void check_faction(void)
{
    static bool zaishen_reset = true;
    static int utc_reset_hour = 15;
    
    struct tm *current_time;
    time_t rawtime = time(NULL);
    current_time = gmtime(&rawtime);

    if ((current_time->tm_hour >= utc_reset_hour) && !zaishen_reset) {
        zaishen_reset = true;
        farm_faction = true;
        farmed_faction = FactionType_Balthazar;
        LogInfo("We are starting to trade imperial for balthazar");
    } else if (current_time->tm_hour < utc_reset_hour) {
        zaishen_reset = false;
    }

    const int GUILD_FACTION_MAX = 214748364;
    if ((farmed_faction != FactionType_Balthazar)
            && ((get_guild_faction() + 5000) > GUILD_FACTION_MAX)) {
        // We cannot depose points anyways.
        return;
    }

    int imperial_needed_to_trade = min(30000, GetMaxImperialFaction());
    if (GetImperialFaction() >= imperial_needed_to_trade) {
        manage_faction(farmed_faction);
    }
}

static void manage_faction(FactionType faction)
{
    if (!random_travel(MapID_KaanaiCanyonKurzick)) {
        LogError("Couldn't travel to Kaanai Canyon (Kurzick) to manage faction");
        exit_with_status("Error", 1);
    }

    if (!move_wait_coarse(580.f, -6118.f, 50.f, 25.f)) {
        exit_with_status("Error", 1);
    }

    if (faction == FactionType_Balthazar) {
        get_zkeys();
    }

    if (!farm_faction)
        return;

    // get_zkeys can change the value of faction.
    if (faction == FactionType_Kurzick) {
        get_kurzick();
    } else if (faction == FactionType_Luxon) {
        get_luxon();
    }
}

static void get_zkeys(void)
{
    assert(GetMapId() == MapID_KaanaiCanyonKurzick);
    Agent *hei_tsu = get_agent_with_model_id_deadlock(ModelID_HeiTsu, pnj_spawn_deadlock);
    if (!hei_tsu) {
        LogInfo("Hei Tsu not found. (model_id: %d, map_id: %d)", ModelID_HeiTsu, GetMapId());
        exit_with_status("Error", 1);
    }

    Agent *tolkano = get_agent_with_model_id_deadlock(ModelID_Tolkano, pnj_spawn_deadlock);
    if (!tolkano) {
        LogInfo("Tolkano not found. (model_id: %d, map_id: %d)", ModelID_Tolkano, GetMapId());
        exit_with_status("Error", 1);
    }

    while (true) {
        int start_balth = GetBalthazarFaction();
        if (start_balth >= 5000) {
            safe_go_to_npc(tolkano);

            while (GetBalthazarFaction() >= 5000) {
                SendDialog(0x88);
                if (!wait_balth_change_deadlock(GetPing() + 1000))
                    break;
                zkey_acquired += 1;
            }

            if (GetBalthazarFaction() >= 5000) {
                DialogInfo *dialog = GetDialogInfo();
                if (wcsncmp(dialog->body, L"\x8103\x0B3B\xAFB7\x940D\x4089", 5)) {
                    // If we never received the message that explicitly tell us that we
                    // can't get anymore zkeys, we don't start trading kurzick|luxon points.
                    return;
                }

                farmed_faction = get_guild_allegiance();
                if ((farmed_faction == FactionType_Kurzick) || (farmed_faction == FactionType_Luxon)) {
                    const char *faction_s = (farmed_faction == FactionType_Kurzick) ? "kurzick" : "luxon";
                    LogInfo("We are starting to trade for imperial %s", faction_s);
                } else {
                    farm_faction = false;
                    LogInfo("Player doesn't have a guild so we won't farm the faction");
                }
            }
            return;
        }

        // Maybe this should be 30k, but when we create new account they only have 25k
        if (GetImperialFaction() < 15000)
            break;

        safe_go_to_npc(hei_tsu);

        int imperial_in_balth = GetImperialFaction() / 3;
        if ((GetMaxBalthazarFaction() - GetBalthazarFaction()) >= imperial_in_balth) {
            SendDialog(0xA3);
            ping_sleep(500);
        } else {
            while (GetImperialFaction() >= 30000 &&
                (GetMaxBalthazarFaction() - GetBalthazarFaction()) >= 10000)
            {
                SendDialog(0xA0);
                ping_sleep(500);
            }
            while (GetImperialFaction() >= 15000 &&
                (GetMaxBalthazarFaction() - GetBalthazarFaction()) >= 5000)
            {
                SendDialog(0x9D);
                ping_sleep(500);
            }
            while (GetImperialFaction() >= 5000 &&
                (GetMaxBalthazarFaction() - GetBalthazarFaction()) >= 1000)
            {
                SendDialog(0x9A);
                ping_sleep(500);
            }
        }
    }
}

static void get_kurzick(void)
{
    assert(GetMapId() == MapID_KaanaiCanyonKurzick);
    assert(farmed_faction == FactionType_Kurzick);

    Agent *hei_tsu = get_agent_with_model_id_deadlock(ModelID_HeiTsu, pnj_spawn_deadlock);
    if (!hei_tsu) {
        LogInfo("Hei Tsu not found. (model_id: %d, map_id: %d)", ModelID_HeiTsu, GetMapId());
        exit_with_status("Error", 1);
    }

    safe_go_to_npc(hei_tsu);

    if (GetImperialFaction() <= (GetMaxKurzickFaction() - GetKurzickFaction())) {
        SendDialog(0xA1);
        ping_sleep(500);
    } else {
        while ((GetImperialFaction() >= 10000)
            && (GetMaxKurzickFaction() - GetKurzickFaction()) >= 10000)
        {
            SendDialog(0x9E);
            ping_sleep(500);
        }
        while ((GetImperialFaction() >= 5000)
            && (GetMaxKurzickFaction() - GetKurzickFaction()) >= 5000)
        {
            SendDialog(0x9B);
            ping_sleep(500);
        }
        while ((GetImperialFaction() >= 5000)
            && (GetMaxKurzickFaction() - GetKurzickFaction()) >= 1000)
        {
            SendDialog(0x98);
            ping_sleep(500);
        }
    }

    // travel_gh_safe();

    Agent *kurzick = get_agent_with_model_id_deadlock(ModelID_KurzickBureaucrat,
        pnj_spawn_deadlock);

    if (!kurzick) {
        LogInfo("Kurzick Bureaucrat not found. (model_id: %d, map_id: %d)",
            ModelID_KurzickBureaucrat, GetMapId());
        exit_with_status("Error", 1);
    }

    assert(kurzick);
    safe_go_to_npc(kurzick);

    {
        char buffer[256];
        int deposed_k = (GetKurzickFaction() / 5000) * 5;
        snprintf(buffer, countof(buffer), "Allez hop, +%dk !", deposed_k);
        SendChat(Channel_Alliance, buffer);
    }
    
#if 1
    int deposit_count = GetKurzickFaction() / 5000;
    for (int i = 0; i < deposit_count; i++) {
        faction_total += 5000;
        DonateFaction(FactionType_Kurzick);
        ping_sleep(500);
    }
#else
    const int MAX_GUILD_KURZICK = 214748364;
    while (GetKurzickFaction() >= 5000) {
        if (GetGuildFaction() + 5000 >= 214748364)
            break;
        faction_total += 5000;
        DonateFaction(FactionType_Kurzick);
        ping_sleep(500);
    }
#endif
}

static void get_luxon(void)
{
    assert(GetMapId() == MapID_KaanaiCanyonKurzick);
    assert(farmed_faction == FactionType_Luxon);

    Agent *hei_tsu = get_agent_with_model_id_deadlock(ModelID_HeiTsu, pnj_spawn_deadlock);
    if (!hei_tsu) {
        LogInfo("Hei Tsu not found. (model_id: %d, map_id: %d)", ModelID_HeiTsu, GetMapId());
        exit_with_status("Error", 1);
    }

    safe_go_to_npc(hei_tsu);

    if (GetImperialFaction() <= (GetMaxLuxonFaction() - GetLuxonFaction())) {
        SendDialog(0xA2);
        ping_sleep(500);
    } else {
        while ((GetImperialFaction() >= 10000)
            && (GetMaxLuxonFaction() - GetLuxonFaction()) >= 10000)
        {
            SendDialog(0x9F);
            ping_sleep(500);
        }
        while ((GetImperialFaction() >= 5000)
            && (GetMaxLuxonFaction() - GetLuxonFaction()) >= 5000)
        {
            SendDialog(0x9C);
            ping_sleep(500);
        }
        while ((GetImperialFaction() >= 1000)
            && (GetMaxLuxonFaction() - GetLuxonFaction()) >= 1000)
        {
            SendDialog(0x99);
            ping_sleep(500);
        }
    }

    // cannot trade luxon at this outpost so fallback on gh
    if (!travel_gh_safe()) {
        LogError("Couldn't travel to guild hall to trade luxon");
        return;
    }

    Agent *luxon = get_agent_with_model_id_deadlock(ModelID_LuxonBureaucrat, pnj_spawn_deadlock);
    if (!luxon) {
        LogInfo("Luxon Bureaucrat not found. (model_id: %d, map_id: %d)",
            ModelID_LuxonBureaucrat, GetMapId());
        exit_with_status("Error", 1);
    }

    assert(luxon);
    safe_go_to_npc(luxon);
    
    {
        char buffer[256];
        int deposed_k = (GetLuxonFaction() / 5000) * 5;
        snprintf(buffer, countof(buffer), "Allez hop, +%dk !", deposed_k);
        SendChat(Channel_Alliance, buffer);
    }

#if 1
    int deposit_count = GetLuxonFaction() / 5000;
    for (int i = 0; i < deposit_count; i++) {
        faction_total += 5000;
        DonateFaction(FactionType_Luxon);
        ping_sleep(500);
    }
#else
    const int MAX_GUILD_KURZICK = 214748364;
    while (GetLuxonFaction() >= 5000) {
        if (GetGuildFaction() + 5000 >= 214748364)
            break;
        faction_total += 5000;
        DonateFaction(FactionType_Luxon);
        ping_sleep(500);
    }
#endif
}

static void take_zaishen_combat_quest(int expected_quest_id)
{
    if (GetMapId() != MapID_GreatTempleOfBalthazar) 
        safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 0);

take_quest_again:;
    Agent *combat_signpost = get_agent_with_model_id_deadlock(ModelID_Zaishen_Combat, pnj_spawn_deadlock);
    if (!combat_signpost) {
        // if the pnj didn't spawn after 5 seconds, we have a big problem.
        LogError("Combat signpost (pnj for imperial) didn't spawn. (model_id: %d, map: %d)",
            ModelID_Zaishen_Combat, GetMapId());
        exit_with_status("Error", 1);
    }

    assert(combat_signpost);
    
    // @Cleanup: We need to make sure that quest an agent info have time to arrive.
    ping_sleep(500);

    if (combat_signpost->marker != AgentMarker_Quest) {
        if (player_has_quest(expected_quest_id)) {
            // @Cleanup: Check if the quest is completed.
            AbandonQuest(expected_quest_id);
        }

        // @Cleanup:
        // This should happend if we took the reward and didn't reload.
        // But it could also happend if we have too many combat quest.
        // We currently don't check the former, so the bot could faile because of that.
        safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 0);
        goto take_quest_again;
    }

    safe_move_wait(-5218.f, -5445.f, 50.f);
    if (!go_to_npc_deadlock(combat_signpost, 10000)) {
        safe_move_wait(-5302.f, -5557.f, 50.f);
        safe_go_to_npc(combat_signpost);
    }

    int expected_dialog_id = 0x00800000 | (expected_quest_id << 8) | 1;
    bool dialog_exist = false;

    DialogButton *button;
    DialogInfo *dialog = GetDialogInfo();
    array_foreach(button, dialog->buttons) {
        if (button->dialog_id == expected_dialog_id)
            dialog_exist = true;
    }

    if (!dialog_exist) {
        LogInfo("We didn't find the dialog for the quest %d (0x%X) at the combat signpost.",
            expected_quest_id, expected_quest_id);
        // @Remark: This returns shouldn't be commented, but for some reason the button
        // with the expected dialog is not found. Need further investigation.        
        // return;
    }

    // @Enhancement: check with the dialog if it's indeed the right quest id.
    AcceptQuest(expected_quest_id);
}

// @Cleanup: This is ugly (duh), we could improve it with proper encoded string parser.
static bool ugly_get_daily_quest_completed(void)
{
    Quest *quest = get_quest_with_id(daily_quest.id);
    if (!quest) return true;
    return (quest->objective[24] == 0x2AF5);
}

static void manage_money(void)
{
    if (GetGoldCharacter() > 90000) {
        DepositGold(0);
        wait_gold_change_deadlock(GetPing() + 500);
    }

    if (GetGoldStorage() >= storage_gold_treshold) {
        if (!travel_gh_safe()) {
            LogError("Couldn't travel to guild hall to manage money");
            return;
        }

        int rare_mat_trader_id = get_rare_material_trader_id(GetMapId());
        assert(rare_mat_trader_id != 0);

        Agent *rare_mat_trader = get_agent_with_model_id_deadlock(rare_mat_trader_id, pnj_spawn_deadlock);
        if (!rare_mat_trader) {
            LogWarn("Couldn't find the rate material trader. (map: %d, pnj_id: %d)",
                GetMapId(), rare_mat_trader_id);
            return;
        }

        assert(rare_mat_trader);
        safe_go_to_npc(rare_mat_trader);

        // make sure the merchant has time to send the items.
        ping_sleep(500);

        Item *ecto_item = NULL;
        ArrayItem items = GetMerchantItems();

        Item **item;
        array_foreach(item, items) {
            if (!*item) continue;
            if ((*item)->model_id == ITEM_ID_GlobOfEctoplasm) {
                ecto_item = *item;
                break;
            }
        }

        if (!ecto_item) {
            LogWarn("Couldn't find the ecto in the merchant windows. (map: %d)", GetMapId());
            return;
        }

        int price = request_quote_wait(ecto_item);
        if (price == 0) {
            LogWarn("There is no more ectos at the merchant (or related bug)");
            return;
        }

        while (GetGoldStorage() >= storage_gold_treshold) {
            WithdrawGold(0);
            wait_gold_change_deadlock(GetPing() + 500);

            while (GetGoldCharacter() >= price) {
                // printf("price: %d, gold character: %d\n", price, GetGoldCharacter());
                buy_rare_material_item(ecto_item);
                wait_gold_change_deadlock(GetPing() + 500);
                price = request_quote_wait(ecto_item);
                if (!price) return;
            }
        }
    }
}

static FactionType get_guild_allegiance(void)
{
    Guild *player_guild = GetPlayerGuild();
    if (!player_guild) return FactionType_Balthazar;
    FactionType allegiance = player_guild->allegiance;
    return allegiance;
}

DllExport bool plugin_load(void)
{
    load_configuration("data/zcoins.vars", "zcoins");

    character_name = GetPlayerName();
    if (!character_name.count) {
        LogError("Couldn't get the player name");
        return false;
    }

    char file_path[255 /* MAX_PATH */];
    snprintf(file_path, sizeof(file_path), "logs/zcoins - %.*s.txt",
        character_name.count, character_name.bytes);
    /*
    if (!(L.stream = fopen(file_path, "a+"))) {
        LogWarn("Couldn't open the log file '%s'", file_path);
    }
    */

    thread_start(&bot_thread, main_bot, NULL);
    thread_start(&deadlock_thread, deadlock_main, NULL);

    return true;
}

DllExport void plugin_unload(void)
{
    running = false;
    mail_slot_send("State", "");
#if defined(OS_WINDOWS)
    CloseHandle(mgr_mail_slot);
#endif
}

DllExport void on_panic(const char *msg)
{
    exit_with_status("Error", 1);
}

static int deadlock_main(void *param)
{
    msec_t deadlock = 30 * 1000;
    msec_t ingame_deadlock;
    bool   ingame_is_dead;

    while (running) {
        bool ingame = IsIngame();
        if (!ingame && ingame_is_dead && (GetAppTime() >= ingame_deadlock)) {
            exit_with_status("Error", 1);
        } else if (ingame && ingame_is_dead) {
            ingame_is_dead = false;
        } else if (!ingame && !ingame_is_dead) {
            ingame_is_dead = true;
            ingame_deadlock = GetAppTime() + deadlock;
        }

        time_sleep_sec(5);
    }

    return 0;
}
