#ifdef CORE_ITEM_H
#error "item.h included more than once"
#endif
#define CORE_ITEM_H

typedef struct Item {
    uint32_t    item_id;
    AgentId     agent_id;

    uint32_t    flags;
    uint32_t    value;
    uint32_t    quantity;
    uint32_t    model_id;

    struct kstr_hdr name;
    uint16_t        name_buffer[8];
    array_uint32_t  mod_struct;

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
}

void free_item(Item* src);

void remove_item_from_bag(Item *item);
Item *get_item_safe(GwClient *client, int32_t id);
Bag *get_bag_safe(GwClient *client, BagEnum bag_id);
