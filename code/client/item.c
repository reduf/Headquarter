#ifdef CORE_ITEM_C
#error "item.c included more than once"
#endif
#define CORE_ITEM_C

uint32_t item_mod_identifier(uint32_t mod)
{
    return mod >> 16;
}

uint32_t item_mod_arg1(uint32_t mod)
{
    return (mod & 0x0000FF00) >> 8;
}

uint32_t item_mod_arg2(uint32_t mod)
{
    return (mod & 0x000000FF);
}

void remove_item_from_bag(Item *item)
{
    if (!item->bag) return;

    Bag *bag = item->bag;
    assert(array_inside(&bag->items, item->slot));
    assert(array_at(&bag->items, item->slot));

    array_set(&bag->items, item->slot, NULL);
    item->bag = NULL;
    item->slot = 0;
    bag->item_count--;
}

Item *get_item_safe(GwClient *client, int32_t id)
{
    if (!(client->ingame && client->world.hash))
        return NULL;
    ArrayItem items = client->world.items;
    if (!array_inside(&items, id))
        return NULL;
    return array_at(&items, id);
}

Bag *get_bag_safe(GwClient *client, BagEnum bag_id)
{
    if (!client->ingame)
        return NULL;
    return client->inventory.bags[bag_id];
}

void HandleItemStreamCreate(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header   header;
        uint16_t stream_id;
        uint8_t  is_hero;
    } StreamCreate;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_STREAM_CREATE);
    assert(sizeof(StreamCreate) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleItemStreamDestroy(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream_id;
    } StreamDestroy;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_STREAM_DESTROY);
    assert(sizeof(StreamDestroy) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void HandleItemGeneralInfo(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        /* +h0000 */ Header header;
        /* +h0002 */ int32_t item_id;
        /* +h0006 */ int32_t file_id;
        /* +h000A */ int8_t type;
        /* +h000B */ int8_t unk0;
        /* +h000C */ int16_t dye_color;
        /* +h000E */ int16_t materials;
        /* +h0010 */ int8_t unk1;
        /* +h0011 */ int32_t flags; // interaction
        /* +h0015 */ int32_t value;
        /* +h0019 */ int32_t model;
        /* +h001D */ int32_t quantity;
        /* +h0021 */ uint16_t name[64];
        /* +h009D */ uint32_t n_modifier;
        /* +h00A1 */ uint32_t modifier[64];
    } ItemInfo;

#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_GENERAL_INFO);
    assert(sizeof(ItemInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ItemInfo *pack = cast(ItemInfo *)packet;
    assert(client && client->game_srv.secured);
    // LogInfo("ItemGeneralInfo {id: %d, modele: %d}", pack->item_id, pack->model);

    ArrayItem *items = &client->world.items;
    if (!array_inside(items, pack->item_id)) {
        array_resize(items, pack->item_id + 1);
        items->size = items->capacity;
    }

    Item *new_item = malloc(sizeof(*new_item));
    memset(new_item, 0, sizeof(Item));
    new_item->item_id = pack->item_id;
    new_item->flags = pack->flags;
    new_item->value = pack->value;
    new_item->model_id = pack->model;
    new_item->quantity = pack->quantity;
    new_item->type = pack->type;
    new_item->value = pack->value;
    kstr_read(&new_item->name, pack->name, ARRAY_SIZE(pack->name));

    // @Cleanup: Save modifier

    array_set(items, pack->item_id, new_item);
}

void HandleItemRemove(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream_id;
        int32_t item_id;
    } ItemRemove;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_REMOVE);
    assert(sizeof(ItemRemove) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ItemRemove *pack = cast(ItemRemove *)packet;
    assert(client && client->game_srv.secured);

    ArrayItem *items = &client->world.items;
    Item *item = get_item_safe(client, pack->item_id);
    if (!item) return;

    array_set(items, pack->item_id, NULL);
    remove_item_from_bag(item);
    free(item);
}

void HandleItemWeaponSet(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream_id;
        int8_t slot;
        int32_t leadhand;
        int32_t offhand;
    } WeaponSet;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_WEAPON_SET);
    assert(sizeof(WeaponSet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    // LogInfo("New Weapon set !");
}

void HandleInventoryItemQuantity(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t item_id;
        int32_t quantity;
    } ItemQuantity;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INVENTORY_ITEM_QUANTITY);
    assert(sizeof(ItemQuantity) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ItemQuantity *pack = cast(ItemQuantity *)packet;
    assert(client && client->game_srv.secured);

    Item *item = get_item_safe(client, pack->item_id);
    if (!item) {
        LogInfo("Updated item quantity of item %d, but the item doesn't exist.", pack->item_id);
        return;
    }

    item->quantity = pack->quantity;
}

