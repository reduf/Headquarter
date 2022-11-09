#ifdef CORE_CLIENT_H
#error "client.h included more than once"
#endif
#define CORE_CLIENT_H

typedef struct SalvageSession {
    uint16_t    salvage_session_id;
    bool        is_open;

    uint32_t    n_upgrades;
    Item       *upgrades[3];
} SalvageSession;

typedef enum AsyncType {
    AsyncType_None,
    AsyncType_AccountLogin,
    AsyncType_PlayCharacter,
    AsyncType_ServerTransfer,
    AsyncType_SendHardwareInfo,
    AsyncType_ChangeCharacter,

    AsyncType_Count,
} AsyncType;

typedef struct AsyncRequest {
    uint32_t trans_id;
    AsyncType type;
} AsyncRequest;
typedef array(AsyncRequest) ArrayAsyncRequest;

typedef struct GameServerTransfer {
    uint32_t map_id;
    uint32_t world_id;
    uint32_t player_id;
    struct sockaddr host;
} GameServerTransfer;

typedef array(uint32_t) array_uint32_t;

//
// AwaitState is used to connect and start playing a character.
// Every state should be added here to precisely know what is
// happening at every moments.
//
typedef enum AwaitState {
    // AwaitNothing means that we are done with our action,
    // generally, because their was an error.
    AwaitNothing,

    // We are awaiting the connection to the auth server.
    // It's currently unused, because we do the handshake synchronously.
    AwaitAuthServer,

    // After connecting to the auth server, we are waiting to receive
    // "session info" to proceed. It's necessary for the old auth.
    AwaitSessionInfo,

    // We received the "session info", and we are ready to connect
    // to the account.
    AwaitAccountConnect,

    // We sent a request to connect, wait for the answer.
    AwaitAccountConnection,

    // If we successfully connected, we have to send our hardware info
    // and we have to wait for the reply to start playing.
    AwaitHardwareInfoReply,

    // We successfully connected to the authentication server and it's
    // ready to receive our play request.
    AwaitPlayCharacter,

    // If we need to change character, we have to wait for the request
    // to succeed.
    AwaitChangeCharacter,

    // We sent the request to start playing and we are waiting to receive
    // the server on which we will be playing.
    AwaitGameServerInfo,

    // If we were playing and we choose to disconnect or we went through a
    // portal so we are disconnecting from the current game server, we wait
    // to flush the socket stream and properly close the communication.
    AwaitGameServerDisconnect,

    // We have the information to connect to a game server and we can now
    // proceed to the connection.
    AwaitGameServerTransfer,
} AwaitState;

typedef struct GwClient {
    Connection          auth_srv;
    Connection          game_srv;

    thread_mutex_t      mutex;

    struct uuid         uuid;

    struct kstr         email;
    struct kstr         charname;
    char                password[20]; // sha1(pswd:mail)
    uint16_t            email_buffer[100];
    uint16_t            charname_buffer[64];

    struct uuid         portal_token;
    struct uuid         portal_user_id;

    AwaitState          state;
    bool                ingame;
    bool                loading;
    bool                connected;
    bool                try_changing_zone;

    // Server keys.
    uint32_t            world_id;
    uint32_t            player_id;
    uint32_t            static_salt;

    uint32_t            next_transaction_id;
    ArrayAsyncRequest   requests;

    PlayerStatus        player_status;
    AgentId             player_agent_id;

    FriendArray         friends;
    CharacterArray      characters;
    Character          *current_character;
    Character          *pending_character;

    GuildMemberUpdate   guild_member_update;

    Chat                chat;
    World               world;

    ArrayTitle          titles;
    ArrayQuest          quests;
    Inventory           inventory;
    Player             *player;

    EventManager        event_mgr;

    ArrayItem           tmp_merchant_items;
    array_uint32_t      tmp_merchant_prices;
    ArrayItem           merchant_items;
    AgentId             merchant_agent_id;
    AgentId             interact_with;

    DialogInfo          dialog;
    TradeSession        trade_session;
    SalvageSession      salvage_session;

    // @Remark:
    // We receive those information before we know the player id, so we store
    // them here and then when we get `set player` we update the player structure.
    PlayerHero          player_hero;

    //
    // There is a series of action that need to have server replies, before continuing.
    //
    GameServerTransfer  server_transfer;
} GwClient;
extern GwClient *client;

void init_client(GwClient *client);
uint32_t issue_next_transaction(GwClient *client, AsyncType type);

void client_frame_update(GwClient *client, msec_t diff);

void compute_pswd_hash(struct kstr *email, struct kstr *pswd, char digest[20]);

// If `pswd` is NULL, it will use the hash in `client->password`
void AccountLogin(GwClient *client);
void AccountLogout(GwClient *client);
void OldAccountConnect(GwClient *client, struct kstr *email, struct kstr *pswd, struct kstr *pseudo);
void PortalAccountConnect(GwClient *client, struct uuid *user_id, struct uuid *token, struct kstr *charname);

void PlayCharacter(GwClient *client, struct kstr *name, PlayerStatus status);

void ContinueAccountLogin(GwClient *client, uint32_t error_code);
void ContinueSendHardwareInfo(GwClient *client, uint32_t error_code);
void ContinuePlayCharacter(GwClient *client, uint32_t error_code);
void ContinueChangeCharacter(GwClient *client, uint32_t error_code);

// Id `name` is NULL, it will uses the last played character
void GameSrv_PlayCharacter(GwClient *client, struct kstr *name, PlayerStatus status);
void GameSrv_LogoutToCharselect(GwClient *cient);
