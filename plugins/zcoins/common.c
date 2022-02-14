#include "vars.c"
#include "gw-helper.c"
#include <stdlib.h>

#define MapID_FortAspenwoodKurzick      294
#define MapID_FortAspenwoodArena        221
#define MapID_JadeQuarryArena           223
#define MapID_KaanaiCanyonKurzick       338
#define MapID_AspenwoodGateKurzick      388
#define MapID_EtnaranKeys               300
#define MapID_GreatTempleOfBalthazar    248

#define ModelID_Zaishen_Combat          1199
#define ModelID_Zehnchu                 1192
#define ModelID_HeiTsu                  3092
#define ModelID_Tesla                   1193
#define ModelID_Pokhe                   1194
#define ModelID_Llam                    1195
#define ModelID_Tolkano                 219
#define ModelID_KurzickBureaucrat       3410
#define ModelID_LuxonBureaucrat         3639

#define ITEM_ID_Copper_Zaishen_Coin     31202
#define ITEM_ID_Silver_Zaishen_Coin     31204
#define ITEM_ID_Gold_Zaishen_Coin       31203
#define ITEM_ID_Zaishen_Key             28517
#define ITEM_ID_GlobOfEctoplasm         930

static const char version[]         = "2.1";
static char  mule_name[32]          = "";
static bool  exchange_gold_zcoins   = false;
static bool  exchange_zkeys         = false;
static bool  opti_faction           = false;
static int   storage_gold_treshold  = 1000 * 1000;

static string character_name;

// Sometime the pnj doesn't spawn right away so we add a max wait time.
// If the pnj didn't spawn after this wait time we have a fatal error.
static const msec_t pnj_spawn_deadlock = 5 * 1000;

static void buy_silver_zcoins(void);
static void buy_gold_zcoins(void);
static void buy_zaishen_keys(void);

static void safe_move_wait(float x, float y, float r);
static void safe_go_to_npc(Agent *agent);

static void wait_gold_change(void);
static bool wait_gold_change_deadlock(msec_t deadlock);

static void buy_crafter_item(int send_model_id, int recv_model_id,
    int ratio, int cost_per_unit);
static int request_quote_wait(Item *item);

static void trade_everything_to(const char *name);
static void trade_up_to_7_items(Player *player);
static bool have_item_to_trade(void);
static bool can_trade(Item *item);

_Noreturn static void exit_with_status(const char *state, int exit_code);
static void mail_slot_send(const char *type, const char *value);
static void safe_redirect(int zone_id, int zone_type, District district, int district_number);

#if defined(OS_WINDOWS)
static char   mgr_mail_slot_name[] = "\\\\.\\mailslot\\ZCoinsManager";
static HANDLE mgr_mail_slot;
#endif

static void buy_silver_zcoins(void)
{
    assert(GetMapId() == MapID_GreatTempleOfBalthazar);

    AgentID interact_with = GetInteractWith();
    Agent *tesla = get_agent_with_model_id_deadlock(ModelID_Tesla, pnj_spawn_deadlock);
    if (!tesla) {
        LogInfo("Tesla not found !! Can't manage zcoins. (map: %d, model_id: %d)",
                GetMapId(), ModelID_Tesla);
        thread_exit(1);
    }

    if (interact_with != tesla->agent_id) {
        move_wait(-5065.f, -5720.f, 50.f);
        go_to_npc(tesla);
    }

    buy_crafter_item(ITEM_ID_Copper_Zaishen_Coin, ITEM_ID_Silver_Zaishen_Coin, 50, 10);
}

static void buy_gold_zcoins(void)
{
    assert(GetMapId() == MapID_GreatTempleOfBalthazar);

    AgentID interact_with = GetInteractWith();
    Agent *pokhe = get_agent_with_model_id_deadlock(ModelID_Pokhe, pnj_spawn_deadlock);
    if (!pokhe) {
        LogInfo("Pokhe not found !! Can't manage zcoins. (map: %d, model_id: %d)",
                GetMapId(), ModelID_Pokhe);
        thread_exit(1);
    }

    if (interact_with != pokhe->agent_id) {
        move_wait(-5079.f, -5789.f, 50.f);
        go_to_npc(pokhe);
    }

    buy_crafter_item(ITEM_ID_Silver_Zaishen_Coin, ITEM_ID_Gold_Zaishen_Coin, 10, 50);
}

static void buy_zaishen_keys(void)
{
    assert(GetMapId() == MapID_GreatTempleOfBalthazar);

    AgentID interact_with = GetInteractWith();
    Agent *llam = get_agent_with_model_id_deadlock(ModelID_Llam, pnj_spawn_deadlock);
    if (!llam) {
        LogInfo("Llam not found !! Can't manage zcoins. (map: %d, model_id: %d)",
                GetMapId(), ModelID_Llam);
        thread_exit(1);
    }

    if (interact_with != llam->agent_id) {
        move_wait(-5052.f, -5883.f, 50.f);
        go_to_npc(llam);
    }

    buy_crafter_item(ITEM_ID_Gold_Zaishen_Coin, ITEM_ID_Zaishen_Key, 1, 100);
}

