#ifdef CORE_ITEM_H
#error "item.h included more than once"
#endif
#define CORE_ITEM_H

typedef struct Item {
    GameObject  object;
    int32_t     item_id;
    AgentId     agent_id;

    uint32_t    flags;
    uint32_t    value;
    uint32_t    quantity;
    uint32_t    model_id;

    Bag        *bag;
    ItemType    type;
    uint32_t    slot;
    uint32_t    quote_price;
} Item;

static void api_make_item(ApiItem *dest, Item *src)
{
    dest->item_id   = src->item_id;
    dest->model_id  = src->model_id;
    dest->quantity  = src->quantity;
    dest->type      = src->type;
    dest->value     = src->value;
}

void remove_item_from_bag(Item *item);
Item *get_item_safe(GwClient *client, int32_t id);
Bag *get_bag_safe(GwClient *client, BagEnum bag_id);
