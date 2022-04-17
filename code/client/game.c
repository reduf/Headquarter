#ifdef CORE_GAME_C
#error "game.c included more than once"
#endif
#define CORE_GAME_C

static void HandlePingRequest(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_PING_REQUEST);
    assert(sizeof(Header) == psize);
    GameSrv_PingReply(conn);
}

static void HandlePingReply(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t ping;
    } PingReply;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_PING_REPLY);
    assert(sizeof(PingReply) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PingReply *pack = cast(PingReply *)packet;
    (void)pack;

    // @Remark:
    // In PingReply packet we send/receive the "reaction" of our code
    // (i.e. the time in ms that it take to start receiving new input)
    // So the actual ping (from send to recv) should be between
    // [latency/2 - srv->reaction - clt->reaction, latency/2 + srv->reaction + clt->reaction]
    conn->pong = client->world.world_time;
    conn->latency = conn->pong - conn->ping;
    // printf("latency %lu, pack {ping: %d}\n", conn->latency, pack->ping);
}

static void HandleAccountCurrency(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t unk0;
        int16_t unk1;
        int16_t unk2;
    } AccountCurrency;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ACCOUNT_CURRENCY);
    assert(sizeof(AccountCurrency) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    AccountCurrency *pack = cast(AccountCurrency *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

static void HandleUpdateActiveWeaponSet(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream;
        int8_t slot;
    } UpdateActiveWeapon;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_SET_ACTIVE_WEAPON_SET);
    assert(sizeof(UpdateActiveWeapon) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateActiveWeapon *pack = cast(UpdateActiveWeapon *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
    // LogInfo("UpdateActiveWeapon {slot: %d}", pack->slot);
}

static void HandleGoldCharacterAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream;
        int32_t gold;
    } AddGold;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GOLD_CHARACTER_ADD);
    assert(sizeof(AddGold) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    AddGold *pack = cast(AddGold *)packet;
    assert(client && client->game_srv.secured);

    client->inventory.gold_character += pack->gold;
}

static void HandleGoldStorageAdd(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream;
        int32_t gold;
    } UpdateGold;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GOLD_STORAGE_ADD);
    assert(sizeof(UpdateGold) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    UpdateGold *pack = cast(UpdateGold *)packet;
    assert(client && client->game_srv.secured);

    client->inventory.gold_storage += pack->gold;
}

void HandleReadyForMapSpawn(Connection *conn, size_t psize, Packet *packet)
{
    (void)psize;
    (void)packet;

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
    assert(!client->ingame);
    Packet spawn = NewPacket(GAME_CMSG_INSTANCE_LOAD_REQUEST_SPAWN);
    SendPacket(conn, sizeof(Packet), &spawn); // 2
}

void HandleGoldCharacterRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream;
        int32_t gold;
    } RemoveGold;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GOLD_CHARACTER_REMOVE);
    assert(sizeof(RemoveGold) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    RemoveGold *pack = cast(RemoveGold *)packet;
    assert(client && client->game_srv.secured);

    client->inventory.gold_character -= pack->gold;
}

static void HandleGoldStorageRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream;
        int32_t gold;
    } RemoveGold;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_GOLD_STORAGE_REMOVE);
    assert(sizeof(RemoveGold) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    RemoveGold *pack = cast(RemoveGold *)packet;
    assert(client && client->game_srv.secured);

    client->inventory.gold_storage -= pack->gold;
}

static void InstanceLoad_RequestData(Connection *conn)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t unk0;
        int8_t unk1;
    } RequestItems;
#pragma pack(pop)

    RequestItems items = NewPacket(GAME_CMSG_INSTANCE_LOAD_REQUEST_ITEMS);
    items.unk0 = 0;
    items.unk1 = 0;

    SendPacket(conn, sizeof(RequestItems), &items); // 1
}

void HandleInstanceLoadSpawnPoint(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t map_file;
        Vec2f pos;
        int16_t plane;
        uint8_t unk0;
        uint8_t is_cinematic;
        uint8_t unknow[8];
    } SpawnPoint;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_SPAWN_POINT);
    assert(sizeof(SpawnPoint) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SpawnPoint *pack = cast(SpawnPoint *)packet;
    assert(client && client->game_srv.secured);

#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t data[4];
    } RequestPlayers;
