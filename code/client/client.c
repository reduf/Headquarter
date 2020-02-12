#ifdef CORE_CLIENT_C
#error "client.c included more than once"
#endif
#define CORE_CLIENT_C

static inline void init_client(GwClient *client)
{
    memzero(client, sizeof(GwClient));
    client->screen = SCREEN_HANDSHAKE;

    thread_mutex_init(&client->mutex);

    array_init(client->characters, 8);
    array_init(client->merchant_items, 32);
    array_init(client->trade_session.trader_items, 7);
    array_init(client->trade_session.player_items, 7);
    array_init2(client->titles, 64);

    // @Cleanup: What is the practical max amount of button in the game.
    array_init(client->dialog.buttons, 16);

    init_chat(&client->chat);

    client->next_transaction_id_to_issue = 0;
    array_init2(client->transactions, 256);
    for (size_t i = 0; i < client->transactions.size; i++)
        array_at(client->transactions, i).id = i;

    init_object_manager(&client->object_mgr);
    init_event_manager(&client->event_mgr);
}

static inline void retire_transaction(Transaction *trans)
{
    assert(trans != NULL);
    if (!trans->enable) return;

    trans->client = NULL;
    trans->error_code = 0;
    thread_event_destroy(&trans->event);

    trans->enable = false;
}

static inline Transaction *issue_next_transaction(GwClient *client)
{
    TransactionArray *transactions = &client->transactions;

    int trans_id = client->next_transaction_id_to_issue;
    client->next_transaction_id_to_issue = (trans_id + 1) % client->transactions.size;

    assert(array_inside(*transactions, trans_id));
    Transaction *trans = &array_at(*transactions, trans_id);
    assert(trans_id == trans->id);

    trans->client = client;
    thread_event_init(&trans->event);

    trans->enable = true;
    return trans;
}

void compute_pswd_hash(string _email, string _pswd, char digest[20])
{
    // @Cleanup: Support utf-8
    wchar_t email[64], pswd[100];
    if (!strtowcs(email, _email.bytes, MIN(_email.count+1, 64))) {
        LogError("Couldn't decode utf-8 string of email");
        return;
    }
    if (!strtowcs(pswd, _pswd.bytes, MIN(_pswd.count+1, 64))) {
        LogError("Couldn't decode utf-8 string of password");
        return;
    }
    uint16_t buffer[512];
    size_t i = 0;
    for (size_t j = 0; pswd[j]; j++, i++)
        buffer[i] = htole16(pswd[j] & 0xffff);
    for (size_t j = 0; email[j]; j++, i++)
        buffer[i] = htole16(email[j] & 0xffff);
    Sha1(buffer, i * 2, digest);
}

static void do_post_connection(GwClient *client)
{
    Transaction *trans = issue_next_transaction(client);
    AuthSrv_HardwareInfo(&client->auth_srv);
    AuthSrv_AskServerResponse(&client->auth_srv, trans->id);

    if (thread_event_wait(&trans->event) != 0) {
        LogError("'thread_event_wait' failed in 'AccountConnect' waiting for Hardware response");
        retire_transaction(trans);
        return;
    }

    client->connected = true;
    client->screen = SCREEN_CHARACTER_SELECT;
}

void AccountConnect(GwClient *client, string email, string pswd, string charname)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t transaction_id;
        uint32_t client_salt;
        char     password[20];
        uint16_t email[64];
        uint16_t charname1[20]; // playing character
        uint16_t charname2[20]; // secret question
    } AccountLogin;
