#ifdef CORE_CLIENT_C
#error "client.c included more than once"
#endif
#define CORE_CLIENT_C

static void init_client(GwClient *client)
{
    memzero(client, sizeof(*client));

    thread_mutex_init(&client->mutex);

    kstr_init(&client->email, client->email_buffer, 0, _countof(client->email_buffer));
    kstr_init(&client->charname, client->charname_buffer, 0, _countof(client->charname_buffer));

    array_init(client->characters, 8);
    array_init(client->merchant_items, 32);
    array_init(client->trade_session.trader_items, 7);
    array_init(client->trade_session.player_items, 7);
    array_init2(client->titles, 64);

    // @Cleanup: What is the practical max amount of button in the game.
    array_init(client->dialog.buttons, 16);

    init_chat(&client->chat);

    client->next_transaction_id = 1;

    init_object_manager(&client->object_mgr);
    init_event_manager(&client->event_mgr);

    init_connection(&client->auth_srv, client);
    init_connection(&client->game_srv, client);
}

static uint32_t issue_next_transaction(GwClient *client, AsyncType type)
{
    AsyncRequest *request = array_push(client->requests, 1);
    request->trans_id = ++client->next_transaction_id;
    request->type = type;
    return request->trans_id;
}

void compute_pswd_hash(struct kstr *email, struct kstr *pswd, char digest[20])
{
    uint16_t buffer[256];
    size_t i = 0;
    for (size_t j = 0; j < pswd->length; j++, i++)
        buffer[i] = htole16(pswd->buffer[j] & 0xffff);
    for (size_t j = 0; j < email->length; j++, i++)
        buffer[i] = htole16(email->buffer[j] & 0xffff);
    Sha1(buffer, i * 2, digest);
}

void ContinueAccountLogin(GwClient *client, uint32_t error_code)
{
    assert(!client->state.connected);
    assert(client->state.connection_pending);

    if (error_code != 0) {
        LogError("AccountLogin failed");
        client->state.connection_pending = false;
        return;
    }

    uint32_t trans_id = issue_next_transaction(client, AsyncType_SendHardwareInfo);
    AuthSrv_HardwareInfo(&client->auth_srv);
    AuthSrv_AskServerResponse(&client->auth_srv, trans_id);
}

void ContinueSendHardwareInfo(GwClient *client, uint32_t error_code)
{
    assert(!client->state.connected);
    assert(client->state.connection_pending);

    client->state.connected = true;
}

void OldAccountConnect(GwClient *client, struct kstr *email, struct kstr *pswd, struct kstr *charname)
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

    if (!client->state.session_ready) {
        LogError("You must connect & send computer infos before calling OldAccountConnect");
        return;
    }

    // @Remark: If pswd isn't set, it will use the static hash of the password.
    // It must be computed before. (compute_pswd_hash can help)
    if (pswd->length) {
        compute_pswd_hash(email, pswd, client->password);
    }

    uint32_t client_salt = (uint32_t)time(NULL);
    ((uint32_t *)buffer)[0] = client_salt;
    ((uint32_t *)buffer)[1] = client->static_salt;
    memcpy(buffer + 8, client->password, 20);
    Sha1(buffer, 28, digest);

    uint32_t trans_id = issue_next_transaction(client, AsyncType_AccountLogin);

    AccountLogin packet = NewPacket(AUTH_CMSG_ACCOUNT_LOGIN);
    packet.transaction_id = trans_id;
    packet.client_salt = client_salt;
    memcpy(packet.password, digest, 20);

    size_t max_email = _countof(packet.email) - 1;
    if (max_email < email->length) {
        LogError("Email is too long. Length: %zu, Max: %zu", email->length, max_email);
        return;
    }
    memcpy(packet.email, email->buffer, email->length * 2);
    packet.email[email->length] = 0;

    assert(_countof(packet.charname1) == _countof(packet.charname2));
    if (_countof(packet.charname1) < charname->length) {
        LogError("Charname is too long. Length: %zu, Max: %zu", charname->length, _countof(packet.charname1));
        return;
    }

    memcpy(packet.charname1, charname->buffer, charname->length * 2); // playing character
    memcpy(packet.charname2, charname->buffer, charname->length * 2);
    packet.charname1[charname->length] = 0;
    packet.charname2[charname->length] = 0;

    LogDebug("OldAccountConnect: {trans_id: %lu}", trans_id);
    SendPacket(auth, sizeof(packet), &packet);
}

