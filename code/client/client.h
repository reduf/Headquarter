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

typedef struct AsyncServerTransfer {
    bool pending;
    uint32_t map_id;
    uint32_t world_id;
    uint32_t player_id;
    struct sockaddr host;
} AsyncServerTransfer;

//
// Connecting to an account follow the following flow:
// 1. Connect to the authentication server.
// 2. Send the computer information. (AUTH_CMSG_SEND_COMPUTER_INFO & AUTH_CMSG_SEND_COMPUTER_HASH)
// 3. Server replies with session information. (AUTH_SMSG_SESSION_INFO)
// The authentication server is ready to receive further packet.
// 4. Begin the connection without or with GwLoginClient.dll. (AUTH_CMSG_ACCOUNT_LOGIN or AUTH_CMSG_PORTAL_ACCOUNT_LOGIN)
// 5. Send hardware information and ask for a reply. (Probably to know when the server received the hardware info)

typedef struct ClientState {
    // When set, the client is on a game server.
    bool ingame : 1;

    // When set, the auth server is ready for being played.
    bool connected : 1;

    // Set as soon as the flow to connect starts
    bool connection_pending : 1;

    // When set, the auth server is ready for a connection request.
    bool session_ready : 1;

    // playing pending
    bool playing_request_pending : 1;
} ClientState;

typedef struct GwClient {
    Connection          auth_srv;
    Connection          game_srv;

    thread_mutex_t      mutex;

    uuid_t              uuid;

    struct kstr         email;
    struct kstr         charname;
    char                password[20]; // sha1(pswd:mail)
    uint16_t            email_buffer[100];
    uint16_t            charname_buffer[64];

    ClientState         state;
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

    Chat                chat;
    World               world;

    ArrayTitle          titles;
    ArrayQuest          quests;
    Inventory           inventory;
    Player             *player;

    EventManager        event_mgr;
    ObjectManager       object_mgr;

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
    AsyncServerTransfer server_transfer;
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
void PortalAccountConnect(GwClient *client, uuid_t user_id, uuid_t session_id, struct kstr *charname);

void ContinueAccountLogin(GwClient *client, uint32_t error_code);
void ContinueSendHardwareInfo(GwClient *client, uint32_t error_code);
void ContinuePlayCharacter(GwClient *client, uint32_t error_code);

// Id `name` is NULL, it will uses the last played character
void GameSrv_PlayCharacter(GwClient *client, struct kstr *name, PlayerStatus status);
void GameSrv_LogoutToCharselect(GwClient *cient);