static void safe_move_wait(float x, float y, float r)
{
    if (!move_wait(x, y, r)) {
        bool ingame = IsIngame();
        LogError("'safe_move_wait' failed ingame: %s", ingame ? "true" : "false");
        exit_with_status("Error", 1);
    }
}

static void safe_go_to_npc(Agent *agent)
{
    if (!go_to_npc_deadlock(agent, 60 * 1000)) {
        bool ingame = IsIngame();
        LogError("'safe_go_to_npc' failed ingame: %s", ingame ? "true" : "false");
        exit_with_status("Error", 1);
    }
}

static void wait_gold_change(void)
{
    int gold = GetGoldCharacter();
    SleepFrame();
    while (gold == GetGoldCharacter()) {
        SleepFrame();
    }
}

static bool wait_gold_change_deadlock(msec_t deadlock)
{
    msec_t deadlock_end = GetAppTime() + deadlock;
    int gold = GetGoldCharacter();
    SleepFrame();
    while (gold == GetGoldCharacter()) {
        SleepFrame();
        if (GetAppTime() > deadlock_end)
            return false;
        if (!IsIngame())
            return false;
    }
    return true;
}

static void wait_balth_change(void)
{
    int balth = GetBalthazarFaction();
    while (balth == GetBalthazarFaction()) {
        SleepFrame();
    }
}

static bool wait_balth_change_deadlock(msec_t deadlock)
{
    msec_t deadlock_end = GetAppTime() + deadlock;
    int balth = GetBalthazarFaction();
    while (balth == GetBalthazarFaction()) {
        SleepFrame();
        if (GetAppTime() > deadlock_end)
            return false;
        if (!IsIngame())
            return false;
    }
    return true;
}

static void buy_crafter_item(int send_model_id, int recv_model_id,
    int ratio, int cost_per_unit)
{
    TransactionInfo item_send = {0};
    TransactionInfo item_recv = {0};

    int estimation_recv;
    int total_send_item = 0;

    for (int i = 0; i < 4; i++) {
        ArrayItem items = get_items_in_bag(i);
        for (size_t i = 0; i < items.size; i++) {
            Item *item = array_at(items, i);
            if (!item) continue;
            if (item->model_id != send_model_id)
                continue;

            estimation_recv = (total_send_item + item->quantity) / ratio;
            if (estimation_recv > 250)
                goto quit_loop;

            total_send_item += item->quantity;

            int j = item_send.item_count;
            item_send.item_ids[j] = item->item_id;
            item_send.item_quants[j] = item->quantity;

            if (++item_send.item_count >= 16)
                goto quit_loop;
        }
    }

quit_loop:
    if (total_send_item < ratio)
        return;

    int overhead = total_send_item % ratio;
    if (overhead > 0) {
        int last_index = item_send.item_count - 1;
        if (item_send.item_quants[last_index] < overhead) {
            overhead -= item_send.item_quants[last_index];
            last_index--;
        }

        assert((last_index >= 0) && (item_send.item_quants[last_index] >= overhead));
        item_send.item_quants[last_index] -= overhead;
        if (item_send.item_quants[last_index] <= 0)
            last_index--;
        item_send.item_count = last_index + 1;
    }

    int total_recv_item = total_send_item / ratio;
    int gold_needed = total_recv_item * cost_per_unit;

    if (GetGoldCharacter() < gold_needed) {
        WithdrawGold(gold_needed);
        ping_sleep(500);

        if (GetGoldCharacter() < gold_needed) {
            LogInfo("Didn't have enough gold for the trade.\n\r"
                "(recv_model_id: %d, send_model_id: %d, gold_needed: %d, gold_character: %d)",
                recv_model_id, send_model_id, gold_needed, GetGoldCharacter());
            return;
        }
    }

    ItemID item_merch_id = 0;
    ArrayItem merch_items = GetMerchantItems();
    for (size_t i = 0; i < merch_items.size; i++) {
        Item *item = merch_items.data[i];
        if (item->model_id == recv_model_id)
            item_merch_id = item->item_id;
    }

    if (!item_merch_id) {
        return;
    }

    // Gw doesn't allow receiving more than 1 stack at the time.
    // total_recv_item = min(total_recv_item, 250);
    assert(total_recv_item <= 250);

    item_recv.item_count = 1;
    item_recv.item_ids[0] = item_merch_id;
    item_recv.item_quants[0] = total_recv_item;

    TransactItems(TRANSACT_TYPE_CrafterBuy, gold_needed, &item_send, 0, &item_recv);
}

static int request_quote_wait(Item *item)
{
    QuoteInfo item_send = {0};
    QuoteInfo item_recv = {0};
    item->quote_price = 0;
    
    item_recv.item_count = 1;
    item_recv.item_ids[0] = item->item_id;

    RequestQuote(TRANSACT_TYPE_TraderBuy, &item_send, &item_recv);

    msec_t deadlock_break = GetAppTime() + sec_to_ms(5);
    while (item->quote_price == 0) {
        SleepFrame();
        if (GetAppTime() >= deadlock_break) {
            LogError("Couldn't request a quote. (item: %d, map: %d)", item->item_id, GetMapId());
            return 0;
        }
    }

    return item->quote_price;
}

