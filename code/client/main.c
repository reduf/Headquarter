#ifndef _CRT_SECURE_NO_WARNINGS
# define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
# define _CRT_NONSTDC_NO_DEPRECATE
#endif

#ifndef __STDC__
# define __STDC__ 1
#endif

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
# pragma comment(lib, "Ws2_32.lib")
# pragma comment(lib, "Winmm.lib")
# include <common/win32/win32.h>

# include <timeapi.h>
# include <Shellapi.h> // CommandLineToArgvA
# include <Winsock2.h>
# include <Ws2tcpip.h>
#endif

#include <mbedtls/arc4.h>
#include <mbedtls/sha1.h>
#include <mbedtls/bignum.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>

#include <common/macro.h>
#include <common/time.h>
#include <common/assert.h>
#include <common/dlfunc.h>
#include <common/socket.h>
#include <common/thread.h>
#include <common/process.h>
#include <common/noreturn.h>

#include <common/log.h>
#include <common/list.h>
#include <common/uuid.h>
#include <common/hash.h>
#include <common/timer.h>
#include <common/endian.h>
#include <common/rbtree.h>

#include "Headquarter.h"
#include "command.h"
#include "error.h"

#include "iocp.h"
#include "replay.h"
#include "portal.h"
#include "opcodes.h"
#include "obsv_opcodes.h"
#include "packets.h"
#include "network.h"

#include "object.h"
#include "bag.h"
#include "chat.h"
#include "item.h"
#include "agent.h"
#include "guild.h"
#include "party.h"
#include "quest.h"
#include "skill.h"
#include "trade.h"
#include "title.h"
#include "dialog.h"
#include "effect.h"
#include "friend.h"
#include "player.h"
#include "instance.h"
#include "merchant.h"
#include "character.h"
#include "inventory.h"

#include "world.h"
#include "event.h"
#include "client.h"

#include "auth.h"
#include "game.h"
#include "plugins.h"
#include "broadcast.h"

#include "api.c"
#include "auth.c"
#include "game.c"
#include "plugins.c"
#include "broadcast.c"

#include "chat.c"
#include "hero.c"
#include "item.c"
#include "agent.c"
#include "guild.c"
#include "party.c"
#include "quest.c"
#include "skill.c"
#include "trade.c"
#include "title.c"
#include "dialog.c"
#include "effect.c"
#include "friend.c"
#include "player.c"
#include "instance.c"
#include "merchant.c"

#include "object.c"
#include "world.c"
#include "event.c"
#include "client.c"

#include "iocp.c"
#include "replay.c"
#include "portal.c"
#include "packets.c"
#include "obsv_packets.c"
#include "network.c"

#include "vars.c"
#include "error.c"
#include "command.c"

_Static_assert(sizeof(Appearance)  == 4, "Appearance size is incorect.");

_Static_assert(sizeof(int64_t) == 8, "int64_t must be exactly 64 bits");
_Static_assert(sizeof(int32_t) == 4, "int32_t must be exactly 32 bits");
_Static_assert(sizeof(int16_t) == 2, "int16_t must be exactly 16 bits");
_Static_assert(sizeof(int8_t)  == 1, "int8_t  must be exactly 8 bits");

static bool     quit;
static thread_t thread_loader;
static uint32_t fps;

GwClient *__client;

static void sighandler(int signum)
{
    (void)signum;
    quit = true;
}

static int script_loader_thread(void *lpParam)
{
    GwClient *client = cast(GwClient *)lpParam;

    while (client->screen != SCREEN_LOGIN_SCREEN)
        time_sleep_ms(1);

    if (options.portal) {
        while (!portal_received_key)
            time_sleep_ms(1);
        PortalAccountConnect(client, portal_user_id, portal_session_id, client->character);
    } else {
        AccountConnect(client, client->email, strzero, client->character);
        if (!client->connected) {
            LogError("'AccountConnect' failed");
            exit(1);
        }
    }

    assert(client->ingame == false);
    GameSrv_PlayCharacter(client, strzero, PlayerStatus_Online);

    // @Cleanup: 'try_changing_zone' is not really meant to do that kind of check
    if (!client->try_changing_zone) {
        LogError("'GameSrv_PlayCharacter' failed");
        exit(1);
    }

    while (client->try_changing_zone || !client->ingame)
        time_sleep_ms(16);
    plugin_load(options.script);
    return 0;
}

