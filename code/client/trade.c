#ifdef CORE_TRADE_C
#error "trade.c included more than once"
#endif
#define CORE_TRADE_C

void HandleTradeRequest(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t player_id;
    } TradeRequest;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRADE_REQUEST);
    assert(sizeof(TradeRequest) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeRequest *pack = cast(TradeRequest *)packet;
    assert(client && client->game_srv.secured);

    // Player *player = GetPlayer(pack->player);
    // LogInfo("%S request a trade.", player->name);

    GameSrv_TradeAcknowledge(client, pack->player_id);
}

void HandleTradeTerminate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t unk0; // u8
    } TradeTerminate;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRADE_TERMINATE);
    assert(sizeof(TradeTerminate) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeTerminate *pack = cast(TradeTerminate *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;

    client->trade_session.state = TradeState_Closed;
    // LogInfo("Trade terminated");
}

void HandleTradeOfferedCount(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t unk0;
        int32_t quantity;
    } OfferedCount;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRADE_OFFERED_COUNT);
    assert(sizeof(OfferedCount) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    OfferedCount *pack = cast(OfferedCount *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;
}

void HandleTradeReceiveOffer(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t gold;
    } TradeOffer;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRADE_RECEIVE_OFFER);
    assert(sizeof(TradeOffer) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeOffer *pack = cast(TradeOffer *)packet;
    assert(client && client->game_srv.secured);

    TradeSession *session = &client->trade_session;
    session->state |= TradeState_Received;
    session->trader_gold = pack->gold;
    // LogInfo("Receive offer");
}

void HandleTradeAddItem(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t item_id;
        uint8_t quantity;
    } TradeItemInfo;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_TRADE_ADD_ITEM);
    assert(sizeof(TradeItemInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeItemInfo*pack = cast(TradeItemInfo*)packet;
    assert(client && client->game_srv.secured);

    TradeSession *session = &client->trade_session;
    assert(array_size(&session->trader_items) <= 6);

    TradeItem trade_item;
    trade_item.item_id = pack->item_id;
    trade_item.quantity = pack->quantity;
    array_add(&session->trader_items, trade_item);
}

void HandleTradeAcknowledge(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t player_id;
    } TradeAcknowledge;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_TRADE_ACKNOWLEDGE);
    assert(sizeof(TradeAcknowledge) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeAcknowledge *pack = cast(TradeAcknowledge *)packet;
    assert(client && client->game_srv.secured);

    TradeSession *session = &client->trade_session;
    assert(session->request_pending);

    // We cannot enqueue more than 1 request, so it is fine to assume there is no request pending.
    session->request_pending = false;
    session->trader = pack->player_id;
    session->state |= TradeState_Opened;

    array_clear(&session->trader_items);
    array_clear(&session->player_items);
}

void HandleTradeAccept(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_TRADE_ACCEPT);
    assert(psize == sizeof(Packet));

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    TradeSession *session = &client->trade_session;
    session->state |= TradeState_Accepted;
}

void HandleTradeChangeOffer(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t unk0; // u8
        int32_t unk1; // u32
    } TradeChangeOffer;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_TRADE_CHANGE_OFFER);
    assert(sizeof(TradeChangeOffer) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    TradeChangeOffer *pack = cast(TradeChangeOffer *)packet;
    assert(client && client->game_srv.secured);
    (void)pack;

    TradeSession *session = &client->trade_session;
    session->state |= ~TradeState_Opened;
    // LogInfo("Change offer");
}

void GameSrv_TradeAcknowledge(GwClient *client, int32_t player_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t player_id;
    } TradeAcknowledge;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    TradeAcknowledge packet = NewPacket(GAME_CMSG_TRADE_ACKNOWLEDGE);
    packet.player_id = player_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);

    TradeSession *session = &client->trade_session;
    session->trader = player_id;
    session->state |= TradeState_Opened;

    array_clear(&session->trader_items);
    array_clear(&session->player_items);
}

void GameSrv_TradeCancel(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_TRADE_CANCEL);
    SendPacket(&client->game_srv, sizeof(packet), &packet);

    TradeSession *session = &client->trade_session;
    session->state = TradeState_Closed;
}

void GameSrv_TradeAddItem(GwClient *client, uint32_t item_id, uint8_t quantity)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t item_id;
        uint8_t quantity;
    } AddItem;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    assert(quantity <= 250);

    AddItem packet = NewPacket(GAME_CMSG_TRADE_ADD_ITEM);
    packet.item_id = item_id;
    packet.quantity = quantity;

    SendPacket(&client->game_srv, sizeof(packet), &packet);

    TradeSession *session = &client->trade_session;
    TradeItem trade_item;
    trade_item.item_id = item_id;
    trade_item.quantity = quantity;
    array_add(&session->player_items, trade_item);
}

void GameSrv_TradeSendOffer(GwClient *client, int gold)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t gold;
    } TradeOffer;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    TradeOffer packet = NewPacket(GAME_CMSG_TRADE_SEND_OFFER);
    packet.gold = gold;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_TradeRemoveItem(GwClient *client, int32_t item_id, int quantity)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t item_id;
        uint8_t quantity;
    } RemoveItem;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    RemoveItem packet = NewPacket(GAME_CMSG_TRADE_REMOVE_ITEM);
    packet.item_id = item_id;
    packet.quantity = quantity % 250;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_TradeCancelOffer(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_TRADE_CANCEL_OFFER);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_TradeAccept(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_TRADE_ACCEPT);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_TradeInitiate(GwClient *client, AgentId partner)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
    } TradeTarget;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    TradeTarget packet = NewPacket(GAME_CMSG_TRADE_INITIATE);
    packet.agent_id = partner;

    SendPacket(&client->game_srv, sizeof(packet), &packet);

    TradeSession *session = &client->trade_session;
    session->request_pending = true;
}