#pragma pack(pop)

    char buffer[256];
    char digest[20];

    Connection *auth = &client->auth_srv;

    // @Cleanup:
    // This check is a little bit weird, we would want to do something like
    // `if (client->screen == SCREEN_LOGIN_SCREEN)`, but when we are ingame it
    // we should be ingame rather than at the handshake.
    if (client->screen == SCREEN_HANDSHAKE) {
        LogError("You must connect & send computer infos before calling AccountConnect.");
        return;
    }

    // @Remark: If pswd isn't set, it will use the static hash of the password.
    // It must be computed before. (compute_pswd_hash can help)
    if (pswd.count) compute_pswd_hash(email, pswd, client->password);

    uint32_t client_salt = (uint32_t)time(NULL);
    ((uint32_t *)buffer)[0] = client_salt;
    ((uint32_t *)buffer)[1] = client->static_salt;
    memcpy(buffer + 8, client->password, 20);
    Sha1(buffer, 28, digest);

    Transaction *trans = issue_next_transaction(client);

    AccountLogin packet = NewPacket(AUTH_CMSG_ACCOUNT_LOGIN);
    packet.transaction_id = trans->id;
    packet.client_salt = client_salt;
    memcpy(packet.password, digest, 20);
    utf8_to_unicode16(packet.email,   64, email.bytes,  email.count);
    utf8_to_unicode16(packet.charname1, 20, charname.bytes, charname.count); // playing character
    utf8_to_unicode16(packet.charname2, 20, charname.bytes, charname.count);

    SendPacket(auth, sizeof(packet), &packet);

    if (thread_event_wait(&trans->event) != 0) {
        LogError("'thread_event_wait' failed in 'AccountConnect' waiting for connect response");
        retire_transaction(trans);
        return;
    }

    if (trans->error_code != 0) {
        if (trans->error_code == 11) {
            // @Enhancement: How do we quite cleanly ?
            exit(1);
        }
        retire_transaction(trans);
        return;
    }

    do_post_connection(client);
}

void PortalAccountConnect(GwClient *client, uuid_t user_id, uuid_t session_id, string charname)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint32_t transaction_id;
        uuid_t   user_id;
        uuid_t   session_id;
        uint16_t charname1[20]; // playing character
        uint16_t charname2[20]; // secret question
    } PortalAccountLogin;
#pragma pack(pop)

    // @Cleanup:
    // This check is a little bit weird, we would want to do something like
    // `if (client->screen == SCREEN_LOGIN_SCREEN)`, but when we are ingame it
    // we should be ingame rather than at the handshake.
    if (client->screen == SCREEN_HANDSHAKE) {
        LogError("You must connect & send computer infos before calling AccountConnect.");
        return;
    }

    Transaction *trans = issue_next_transaction(client);

    PortalAccountLogin packet = NewPacket(AUTH_CMSG_PORTAL_ACCOUNT_LOGIN);
    packet.transaction_id = trans->id;
    uuid_enc_le(packet.user_id, user_id);
    uuid_enc_le(packet.session_id, session_id);
    utf8_to_unicode16(packet.charname1, 20, charname.bytes, charname.count); // playing character
    utf8_to_unicode16(packet.charname2, 20, charname.bytes, charname.count);

    SendPacket(&client->auth_srv, sizeof(packet), &packet);

    if (thread_event_wait(&trans->event) != 0) {
        LogError("'thread_event_wait' failed in 'AccountConnect' waiting for connect response");
        retire_transaction(trans);
        return;
    }

    if (trans->error_code != 0) {
        if (trans->error_code == 11) {
            // @Enhancement: How do we quite cleanly ?
            exit(1);
        }
        retire_transaction(trans);
        return;
    }

    do_post_connection(client);
}

void AccountLogout(GwClient *client)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t unk0;
    } Logout;
#pragma pack(pop)

    Logout packet = NewPacket(AUTH_CMSG_DISCONNECT);
    packet.unk0 = 0;

    // NetConn_FlagRemove(client->auth_srv);
    // client->connected = false;

    SendPacket(&client->auth_srv, sizeof(packet), &packet);
}