#pragma pack(pop)

    RequestPlayers players = NewPacket(GAME_CMSG_INSTANCE_LOAD_REQUEST_PLAYERS);
    players.data[0] = 4;
    players.data[1] = pack->map_file;

    SendPacket(conn, sizeof(RequestPlayers), &players); // 3
}

void HandleInstanceLoadInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent;
        int16_t map_id;
        int8_t is_explorable;
        int32_t district;
        int8_t language;
        int8_t is_observer;
    } InstanceInfo;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_INFO);
    assert(sizeof(InstanceInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    InstanceInfo *pack = cast(InstanceInfo *)packet;
    assert(client && client->game_srv.secured);

    World *world = &client->world;
    world->map_id = pack->map_id;
    world->district = pack->district;
    world->language = (DistrictLanguage)pack->language;

    LogInfo("InstanceInfo {agent: %d, map: %d, language: %d, district: %d}", pack->agent, pack->map_id, pack->language, pack->district);
    InstanceLoad_RequestData(conn);
}

void HandleInstanceLoadHead(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t b1;
        int8_t b2;
        int8_t b3;
        int8_t b4;
    } InstanceHead;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_HEAD);
    assert(sizeof(InstanceHead) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleInstanceLoadPlayerName(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t name[20];
    } InstancePlayerName;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_LOAD_PLAYER_NAME);
    assert(sizeof(InstancePlayerName) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    InstancePlayerName *pack = cast(InstancePlayerName *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

void HandleCinematicSkipEveryone(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_CINEMATIC_SKIP_EVERYONE);
    assert(sizeof(Packet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    World *world = &client->world;
    world->cinematic_skip_count = world->cinematic_member_count;
}

void HandleCinematicSkipCount(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint8_t skip_count;
        uint8_t member_count;
    } SkipCount;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CINEMATIC_SKIP_COUNT);
    assert(sizeof(SkipCount) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SkipCount *pack = cast(SkipCount *)packet;
    assert(client && client->game_srv.secured);

    client->world.cinematic_skip_count = pack->skip_count;
    client->world.cinematic_member_count = pack->member_count;
}

void HandleCinematicStart(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint8_t start; // 0=ready, 1=start
    } CinematicStart;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CINEMATIC_START);
    assert(sizeof(CinematicStart) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    CinematicStart *pack = cast(CinematicStart *)packet;
    assert(client && client->game_srv.secured);

    client->world.in_cinematic = true;
    if (pack->start) {
        Event event;
        Event_Init(&event, EventType_CinematicPlay);
        broadcast_event(&client->event_mgr, &event);
    }
}

void HandleCinematicDataEnd(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_CINEMATIC_DATA_END);
    assert(sizeof(Packet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleCinematicData(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t n_data;
        uint8_t data[1024];
    } CinematicData;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_CINEMATIC_DATA);
    assert(sizeof(CinematicData) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleCinematicEnd(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_CINEMATIC_END);
    assert(sizeof(Packet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    client->world.in_cinematic = false;
}

void HandleInstanceShowWin(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_INSTANCE_SHOW_WIN);
    assert(sizeof(Packet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleMissionAddGoal(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint16_t unk0[128];
        int32_t unk1;
    } Payload;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_MISSION_INFOBOX_ADD);
    assert(sizeof(Payload) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    client->world.objective_count += 1;
}
void HandleFriendListMessage(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t type;
        int32_t unk2;
        uint16_t subject[256];
        uint16_t message[256];
    } ErrorMessage;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_FRIENDLIST_MESSAGE);
    assert(sizeof(ErrorMessage) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    ErrorMessage* pack = cast(ErrorMessage*)packet;
    assert(client && client->game_srv.secured);
    
    LogInfo("Game server message %d %d %ls %ls", pack->type, pack->unk2, pack->subject, pack->message);

    Event params;
    Event_Init(&params, EventType_ChatMessage);
    params.ChatMessage.extra_id = 0;
    switch (pack->type) {
    case 2:
        // Outgoing whisper
        params.ChatMessage.channel = Channel_GWCA1;
        break;
    case 54:
        // Player is not online, slap a message in.
        params.ChatMessage.channel = Channel_Warning;
        params.ChatMessage.sender.buffer = pack->subject;
        wchar_t new_message[ARRAY_SIZE(pack->message)];
        wchar_t sender_ws[ARRAY_SIZE(pack->subject)];
        int length = 0;
        for (length = 0; length < ARRAY_SIZE(pack->subject) && pack->subject[length]; length++)
            sender_ws[length] = pack->subject[length];
        sender_ws[length] = 0;
        length = swprintf(new_message, ARRAY_SIZE(new_message), L"Player %s is not online", sender_ws);
        assert(length != -1);
        for (int i = 0; i < length; i++)
            pack->message[i] = new_message[i];
    }
    
    params.ChatMessage.sender.buffer = pack->subject;
    params.ChatMessage.sender.length = u16len(pack->subject, ARRAY_SIZE(pack->subject));

    params.ChatMessage.sender.buffer = pack->message;
    params.ChatMessage.sender.length = u16len(pack->message, ARRAY_SIZE(pack->message));

    broadcast_event(&client->event_mgr, &params);
}

void HandleMissionAddObjective(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t unk0;
        int32_t unk1;
        uint16_t unk2[128];
    } Payload;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_MISSION_OBJECTIVE_ADD);
    assert(sizeof(Payload) == psize);

    (void)conn;
}

void GameSrv_RegisterCallbacks(Connection *conn)
{
    array_init(&conn->handlers);
    array_resize(&conn->handlers, GAME_SMSG_COUNT);

    MsgHandler *handlers = conn->handlers.data;

    handlers[GAME_SMSG_PING_REQUEST]                    = HandlePingRequest;
    handlers[GAME_SMSG_PING_REPLY]                      = HandlePingReply;

    // instance
    handlers[GAME_SMSG_INSTANCE_SHOW_WIN]               = HandleInstanceShowWin;
    handlers[GAME_SMSG_INSTANCE_LOAD_HEAD]              = HandleInstanceLoadHead;
    handlers[GAME_SMSG_INSTANCE_COUNTDOWN_STOP]         = HandleInstanceCountdownStop;
    handlers[GAME_SMSG_INSTANCE_COUNTDOWN]              = HandleInstanceCountdown;
    handlers[GAME_SMSG_INSTANCE_LOAD_PLAYER_NAME]       = HandleInstanceLoadPlayerName;
    handlers[GAME_SMSG_INSTANCE_LOAD_SPAWN_POINT]       = HandleInstanceLoadSpawnPoint;
    handlers[GAME_SMSG_INSTANCE_LOAD_INFO]              = HandleInstanceLoadInfo;
    handlers[GAME_SMSG_TRANSFER_GAME_SERVER_INFO]       = HandleGameTransferInfo;
    handlers[GAME_SMSG_READY_FOR_MAP_SPAWN]             = HandleReadyForMapSpawn;
    handlers[GAME_SMSG_INSTANCE_CANT_ENTER]             = HandleCantEnterOutpost;
    handlers[GAME_SMSG_INSTANCE_TRAVEL_TIMER]           = HandleInstanceTravelTimer;
    handlers[GAME_SMSG_INSTANCE_LOAD_FINISH]            = HandleInstanceLoadFinish;
    handlers[GAME_SMSG_INSTANCE_LOADED]                 = HandleInstanceLoaded;
    handlers[GAME_SMSG_FRIENDLIST_MESSAGE]              = HandleFriendListMessage;

    handlers[GAME_SMSG_GOLD_CHARACTER_ADD]              = HandleGoldCharacterAdd;
    handlers[GAME_SMSG_GOLD_STORAGE_ADD]                = HandleGoldStorageAdd;
    handlers[GAME_SMSG_GOLD_CHARACTER_REMOVE]           = HandleGoldCharacterRemove;
    handlers[GAME_SMSG_GOLD_STORAGE_REMOVE]             = HandleGoldStorageRemove;

    // items
    handlers[GAME_SMSG_INVENTORY_ITEM_QUANTITY]         = HandleInventoryItemQuantity;
    handlers[GAME_SMSG_INVENTORY_ITEM_LOCATION]         = HandleInventoryItemLocation;
    handlers[GAME_SMSG_INVENTORY_CREATE_BAG]            = HandleInventoryCreateBag;
    handlers[GAME_SMSG_ITEM_STREAM_CREATE]              = HandleItemStreamCreate;
    handlers[GAME_SMSG_ITEM_STREAM_DESTROY]             = HandleItemStreamDestroy;
    handlers[GAME_SMSG_ITEM_WEAPON_SET]                 = HandleItemWeaponSet;
    handlers[GAME_SMSG_ITEM_SET_ACTIVE_WEAPON_SET]      = HandleUpdateActiveWeaponSet;
    handlers[GAME_SMSG_ITEM_GENERAL_INFO]               = HandleItemGeneralInfo;
    handlers[GAME_SMSG_ITEM_CHANGE_LOCATION]            = HandleItemChangeLocation;
    handlers[GAME_SMSG_ITEM_REMOVE]                     = HandleItemRemove;
    handlers[GAME_SMSG_ITEM_PRICE_QUOTE]                = HandleItemPriceQuote;
    handlers[GAME_SMSG_WINDOW_MERCHANT]                 = HandleWindowMerchant;
    handlers[GAME_SMSG_WINDOW_ITEM_STREAM_END]          = HandleWindowItemStreamEnd;
    handlers[GAME_SMSG_WINDOW_OWNER]                    = HandleWindowOwner;
    handlers[GAME_SMSG_WINDOW_ADD_ITEMS]                = HandleWindowAddItems;
    handlers[GAME_SMSG_ITEM_PRICES]                     = HandleWindowAddPrices;

    // salvage
    handlers[GAME_SMSG_ITEM_SALVAGE_SESSION_START]      = HandleSalvageSessionStart;
    handlers[GAME_SMSG_ITEM_SALVAGE_SESSION_CANCEL]     = HandleSalvageSessionCancel;
    handlers[GAME_SMSG_ITEM_SALVAGE_SESSION_DONE]       = HandleSalvageSessionDone;
    handlers[GAME_SMSG_ITEM_SALVAGE_SESSION_SUCCESS]    = HandleSalvageSessionSuccess;
    handlers[GAME_SMSG_ITEM_SALVAGE_SESSION_ITEM_KEPT]  = HandleSalvageSessionItemKept;

    // agents
    handlers[GAME_SMSG_AGENT_MOVEMENT_TICK]             = HandleAgentMovementTick;
    handlers[GAME_SMSG_AGENT_INSTANCE_TIMER]            = HandleAgentInstanceTimer;
    handlers[GAME_SMSG_AGENT_SPAWNED]                   = HandleAgentSpawned;
    handlers[GAME_SMSG_AGENT_DESPAWNED]                 = HandleAgentDespawned;
    handlers[GAME_SMSG_AGENT_SET_PLAYER]                = HandleAgentSetPlayer;
    handlers[GAME_SMSG_AGENT_UPDATE_DIRECTION]          = HandleAgentUpdateDirection;
    handlers[GAME_SMSG_AGENT_UPDATE_SPEED_BASE]         = HandleAgentUpdateSpeedBase;
    handlers[GAME_SMSG_AGENT_STOP_MOVING]               = HandleAgentStopMoving;
    handlers[GAME_SMSG_AGENT_MOVE_TO_POINT]             = HandleAgentMoveToPoint;
    handlers[GAME_SMSG_AGENT_UPDATE_SPEED]              = HandleAgentUpdateSpeedModifier;
    handlers[GAME_SMSG_AGENT_UPDATE_POSITION]           = HandleAgentUpdatePosition;
    handlers[GAME_SMSG_AGENT_UPDATE_ROTATION]           = HandleAgentUpdateRotation;
    handlers[GAME_SMSG_AGENT_UPDATE_DESTINATION]        = HandleAgentUpdateDestination;
    handlers[GAME_SMSG_AGENT_CREATE_PLAYER]             = HandleAgentCreatePlayer;
    handlers[GAME_SMSG_AGENT_DESTROY_PLAYER]            = HandleAgentDestroyPlayer;
    
    handlers[GAME_SMSG_AGENT_CREATE_NPC]                = HandleAgentCreateNPC;
    handlers[GAME_SMSG_AGENT_PINGED]                    = HandleAgentPingeg;
    handlers[GAME_SMSG_AGENT_UPDATE_EFFECTS]            = HandleAgentUpdateEffects;

    handlers[GAME_SMSG_NPC_UPDATE_PROPERTIES]           = HandleNPCUpdateProperties;
    handlers[GAME_SMSG_NPC_UPDATE_MODEL]                = HandleNPCUpdateModel;

    handlers[GAME_SMSG_PLAYER_UNLOCKED_PROFESSION]      = HandlePlayerUnlockedProfession;
    handlers[GAME_SMSG_PLAYER_UPDATE_PROFESSION]        = HandlePlayerUpdateProfession;

    // agent update
    handlers[GAME_SMSG_AGENT_UPDATE_NPC_NAME]           = HandleAgentUpdateNpcName;
    handlers[GAME_SMSG_AGENT_ATTR_UPDATE_INT]           = HandleAgentAttrUpdateInt;
    handlers[GAME_SMSG_AGENT_ATTR_UPDATE_INT_TARGET]    = HandleAgentAttrUpdateIntTarget;
    handlers[GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT]         = HandleAgentAttrUpdateFloat;
    handlers[GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT_TARGET]  = HandleAgentAttrUpdateFloatTarget;
    handlers[GAME_SMSG_AGENT_UPDATE_PROFESSION]         = HandleAgentUpdateProfession;

    // cinematic
    handlers[GAME_SMSG_CINEMATIC_START]                 = HandleCinematicStart;
    handlers[GAME_SMSG_CINEMATIC_DATA]                  = HandleCinematicData;
    handlers[GAME_SMSG_CINEMATIC_DATA_END]              = HandleCinematicDataEnd;
    handlers[GAME_SMSG_CINEMATIC_END]                   = HandleCinematicEnd;

    // trade
    handlers[GAME_SMSG_TRADE_REQUEST]                   = HandleTradeRequest;
    handlers[GAME_SMSG_TRADE_TERMINATE]                 = HandleTradeTerminate;
    handlers[GAME_SMSG_TRADE_OFFERED_COUNT]             = HandleTradeOfferedCount;
    handlers[GAME_SMSG_TRADE_RECEIVE_OFFER]             = HandleTradeReceiveOffer;
    handlers[GAME_SMSG_TRADE_ADD_ITEM]                  = HandleTradeAddItem;
    handlers[GAME_SMSG_TRADE_ACKNOWLEDGE]               = HandleTradeAcknowledge;
    handlers[GAME_SMSG_TRADE_ACCEPT]                    = HandleTradeAccept;
    handlers[GAME_SMSG_TRADE_CHANGE_OFFER]              = HandleTradeChangeOffer;

    // chat
    handlers[GAME_SMSG_CHAT_MESSAGE_CORE]               = HandleChatMessageCore;
    handlers[GAME_SMSG_CHAT_MESSAGE_LOCAL]              = HandleChatMessageLocal;
    handlers[GAME_SMSG_CHAT_MESSAGE_GLOBAL]             = HandleChatMessageGlobal;
    handlers[GAME_SMSG_CHAT_MESSAGE_SERVER]             = HandleChatMessageServer;

    // players
    handlers[GAME_SMSG_PLAYER_ATTR_SET]                 = HandlePlayerAttrSet;
    handlers[GAME_SMSG_PLAYER_ATTR_MAX_KURZICK]         = HandlePlayerAttrMaxKurzick;
    handlers[GAME_SMSG_PLAYER_ATTR_MAX_LUXON]           = HandlePlayerAttrMaxLuxon;
    handlers[GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR]       = HandlePlayerAttrMaxBalthazar;
    handlers[GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL]        = HandlePlayerAttrMaxImperial;
    handlers[GAME_SMSG_PLAYER_ATTR_UPDATE]              = HandlePlayerAttrUpdate;

    // skills
    handlers[GAME_SMSG_SKILLBAR_UPDATE_SKILL]           = HandleSkillbarUpdateSkill;
    handlers[GAME_SMSG_SKILLBAR_UPDATE]                 = HandleSkillbarUpdate;
    handlers[GAME_SMSG_SKILL_ADD_TO_WINDOWS_DATA]       = HandleSkillAddToWindowsData;
    handlers[GAME_SMSG_SKILL_ADD_TO_WINDOWS_END]        = HandleSkillAddToWindowsEnd;
    handlers[GAME_SMSG_SKILL_ACTIVATED]                 = HandleSkillActivated;
    handlers[GAME_SMSG_SKILL_ACTIVATE]                  = HandleSkillActivate;
    handlers[GAME_SMSG_SKILL_RECHARGE]                  = HandleSkillRecharge;
    handlers[GAME_SMSG_SKILL_RECHARGED]                 = HandleSkillRecharged;
    handlers[GAME_SMSG_HERO_BEHAVIOR]                   = HandleHeroBehavior;
    handlers[GAME_SMSG_HERO_SKILL_STATUS]               = HandleHeroSkillStatus;
    handlers[GAME_SMSG_HERO_SKILL_STATUS_BITMAP]        = HandleHeroSkillStatusBitmap;
    handlers[GAME_SMSG_AGENT_UPDATE_ATTRIBUTE]          = HandleAgentUpdateAttribute;

    // quests
    handlers[GAME_SMSG_QUEST_DESCRIPTION]               = HandleQuestDescription;
    handlers[GAME_SMSG_QUEST_ADD]                       = HandleQuestAdd;
    handlers[GAME_SMSG_QUEST_UPDATE_MARKER]             = HandleQuestUpdateMarker;
    handlers[GAME_SMSG_QUEST_REMOVE]                    = HandleQuestRemove;

    // titles
    handlers[GAME_SMSG_TITLE_RANK_DATA]                 = HandleTitleRankData;
    handlers[GAME_SMSG_TITLE_RANK_DISPLAY]              = HandleTitleRankDisplay;
    handlers[GAME_SMSG_TITLE_TRACK_INFO]                = HandleTitleTrackInfo;

    // missions
    handlers[GAME_SMSG_MISSION_INFOBOX_ADD]             = HandleMissionAddGoal;
    handlers[GAME_SMSG_MISSION_OBJECTIVE_ADD]           = HandleMissionAddObjective;

    // dialogs
    handlers[GAME_SMSG_DIALOG_BUTTON]                   = HandleDialogButton;
    handlers[GAME_SMSG_DIALOG_BODY]                     = HandleDialogBody;
    handlers[GAME_SMSG_DIALOG_SENDER]                   = HandleDialogSender;

    // guilds
    handlers[GAME_SMSG_GUILD_PLAYER_ROLE]               = HandleGuildPlayerRole;
    handlers[GAME_SMSG_GUILD_PLAYER_INFO]               = HandleGuildPlayerInfo;
    handlers[GAME_SMSG_GUILD_GENERAL_INFO]              = HandleGuildGeneralInfo;
    handlers[GAME_SMSG_GUILD_CHANGE_FACTION]            = HandleGuildChangeFaction;
    handlers[GAME_SMSG_GUILD_INVITE_RECEIVED]           = HandleGuildInviteReceived;
    handlers[GAME_SMSG_GUILD_PLAYER_CHANGE_COMPLETE]    = HandleGuildPlayerChangeComplete;
    handlers[GAME_SMSG_GUILD_CHANGE_PLAYER_CONTEXT]     = HandleGuildChangePlayerContext;
    handlers[GAME_SMSG_GUILD_CHANGE_PLAYER_TYPE]        = HandleGuildChangePlayerType;
    handlers[GAME_SMSG_GUILD_CHANGE_PLAYER_STATUS]      = HandleGuildChangePlayerStatus;
   // handlers[GAME_SMSG_GUILD_CHANGE_MEMBER_NAME]        = HandleGuildChangePlayerName;

    // effects
    handlers[GAME_SMSG_EFFECT_UPKEEP_ADDED]             = HandleEffectUpkeepAdded;
    handlers[GAME_SMSG_EFFECT_UPKEEP_REMOVED]           = HandleEffectUpkeepRemoved;
    handlers[GAME_SMSG_EFFECT_UPKEEP_APPLIED]           = HandleEffectUpkeepApplied;
    handlers[GAME_SMSG_EFFECT_APPLIED]                  = HandleEffectApplied;
    handlers[GAME_SMSG_EFFECT_RENEWED]                  = HandleEffectRenewed;
    handlers[GAME_SMSG_EFFECT_REMOVED]                  = HandleEffectRemoved;

    // parties
    handlers[GAME_SMSG_PARTY_SET_DIFFICULTY]            = HandlePartySetDifficulty;
    handlers[GAME_SMSG_PARTY_HERO_ADD]                  = HandlePartyHeroAdd;
    handlers[GAME_SMSG_PARTY_HERO_REMOVE]               = HandlePartyHeroRemove;
    handlers[GAME_SMSG_PARTY_INVITE_ADD]                = HandlePartyInviteAdd;
    handlers[GAME_SMSG_PARTY_JOIN_REQUEST]              = HandlePartyJoinRequest;
    handlers[GAME_SMSG_PARTY_INVITE_CANCEL]             = HandlePartyInviteCancel;
    handlers[GAME_SMSG_PARTY_REQUEST_CANCEL]            = HandlePartyRequestCancel;
    handlers[GAME_SMSG_PARTY_YOU_ARE_LEADER]            = HandlePartyYouAreLeader;
    handlers[GAME_SMSG_PARTY_PLAYER_ADD]                = HandlePartyPlayerAdd;
    handlers[GAME_SMSG_PARTY_PLAYER_REMOVE]             = HandlePartyPlayerRemove;
    handlers[GAME_SMSG_PARTY_PLAYER_READY]              = HandlePartyPlayerReady;
    handlers[GAME_SMSG_PARTY_CREATE]                    = HandlePartyCreate;
    handlers[GAME_SMSG_PARTY_MEMBER_STREAM_END]         = HandlePartyMemberStreamEnd;
    handlers[GAME_SMSG_PARTY_DEFEATED]                  = HandlePartyDefeated;

    // party search
    handlers[GAME_SMSG_PARTY_SEARCH_REQUEST_JOIN]       = HandlePartySearchRequestJoin;
    handlers[GAME_SMSG_PARTY_SEARCH_REQUEST_DONE]       = HandlePartySearchRequestDone;
    handlers[GAME_SMSG_PARTY_SEARCH_ADVERTISEMENT]      = HandlePartySearchAdvertisement;
    handlers[GAME_SMSG_PARTY_SEARCH_SEEK]               = HandlePartySearchSeek;
    handlers[GAME_SMSG_PARTY_SEARCH_REMOVE]             = HandlePartySearchRemove;
    handlers[GAME_SMSG_PARTY_SEARCH_SIZE]               = HandlePartySearchSize;
    handlers[GAME_SMSG_PARTY_SEARCH_TYPE]               = HandlePartySearchType;
}

void HandleCantEnterOutpost(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int8_t value;
    } Payload;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INSTANCE_CANT_ENTER);
    assert(sizeof(Payload) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    Payload *pack = cast(Payload *)packet;
    assert(client && client->game_srv.secured);

    LogInfo("Can't enter outpost: %d\n", pack->value);
    client->try_changing_zone = false;

    Event event;
    Event_Init(&event, EventType_WorldCantTravel);
    event.WorldCantTravel.value = pack->value;
    broadcast_event(&client->event_mgr, &event);
}

void GameSrv_PingReply(Connection *conn)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t value;
    } PingReply;
#pragma pack(pop)

    PingReply pack = NewPacket(GAME_CMSG_PING_REPLY);
    pack.value = 16;
    SendPacket(conn, sizeof(pack), &pack);
}

void GameSrv_PingRequest(Connection *conn)
{
    GwClient *client = cast(GwClient *)conn->data;
    conn->ping = client->world.world_time;

    Packet packet = NewPacket(GAME_CMSG_PING_REQUEST);
    SendPacket(conn, sizeof(packet), &packet);
}

void GameSrv_HeartBeat(Connection *conn)
{
    GameSrv_PingRequest(conn);
}