static void exit_with_status(const char *state, int exit_code)
{
    mail_slot_send("State", state);
#if defined(OS_WINDOWS)
    CloseHandle(mgr_mail_slot);
#endif
    exit(exit_code);
}

static void mail_slot_send(const char *type, const char *value)
{
#if defined(OS_WINDOWS)
    if ((mgr_mail_slot == NULL) || (mgr_mail_slot == INVALID_HANDLE_VALUE))
        return;

    char message[256] = {0};
    strncat(message, character_name.bytes, min(256, 1+character_name.count));
    strncat(message, "/", 256);
    strncat(message, type, 256);
    strncat(message, "/", 256);
    strncat(message, value, 256);

    DWORD szbuf = strlen(message);
    DWORD written;

    BOOL succeed = WriteFile(mgr_mail_slot, message, szbuf, &written, NULL);
    if (succeed != TRUE || szbuf != written) {
        if (GetLastError() == ERROR_HANDLE_EOF) {
            mgr_mail_slot = CreateFileA(mgr_mail_slot_name, GENERIC_WRITE,
                FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (mgr_mail_slot != INVALID_HANDLE_VALUE) {
                mail_slot_send(type, value);
                return;
            }
            LogWarn("Couldn't open the manager mail slot '%s' (%lu)",
                    mgr_mail_slot_name, GetLastError());
        }
        LogInfo("Couldn't write '%s' to the mail slot. (bytes: %lu, error: %lu)",
            message, written, GetLastError());
        return;
    }
#endif
}

static void safe_redirect(int zone_id, int zone_type, District district, int district_number)
{
#if 0
    EnsureAuthConnection();
#else
    if (!IsConnected()) {
        exit_with_status("Error", 1);
    }
#endif

    RedirectMap(zone_id, zone_type, district, district_number);
    if (!WaitMapLoading(zone_id)) {
        LogError("Couldn't load map '%d'", zone_id);
        exit_with_status("Error", 1);
    }
}

static void trade_everything_to(const char *name)
{
    if (!name) return;

    Player *mule = get_player_with_name(name);
    if (mule == NULL) {
        LogInfo("Couldn't find the player '%S' in the map %d", name, GetMapId());
        return;
    }

    while (have_item_to_trade()) {
        trade_up_to_7_items(mule);
        // @Cleanup: check if stack have been changed instead if ping_sleep
        ping_sleep(750);
    }
}

static void trade_up_to_7_items(Player *player)
{
    TradeSession *session = GetTradeSession();

    msec_t last_request_time = 0;
    while (session->state == TRADE_CLOSED) {
        msec_t curr_time = GetAppTime();
        if (curr_time - last_request_time >= 7000) {
            TradeInitiate(player);
            last_request_time = curr_time;
        }
        SleepFrame();
    }

    if (session->trader != player->player_id)
        return;

    int sent_count = 0;
    for (int i = 0; i < 4; i++) {
        ArrayItem items = get_items_in_bag(i);
        for (size_t j = 0; j < items.size; j++) {
            Item *item = array_at(items, j);
            if (!item) continue;

            if (can_trade(item)) {
                TradeAddItem(item, 0);
                if (++sent_count >= 7)
                    break;
            }
        }

        if (sent_count >= 7)
            break;
    }

    TradeSendOffer(0);
    while (session->state != TRADE_OFFER_ACCEPTED) {
        SleepFrame();
        if (session->state == TRADE_CLOSED)
            return;
    }

    TradeAccept();
    while (session->state != TRADE_CLOSED)
        SleepFrame();
}

static bool have_item_to_trade(void)
{
    for (int i = 0; i < 4; i++) {
        ArrayItem items = get_items_in_bag(i);
        for (size_t j = 0; j < items.size; j++) {
            Item *item = array_at(items, j);
            if (!item) continue;
            if (can_trade(item))
                return true;
        }
    }

    return false;
}

static bool can_trade(Item *item)
{
    if (!item) return false;

    switch (item->model_id) {
        case ITEM_ID_GlobOfEctoplasm:
        case ITEM_ID_Gold_Zaishen_Coin:
        case ITEM_ID_Silver_Zaishen_Coin:
        case ITEM_ID_Zaishen_Key:
            return true;
    }

    return false;
}

static void load_configuration(const char *path, const char *sec_name)
{
    VarFile file = parse_vars_file(path);
    if (!file.error) {        
        VarSection *section = find_section(&file, sec_name);
        opti_faction            = var_get_boolean_default(section, "opti_faction",  false);
        exchange_gold_zcoins    = var_get_boolean_default(section, "exchange_gold", false);
        exchange_zkeys          = var_get_boolean_default(section, "exchange_zkey", false);
        storage_gold_treshold   = var_get_int_default(section, "gold_treshold", 1000 * 1000);

        const char *str = var_get_string_default(section, "name", "");
        if (!wcsnul(str)) strncpy(mule_name, str, 32);
    } else {
        LogWarn("Couldn't load the configuration '%s'", path);
    }

    free_vars_file(&file);
}