void HandleInventoryItemLocation(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream_id;
        int32_t item_id;
        int16_t bag_id;
        int8_t slot;
    } ItemLocation;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_INVENTORY_ITEM_LOCATION);
    assert(sizeof(ItemLocation) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ItemLocation *pack = cast(ItemLocation *)packet;
    assert(client && client->game_srv.secured);

#if 0
    Item *item = get_item_safe(client, pack->item_id);
    if (!item) return;
#else
    if (!client->world.hash)
        return;
    ArrayItem items = client->world.items;
    if (!(array_inside(&items, pack->item_id) && array_at(&items, pack->item_id)))
        return;
    Item *item = array_at(&items, pack->item_id);
#endif

    BagArray *bags = &client->world.bags;
    uint32_t hash = hash_int32(pack->bag_id);
    uint32_t index = hash % bags->size;
    Bag *bag = &array_at(bags, index);

    while (bag->bag_id != pack->bag_id) {
        assert(bag->bag_id != 0);
        hash = hash_int32(hash);
        index = hash % bags->size;
        bag = &array_at(bags, index);
    }

    assert(bag->bag_id == pack->bag_id);
    assert(array_inside(&bag->items, pack->slot));
#if 0 // we sometimes receive this
    assert(array_at(bag->items, pack->slot) == NULL);
#endif

    item->bag = bag;
    item->slot = pack->slot;
    array_set(&bag->items, pack->slot, item);
    bag->item_count++;
}

void HandleInventoryCreateBag(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int16_t stream_id;
        int8_t bag_type;
        int8_t bag_model_id;
        int16_t bag_id;
        int8_t slot_count;
        int32_t assoc_item_id;
    } BagInfo;