void GameSrv_PlayCharacter(GwClient *client, string name, PlayerStatus status)
{
    if (client->screen != SCREEN_CHARACTER_SELECT) {
        LogError("You must connect and be in the character selection before calling 'GameSrv_PlayCharacter'.");
        return;
    }

    if (name.count && str_cmp(client->current_character->name, name)) {
        Transaction *trans = issue_next_transaction(client);

        AuthSrv_ChangeCharacter(&client->auth_srv, trans->id, name);
        if (thread_event_wait(&trans->event) != 0) {
            LogError("Couldn't change character !");
            retire_transaction(trans);
            return;
        }

        if (trans->error_code != 0) {
            LogInfo("Could not play the character '%.*' (Code=%03d).",
                name.count, name.bytes, trans->error_code);
            retire_transaction(trans);
            return;
        }

        retire_transaction(trans);
    }

    if (client->player_status != status) {
        AuthSrv_SetPlayerStatus(&client->auth_srv, status);
        client->player_status = status;
    }

    // @Cleanup:
    // We could teleport to the last map of the character, but for that
    // we would need to find the type from data. Can be done with `AreaInfo`.
    extern void begin_travel(GwClient *client);
    begin_travel(client);

    // Character *cc = client->current_character; // can use to find the map
    AuthSrv_RequestInstance(&client->auth_srv, 0, 248, 3, 0, 0, 0);
}

void GameSrv_Disconnect(GwClient *client)
{
    assert(client && client->ingame);
    client->ingame = false;
    Packet packet = NewPacket(GAME_CMSG_DISCONNECT);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LogoutToCharselect(GwClient *client)
{
    GameSrv_Disconnect(client);
    if (client->player_status != PlayerStatus_Offline) {
        AuthSrv_SetPlayerStatus(&client->auth_srv, PlayerStatus_Offline);
        client->player_status = PlayerStatus_Offline;
    }

    // @Cleanup: This is wrong !!
    void world_reset(ObjectManager *mgr, World *world);
    reset_world(&client->world, &client->object_mgr);
    NetConn_Remove(&client->game_srv);
}

void GameSrv_SkipCinematic(GwClient *client)
{
    if (client->screen != SCREEN_CINEMATIC)
        return;
    Packet packet = NewPacket(GAME_CMSG_SKIP_CINEMATIC);
    SendPacket(&client->auth_srv, sizeof(packet), &packet);
}

void GameSrv_ChangeGold(GwClient *client, int gold_character, int gold_storage)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t gold_character;
        int32_t gold_storage;
    } ChangeGold;
#pragma pack(pop)

    assert(client && client->ingame);

    ChangeGold packet = NewPacket(GAME_CMSG_ITEM_CHANGE_GOLD);
    packet.gold_character = gold_character;
    packet.gold_storage = gold_storage;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_HeartBeat(Connection *conn);
void AuthSrv_HeartBeat(Connection *conn, msec_t tick);

void client_frame_update(GwClient *client, msec_t diff)
{
    thread_mutex_lock(&client->mutex);
    Connection *auth = &client->auth_srv;
    Connection *game = &client->game_srv;
    msec_t frame_time = time_get_ms();

    if (auth && ((frame_time - auth->last_tick_time) >= 120*1000)) {
        AuthSrv_HeartBeat(auth, frame_time);
        auth->last_tick_time = frame_time;
    }

    if (client->ingame && ((frame_time - game->last_tick_time) >= 5*1000)) {
        GameSrv_HeartBeat(game);
        game->last_tick_time = frame_time;
    }

    if (client->world.hash)
        world_update(&client->world, diff);
    thread_mutex_unlock(&client->mutex);
}

// @Cleanup: This should move else where, but we have to handle unexpected connection lost
static void release_connections(void)
{
    struct list *conns = &conns_to_release;
    Connection  *conn;
    while (!list_empty(conns)) {
        conn = list_first_entry(conns, Connection, node);
        list_remove(&conn->node);
        
        GwClient *client = cast(GwClient *)conn->data;
        if (conn == &client->auth_srv) {
            client->connected = false;
        } else if (conn == &client->game_srv) {
            // @Cleanup:
            // Adapt following line
            // client->game_srv = NULL;
            client->ingame = false;
        }

        NetConn_Reset(conn);
    }
}