void PortalAccountConnect(GwClient *client, uuid_t user_id, uuid_t session_id, struct kstr *charname)
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
    if (!client->state.session_ready) {
        LogError("You must connect & send computer infos before calling PortalAccountConnect");
        return;
    }

    uint32_t trans_id = issue_next_transaction(client, AsyncType_AccountLogin);

    PortalAccountLogin packet = NewPacket(AUTH_CMSG_PORTAL_ACCOUNT_LOGIN);
    packet.transaction_id = trans_id;
    uuid_enc_le(packet.user_id, user_id);
    uuid_enc_le(packet.session_id, session_id);
    
    assert(_countof(packet.charname1) == _countof(packet.charname2));
    if (_countof(packet.charname1) < charname->length) {
        LogError("Charname is too long. Length: %zu, Max: %zu", charname->length, _countof(packet.charname1));
        return;
    }

    memcpy(packet.charname1, charname->buffer, charname->length * 2); // playing character
    memcpy(packet.charname2, charname->buffer, charname->length * 2);
    packet.charname1[charname->length] = 0;
    packet.charname2[charname->length] = 0;

    LogDebug("PortalAccountConnect: {trans_id: %lu}", trans_id);
    SendPacket(&client->auth_srv, sizeof(packet), &packet);
}

void AccountLogin(GwClient *client)
{
    if (options.portal) {
        if (!portal_received_key) {
            LogError("GwLoginClient didn't replied with the connection key yet");
            return;
        }
        PortalAccountConnect(client, portal_user_id, portal_session_id, &client->charname);
    } else {
        OldAccountConnect(client, &client->email, NULL, &client->charname);
    }
    client->state.connection_pending = true;
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
    // client->state.connected = false;

    SendPacket(&client->auth_srv, sizeof(packet), &packet);
}

void ContinuePlayCharacter(GwClient *client, uint32_t error_code)
{
    assert(client->state.playing_request_pending);

    if (error_code != 0) {
        LogInfo("ContinuePlayCharacter failed (Code=%03d)", error_code);
        return;
    }

    // @Cleanup:
    // We could teleport to the last map of the character, but for that
    // we would need to find the type from data. Can be done with `AreaInfo`.
    extern void begin_travel(GwClient *client);
    begin_travel(client);

    // Character *cc = client->current_character; // can use to find the map
    uint32_t trans_id = issue_next_transaction(client, AsyncType_PlayCharacter);
    LogDebug("AuthSrv_RequestInstance: {trans_id: %lu}", trans_id);
    AuthSrv_RequestInstance(&client->auth_srv, trans_id, 248, 0, 0, 0, 0);
}

void PlayCharacter(GwClient *client, struct kstr *name, PlayerStatus status)
{
    assert(client->state.connected);
    assert(!client->state.playing_request_pending);

    client->state.playing_request_pending = true;

    if (client->player_status != status) {
        AuthSrv_SetPlayerStatus(&client->auth_srv, status);
        client->player_status = status;
    }

    Character *cc = client->current_character;
    if (name && kstr_compare(&cc->name, name)) {
        uint32_t trans_id = issue_next_transaction(client, AsyncType_ChangeCharacter);

        LogDebug("AuthSrv_ChangeCharacter {trans_id: %lu}", trans_id);
        AuthSrv_ChangeCharacter(&client->auth_srv, trans_id, name);
    } else {
        ContinuePlayCharacter(client, 0);
    }
}

void GameSrv_Disconnect(GwClient *client)
{
    assert(client && client->game_srv.secured);

    client->state.ingame = false;
    NetConn_Shutdown(&client->game_srv);

    Packet packet = NewPacket(GAME_CMSG_DISCONNECT);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_LogoutToCharselect(GwClient *client)
{
    if (client->player_status != PlayerStatus_Offline) {
        AuthSrv_SetPlayerStatus(&client->auth_srv, PlayerStatus_Offline);
        client->player_status = PlayerStatus_Offline;
    }

    // @Cleanup:
    // We have to ensure that whenever we lose the connection or connect to a game server
    // we reset all the necessary states.
    GameSrv_Disconnect(client);
}

void GameSrv_SkipCinematic(GwClient *client)
{
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

    assert(client && client->game_srv.secured);

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

    if (client->state.ingame && ((frame_time - game->last_tick_time) >= 5*1000)) {
        GameSrv_HeartBeat(game);
        game->last_tick_time = frame_time;
    }

    if (client->world.hash)
        world_update(&client->world, diff);
    thread_mutex_unlock(&client->mutex);
}