#pragma pack(pop)
    
    assert(packet->header == GAME_SMSG_INVENTORY_CREATE_BAG);
    assert(sizeof(BagInfo) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    BagInfo *pack = cast(BagInfo *)packet;

    BagArray *bags = &client->world.bags;
    uint32_t hash = hash_int32(pack->bag_id);
    uint32_t index = hash % bags->size;
    Bag *bag = &array_at(bags, index);

    while (bag->bag_id != 0) {
        hash = hash_int32(hash);
        index = hash % bags->size;
        bag = &array_at(bags, index);
    }

    assert(bag->bag_id == 0);
    bag->bag_id = pack->bag_id;
    bag->type = (BagType)pack->bag_type;
    bag->model = (BagEnum)pack->bag_model_id;
    array_init(&bag->items);
    array_resize(&bag->items, pack->slot_count);

    assert(pack->bag_model_id < BagEnum_Count);
    client->inventory.bags[pack->bag_model_id] = bag;
}
void HandleWindowItemStreamEnd(Connection* conn, size_t psize, Packet* packet) {
    #pragma pack(push, 1)
        typedef struct {
            Header header;
            uint8_t type;
        } ItemStreamEnd;
    #pragma pack(pop)

    assert(packet->header == GAME_SMSG_WINDOW_ITEM_STREAM_END);
    assert(sizeof(ItemStreamEnd) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    assert(client && client->game_srv.secured);

    HandleMerchantReady(client);
}
void HandleWindowMerchant(Connection* conn, size_t psize, Packet* packet) {
    #pragma pack(push, 1)
        typedef struct {
            Header header;
            uint8_t type;
            uint32_t unk;
        } WindowMerchant;
    #pragma pack(pop)

    assert(packet->header == GAME_SMSG_WINDOW_MERCHANT);
    assert(sizeof(WindowMerchant) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    WindowMerchant* pack = cast(WindowMerchant*)packet;
    assert(client&& client->game_srv.secured);

    if (pack->type == 11 && !pack->unk) {
        // Special case for merchant; only receives list of buyable items (i.e. no WindowPricesEnd packet; GW client figures out the sell tab)
        HandleMerchantReady(client);
    }
}
void HandleWindowOwner(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        AgentId agent_id;
    } WindowOwner;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_WINDOW_OWNER);
    assert(sizeof(WindowOwner) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    WindowOwner *pack = cast(WindowOwner *)packet;
    assert(client && client->game_srv.secured);

    array_clear(&client->merchant_items);
    array_clear(&client->tmp_merchant_items);
    array_clear(&client->tmp_merchant_prices);
    client->merchant_agent_id = pack->agent_id;
    client->interact_with = pack->agent_id;
    Event_DialogOpenned event;
    event.sender_agent_id = pack->agent_id;
    broadcast_event(&client->event_mgr, DIALOG_OPENNED, &event);
}
void HandleMerchantReady(GwClient* client) {
    Item* item;
    LogInfo("HandleMerchantReady called for %d items", client->tmp_merchant_items.size);
    thread_mutex_lock(&client->mutex);
    for (size_t i = 0; i < client->tmp_merchant_items.size; i++) {
        array_add(&client->merchant_items, client->tmp_merchant_items.data[i]);
        if (array_inside(&client->tmp_merchant_prices, i)) {
            item = client->tmp_merchant_items.data[i];
            // GW Bug: last sale price can be LOWER than the default item value!
            if(item->value < client->tmp_merchant_prices.data[i])
                item->value = client->tmp_merchant_prices.data[i];
        }
    }
    thread_mutex_unlock(&client->mutex);
    Event_DialogOpenned event;
    event.sender_agent_id = client->merchant_agent_id;
    broadcast_event(&client->event_mgr, MERCHANT_WINDOW_OPENED, &event);
}

void HandleWindowAddItems(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t n_items;
        uint32_t items[16];
    } AddItems;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_WINDOW_ADD_ITEMS);
    assert(sizeof(AddItems) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    AddItems *pack = cast(AddItems *)packet;
    assert(client && client->game_srv.secured);

    ArrayItem *items = &client->world.items;
    ArrayItem *merchant_items = &client->tmp_merchant_items;

    for (size_t i = 0; i < pack->n_items; i++) {
        int32_t item_id = pack->items[i];
        if (!array_inside(items, item_id)) {
            LogError("Expected item '%d' but was not found", item_id);
            continue;
        }

        Item *item = array_at(items, item_id);
        if (!item) {
            LogError("Expected item '%d' but was not found", item_id);
            continue;
        }
        array_add(merchant_items, item);
    }
    printf("AddItems, %d\n", pack->n_items);
}

void HandleWindowAddPrices(Connection* conn, size_t psize, Packet* packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint32_t  n_prices;
        int32_t prices[16];
    } AddPrices;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_PRICES);
    assert(sizeof(AddPrices) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    AddPrices* pack = cast(AddPrices*)packet;
    assert(client&& client->game_srv.secured);

    array_uint32_t* merchant_prices = &client->tmp_merchant_prices;
    for (size_t i = 0; i < pack->n_prices; i++) {
        array_add(merchant_prices, pack->prices[i]);
    }
}

void HandleItemPriceQuote(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        int32_t item_id;
        int32_t price;
    } PriceQuote;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_PRICE_QUOTE);
    assert(sizeof(PriceQuote) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    PriceQuote *pack = cast(PriceQuote *)packet;
    assert(client && client->game_srv.secured);

    ArrayItem *items = &client->world.items;
    assert(array_inside(items, pack->item_id));
    assert(array_at(items, pack->item_id));
    Item *item = array_at(items, pack->item_id);

    item->quote_price = pack->price;
    Event_ItemPrice params;
    params.item_id = item->item_id;
    params.quote_price = item->quote_price;
    broadcast_event(&client->event_mgr, EventType_ItemQuotePrice, &params);
}

