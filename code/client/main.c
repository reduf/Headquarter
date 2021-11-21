#ifdef CORE_MAIN_C
#error "main.c included more than once"
#endif
#define CORE_MAIN_C

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
        msec_t frame_delta = frame_delta_nsec / 1000000;

        //
        // Update the client asynchronous operations such as:
        // - Connecting to an GuildWars account.
        // - Connecting to a game server or transferring game server.
        //
        if (client->state == AwaitAccountConnect) {
            if (!options.newauth || portal_received_key)
                AccountLogin(client);
        }

        // @Enhancement:
        // We gotta think a bit more about theses condition
        if (client->state == AwaitPlayCharacter) {
            assert(NetConn_IsShutdown(&client->game_srv));
            PlayCharacter(client, NULL, PlayerStatus_Online);
        }

        if (client->state == AwaitGameServerDisconnect &&
            NetConn_IsShutdown(&client->game_srv))
        {
            client->ingame = false;
            client->loading = true;
            client->state = AwaitGameServerTransfer;
        }

        if (client->state == AwaitGameServerTransfer &&
            NetConn_IsShutdown(&client->game_srv)) {

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

    if (options.verbose)
        log_set_level(LOG_DEBUG);
    if (options.trace)
        log_set_level(LOG_TRACE);

#ifdef _WIN32
    timeBeginPeriod(1);
#endif
    fps = 60;

    Network_Init();

    if (options.newauth) {
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
        kstr_read_ascii(&client->email, options.email, ARRAY_SIZE(options.email));
        kstr_read_ascii(&client->charname, options.charname, ARRAY_SIZE(options.charname));

        DECLARE_KSTR(password, 100);
        kstr_read_ascii(&password, options.password, ARRAY_SIZE(options.password));

        if (options.newauth) {
            portal_login(&client->email, &password);
        } else {
            compute_pswd_hash(&client->email, &password, client->password);
        }

    #if defined(HEADQUARTER_CONSOLE)
        SetConsoleTitleA(options.charname);
    #endif
    }

    char full_script_path[1024];
    int length = dlldir(full_script_path, sizeof(full_script_path));
    snprintf(&full_script_path[length], sizeof(full_script_path) - length, "/%s", options.script);
    if (!plugin_load(full_script_path)) {
        LogError("Couldn't load the plugin '%s'", full_script_path);
        return 1;
    }
    LogInfo("Plugin loaded, %s\n", full_script_path);

    main_loop();

    while (!list_empty(&plugins)) {
        Plugin *it = plugin_first();
        plugin_unload(it);
    }

    if (options.newauth) {
        portal_cleanup();
    }

    Network_Shutdown();
    printf("Quit cleanly !!\n");

    return 0;
}
