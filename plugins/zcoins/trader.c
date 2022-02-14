#include <Headquarter.h>

#include <time.h>
#include <os/time.h>
#include <os/thread.h>

#include "common.c"

static void manage_inventory(void);

static thread_t bot_thread;
static volatile bool running = true;

static int main_bot(void *lparam)
{
    running = true;
    load_configuration("data/zcoins.vars", "trader");
    LogInfo("ZCoins trader v%s starting...", version);

    mgr_mail_slot = CreateFileA(mgr_mail_slot_name, GENERIC_WRITE,
        FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (mgr_mail_slot == INVALID_HANDLE_VALUE) {
        LogInfo("Couldn't open the manager mail slot '%s' (%lu)",
            mgr_mail_slot_name, GetLastError());
    }

    character_name = GetPlayerName();
    if (!character_name.count) {
        LogError("'GetPlayerName' failed !");
        exit_with_status("Error", 1);
    }

    mail_slot_send("State", "Running");

    if (GetDistrictNumber() != 1) {
        safe_redirect(MapID_GreatTempleOfBalthazar, 3, DISTRICT_AMERICAN, 1);
        if (GetDistrictNumber() != -1) {
            exit_with_status("Error", 1);
        }
    }

    manage_inventory();
    SendChat(Channel_Alliance, "a+ les enfants !");
    ping_sleep(250);

    exit_with_status("Exit", 0);
    return 0;
}

static void manage_inventory(void)
{
    move_wait(-5218.f, -5445.f, 50.f);

    while (count_inventory_item(ITEM_ID_Copper_Zaishen_Coin) >= 50) {
        buy_silver_zcoins();
        wait_gold_change_deadlock(5000);
    }

    while (exchange_gold_zcoins && count_inventory_item(ITEM_ID_Silver_Zaishen_Coin) >= 10) {
        buy_gold_zcoins();
        wait_gold_change_deadlock(5000);
    }

    while (exchange_zkeys && count_inventory_item(ITEM_ID_Gold_Zaishen_Coin) >= 1) {
        buy_zaishen_keys();
        wait_gold_change_deadlock(5000);
    }

    trade_everything_to(mule_name);
}

DllExport bool plugin_load(void)
{
    thread_start(&bot_thread, main_bot, NULL);
    return true;
}

DllExport void plugin_unload(void)
{
    running = false;
}