void HandleItemChangeLocation(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int16_t unk1;
        int32_t item_id;
        int16_t bag_id;
        int8_t  slot;
    } ChangeLocation;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_CHANGE_LOCATION);
    assert(sizeof(ChangeLocation) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    ChangeLocation *pack = cast(ChangeLocation *)packet;
    assert(client && client->game_srv.secured);

    Item *item = get_item_safe(client, pack->item_id);
    if (!item) {
        LogError("HandleItemChangeLocation: Couldn't find item %d", pack->item_id);
        return;
    }

    BagArray *bags = &client->world.bags;
    uint32_t hash = hash_int32(pack->bag_id);
    uint32_t index = hash % bags->size;
    Bag *bag = &array_at(bags, index);

    while (bag->bag_id != pack->bag_id) {
        assert(bag->bag_id != 0);
        hash = hash_int32(hash);
        index = hash % bags->size;
        bag = &array_at(bags, index);
    }

    // @Cleanup
    remove_item_from_bag(item);
    item->bag  = bag;
    item->slot = pack->slot;
    bag->items.data[pack->slot] = item;
    bag->item_count++;
}

void GameSrv_UseItem(GwClient *client, Item *item)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t item_id;
    } ItemUse;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    ItemUse packet = NewPacket(GAME_CMSG_ITEM_USE);
    packet.item_id = item->item_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_InteractItem(GwClient *client, uint32_t agent_id)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        AgentId agent_id;
        uint8_t unk0;
    } ItemUse;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    ItemUse packet = NewPacket(GAME_CMSG_INTERACT_ITEM);
    packet.agent_id = agent_id;
    packet.unk0 = 0;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_MoveItem(GwClient *client, Item *item, Bag *bag, uint8_t slot)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t item_id;
        int16_t bag_id;
        uint8_t slot;
    } ItemMove;
#pragma pack(pop)

    assert(client && client->game_srv.secured);

    ItemMove packet = NewPacket(GAME_CMSG_ITEM_MOVE);
    packet.item_id = item->item_id;
    packet.bag_id = bag->bag_id;
    packet.slot = slot;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_StartSalvage(GwClient *client, Item *kit, Item *item)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int16_t session_id;
        int32_t kit_item_id;
        int32_t item_id;
    } SalvagePacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SalvageSession *session = &client->salvage_session;

    SalvagePacket packet = NewPacket(GAME_CMSG_ITEM_SALVAGE_SESSION_OPEN);
    packet.session_id = session->salvage_session_id = 21; // @fixme only true for first player in guild hall - client will get disconnected if it's wrong
    packet.kit_item_id = kit->item_id;
    packet.item_id = item->item_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);

    LogDebug("SendPacket Salvage { session: %u, item: %u, kit: %u }", packet.session_id, packet.item_id, packet.kit_item_id);
}