static void main_loop(void);

int main(int argc, const char *argv[])
{
    parse_command_args(argc - 1, argv + 1);

    if (options.print_version) {
        printf("Headquarter %d.%d\n", HEADQUARTER_VERSION_MAJOR, HEADQUARTER_VERSION_MINOR);
        return 0;
    }

    if (argc < 2 || options.print_help) {
        print_help(true);
    }

    if (wcsnul(options.character) || wcsnul(options.password)
        || wcsnul(options.email) || !options.script)
    {
        print_help(true);
    }

    // @Enhancement: add arguments "-seed=23232"
    unsigned int seed = (unsigned int)time(NULL);
    srand(seed);

    signal(SIGINT, sighandler);
    log_init();
    time_init();
    init_timers();

#ifdef _WIN32
    timeBeginPeriod(1);
#endif
    fps = 60;

    Network_Init();

    if (options.portal) {
        if (!portal_init())
            return 1;
    }

    GwClient app;
    GwClient *client = &app;
    init_client(client);
    __client = client;

    if (!init_auth_connection(client, options.auth_srv)) {
        LogInfo("Auth connection try failed.");
        exit(1);
    }

    {
        strncpy_s(client->email_buffer, sizeof(client->email_buffer), options.email, sizeof(client->email_buffer) - 1);
        client->email.bytes = cast(uint8_t *)client->email_buffer;
        client->email.count = strlen(client->email_buffer);

        strncpy_s(client->charac_buffer, sizeof(client->charac_buffer), options.character, sizeof(client->charac_buffer) - 1);
        client->character.bytes = cast(uint8_t *)client->charac_buffer;
        client->character.count = strlen(client->charac_buffer);

        string password;
        password.bytes = cast(uint8_t *)options.password;
        password.count = strlen(options.password);

        if (options.portal) {
            portal_login(client->email, password);
        } else {
            compute_pswd_hash(client->email, password, client->password);
        }

    #if defined(HEADQUARTER_CONSOLE)
        SetConsoleTitleA(options.character);
    #endif

        // memzero(options.email,     sizeof(options.email));
        // memzero(options.password,  sizeof(options.password));
        // memzero(options.character, sizeof(options.character));

        int error = thread_create(&thread_loader, script_loader_thread, client);
        if (error != 0) {
            LogError("Couldn't open the thread_plugin loader (error %d)", error);
            return 1;
        }
    }

    main_loop();

    while (!list_empty(&connections)) {
        Connection *conn = list_first_entry(&connections, Connection, node);
        NetConn_Reset(conn);
        free(conn);
        list_remove(&conn->node);
    }

    while (!list_empty(&plugins)) {
        Plugin *it = plugin_first();
        plugin_unload(it);
    }

    if (options.portal) {
        portal_cleanup();
    }

    Network_Shutdown();
    printf("Quit cleanly !!\n");

    return 0;
}

static void main_loop(void)
{
    GwClient *client = __client;

    uint32_t frame_count = 0;
    struct timespec t0, t1;
    struct timespec last_frame_time;

    while (!quit) {
        frame_count += 1;
        get_wall_clock_time(&t0);

        long frame_delta_nsec = time_diff_nsec(&t0, &last_frame_time);
        msec_t frame_delta = frame_delta_nsec / 1000;

        release_connections();

        // Update(networks) (i.e. update input)
        // iocp_process_all(&iocp);

        NetConn_Update(&client->auth_srv);
        NetConn_Update(&client->game_srv);

        // Update worlds
        client_frame_update(client, frame_delta);

        // trigger all timers event
        process_timers();

        // repost the recv & the send or any event
        // iocp_process_posted_event(&iocp);

        {
            long frame_target_nsec = 1000000000 / fps;

            get_wall_clock_time(&t1);
            long diff_nsec = time_diff_nsec(&t1, &t0);
            long sleep_time_nsec = frame_target_nsec - diff_nsec;
            if (sleep_time_nsec > 0)
                time_sleep_ns(sleep_time_nsec);
        }

        last_frame_time = t0;
    }
}
