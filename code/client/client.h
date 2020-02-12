#ifdef CORE_CLIENT_H
#error "client.h included more than once"
#endif
#define CORE_CLIENT_H

typedef struct Transaction {
    uint32_t id;
    bool enable;
    int error_code;
    thread_event_t event;
    struct GwClient *client;
} Transaction;
typedef array(Transaction) TransactionArray;

typedef enum Screen {
    SCREEN_HANDSHAKE,
    SCREEN_LOGIN_SCREEN,
    SCREEN_CHARACTER_SELECT,
    SCREEN_LOADING_MAP,
    SCREEN_INGAME,
    SCREEN_SPECTACTING,
    SCREEN_CINEMATIC
} Screen;

typedef struct SalvageSession {
    int16_t salvage_session_id;
    bool    is_open;

    int32_t n_upgrades;
    Item   *upgrades[3];
} SalvageSession;

typedef struct GwClient {
    Connection          auth_srv;
    Connection          game_srv;

    thread_mutex_t      mutex;

    uuid_t              uuid;
    string              email;
    string              character;
    char                password[20]; // sha1(pswd:mail)
    char                email_buffer[100];
    char                charac_buffer[64];

    Screen              screen;
    bool                ingame;
    bool                connected;
    bool                try_changing_zone;

    // Server keys.
    uint32_t            token1; // world id
    uint32_t            token2; // player id
    uint32_t            static_salt;

    TransactionArray    transactions;
    uint32_t            next_transaction_id_to_issue;

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
} GwClient;

void init_client(GwClient *client);
void retire_transaction(Transaction *trans);
Transaction *issue_next_transaction(GwClient *client);

static void release_connections(void);

void client_frame_update(GwClient *client, msec_t diff);

void compute_pswd_hash(string _email, string _pswd, char digest[20]);

// If `pswd` is NULL, it will use the hash in `client->password`
void AccountLogout(GwClient *client);
void AccountConnect(GwClient *client, string email, string pswd, string pseudo);
void PortalAccountConnect(GwClient *client, uuid_t user_id, uuid_t session_id, string charname);

// Id `name` is NULL, it will uses the last played character
void GameSrv_PlayCharacter(GwClient *client, string name, PlayerStatus status);
void GameSrv_LogoutToCharselect(GwClient *cient);