void GameSrv_CancelSalvage(GwClient *client)
{
    assert(client && client->game_srv.secured);
    SalvageSession *session = &client->salvage_session;
    if (!session->is_open) return;

    Packet packet = NewPacket(GAME_CMSG_ITEM_SALVAGE_SESSION_CANCEL);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SalvageDone(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_ITEM_SALVAGE_SESSION_DONE);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SalvageMaterials(GwClient *client)
{
    assert(client && client->game_srv.secured);
    Packet packet = NewPacket(GAME_CMSG_ITEM_SALVAGE_MATERIALS);
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void GameSrv_SalvageUpgrade(GwClient *client, size_t index)
{
#pragma pack(push, 1)
    typedef struct {
        Header header;
        uint8_t upgrade_index;
    } SalvagePacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    SalvageSession *session = &client->salvage_session;
    if (index >= session->n_upgrades) {
        LogError("The maximum upgrade index is %u, but you entered %u", session->n_upgrades, index);
        return;
    }

    SalvagePacket packet = NewPacket(GAME_CMSG_ITEM_SALVAGE_UPGRADE);
    packet.upgrade_index = cast(uint8_t)index;
    SendPacket(&client->game_srv, sizeof(packet), &packet);
}

void HandleSalvageSessionStart(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint16_t salvage_session_id;
        uint32_t item_id; // representing the trade
        uint32_t n_upgrades;
        uint32_t upgrades[3];
    } SalvagePacket;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_SALVAGE_SESSION_START);
    assert(sizeof(SalvagePacket) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    SalvagePacket *pack = cast(SalvagePacket *)packet;
    assert(client && client->game_srv.secured);
    SalvageSession *session = &client->salvage_session;

    if (pack->salvage_session_id != session->salvage_session_id) {
        LogError("salvage_session_id mismatch {received: %d, expected: %d}",
            pack->salvage_session_id, session->salvage_session_id);
        return;
    }

    session->n_upgrades = pack->n_upgrades;
    for (size_t i = 0; i < session->n_upgrades; i++) {
        uint32_t item_id = pack->upgrades[i];
        session->upgrades[i] = get_item_safe(client, item_id);
    }
    session->is_open = true;

    broadcast_event(&client->event_mgr, SALVAGE_SESSION_START, &pack->item_id); // @todo pass possible salvage options
}

void HandleSalvageSessionCancel(Connection *conn, size_t psize, Packet *packet)
{
    assert(packet->header == GAME_SMSG_ITEM_SALVAGE_SESSION_CANCEL);
    assert(sizeof(Packet) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);

    SalvageSession *session = &client->salvage_session;
    session->is_open = false;
    session->n_upgrades = 0;
}

void HandleSalvageSessionDone(Connection* conn, size_t psize, Packet* packet)
{
    assert(packet->header == GAME_SMSG_ITEM_SALVAGE_SESSION_DONE);
    assert(sizeof(Packet) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    assert(client && client->game_srv.secured);
    SalvageSession* session = &client->salvage_session;

    session->is_open = false;
    session->n_upgrades = 0;
}

void HandleSalvageSessionSuccess(Connection* conn, size_t psize, Packet* packet)
{
    assert(packet->header == GAME_SMSG_ITEM_SALVAGE_SESSION_SUCCESS);
    assert(sizeof(Packet) == psize);

    GwClient* client = cast(GwClient*)conn->data;
    assert(client && client->game_srv.secured);

    GameSrv_SalvageDone(client);
}

void HandleSalvageSessionItemKept(Connection *conn, size_t psize, Packet *packet)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint8_t item_kept;
    } SalvagePacket;
#pragma pack(pop)

    assert(packet->header == GAME_SMSG_ITEM_SALVAGE_SESSION_ITEM_KEPT);
    assert(sizeof(SalvagePacket) == psize);

    GwClient *client = cast(GwClient *)conn->data;
    assert(client && client->game_srv.secured);
}

void GameSrv_UnequipItem(GwClient *client, EquipedItemSlot equip_slot, Bag *bag, uint8_t slot)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        uint8_t equip_slot;
        int16_t bag_id;
        uint8_t bag_slot;
    } UnequipItemPacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);

    UnequipItemPacket packet = NewPacket(GAME_CMSG_UNEQUIP_ITEM);
    packet.equip_slot = equip_slot;
    packet.bag_id = bag->bag_id;
    packet.bag_slot = slot;

    SendPacket(&client->game_srv, sizeof(packet), &packet);    
}

void GameSrv_Identify(GwClient* client, Item* kit, Item* item)
{
#pragma pack(push, 1)
    typedef struct {
        Header  header;
        int32_t kit_item_id;
        int32_t item_id;
    } IdentifyPacket;
#pragma pack(pop)

    assert(client && client->game_srv.secured);
    IdentifyPacket packet = NewPacket(GAME_CMSG_ITEM_IDENTIFY);
    packet.kit_item_id = kit->item_id;
    packet.item_id = item->item_id;

    SendPacket(&client->game_srv, sizeof(packet), &packet);
}
