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

#include "headquarter.h"
#include "command.h"
#include "error.h"
#include "kstr.h"

#include "portal.h"
#include "opcodes.h"
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

#include "api.c"
#include "auth.c"
#include "game.c"
#include "plugins.c"

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

#include "portal.c"
#include "packets.c"
#include "network.c"

#include "kstr.c"
#include "vars.c"
#include "error.c"
#include "command.c"

_Static_assert(sizeof(Appearance)  == 4, "Appearance size is incorect.");

_Static_assert(sizeof(int64_t) == 8, "int64_t must be exactly 64 bits");
_Static_assert(sizeof(int32_t) == 4, "int32_t must be exactly 32 bits");
_Static_assert(sizeof(int16_t) == 2, "int16_t must be exactly 16 bits");
_Static_assert(sizeof(int8_t)  == 1, "int8_t  must be exactly 8 bits");

static bool quit;
static uint32_t fps;
GwClient *client;

static void sighandler(int signum)
{
    (void)signum;
    quit = true;
}

static void main_loop(void)
{
    uint32_t frame_count = 0;
    struct timespec t0, t1;
    struct timespec last_frame_time;

    while (!quit) {
        frame_count += 1;
        get_wall_clock_time(&t0);

        long frame_delta_nsec = time_diff_nsec(&t0, &last_frame_time);
        msec_t frame_delta = frame_delta_nsec / 1000;

        //
        // Update the client asynchronous operations such as:
        // - Connecting to an GuildWars account.
        // - Connecting to a game server or transferring game server.
        //
        if (!(client->state.connected || client->state.connection_pending)) {
            if (!options.portal || portal_received_key)
                AccountLogin(client);
        }

        // @Enhancement:
        // We gotta think a bit more about theses condition
        if (client->state.connected &&
            NetConn_IsShutdown(&client->game_srv) &&
            !client->server_transfer.pending &&
            !client->state.playing_request_pending) {

            PlayCharacter(client, NULL, PlayerStatus_Online);
        }

        if (NetConn_IsShutdown(&client->game_srv) && client->server_transfer.pending) {
            TransferGameServer(client);
        }

        //
        // Update the network connection.
        // It will send the data out and check if new data was received.
        //
        NetConn_Update(&client->auth_srv);
        NetConn_Update(&client->game_srv);

        //
        // Update worlds
        // @TODO: Rename to "simulate world"?
        //
        client_frame_update(client, frame_delta);

        //
        // trigger all timers event
        //
        process_timers();

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

    if (wcsnul(options.charname) || wcsnul(options.password)
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
        if (!portal_init()) {
            LogError("portal_init failed");
            return 1;
        }
    }

    client = malloc(sizeof(*client));
    init_client(client);

    AuthSrv_RegisterCallbacks(&client->auth_srv);
    GameSrv_RegisterCallbacks(&client->game_srv);

    if (!init_auth_connection(client, options.auth_srv)) {
        LogInfo("Auth connection try failed.");
        exit(1);
    }

    {
        kstr_read_ascii(&client->email, options.email, _countof(options.email));
        kstr_read_ascii(&client->charname, options.charname, _countof(options.charname));

        DECLARE_KSTR(password, 100);
        kstr_read_ascii(&password, options.password, _countof(options.password));

        if (options.portal) {
            portal_login(&client->email, &password);
        } else {
            compute_pswd_hash(&client->email, &password, client->password);
        }

    #if defined(HEADQUARTER_CONSOLE)
        SetConsoleTitleA(options.charname);
    #endif
    }

    if (!plugin_load(options.script)) {
        LogError("Couldn't load the plugin '%s'", options.script);
        return 1;
    }

    main_loop();

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
