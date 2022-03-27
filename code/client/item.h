#ifdef CORE_ITEM_H
#error "item.h included more than once"
#endif
#define CORE_ITEM_H

typedef uint32_t ItemModifier;
typedef array(ItemModifier) ArrayItemModifier;

uint32_t item_mod_identifier(ItemModifier mod);
uint32_t item_mod_arg1(ItemModifier mod);
uint32_t item_mod_arg2(ItemModifier mod);

typedef struct Item {
    GameObject  object;
    uint32_t    item_id;
    AgentId     agent_id;

    uint32_t    flags;
    uint32_t    value;
    uint32_t    quantity;
    uint32_t    model_id;

    struct kstr name;
    uint16_t    name_buffer[8];
    uint32_t    mod_struct[12];

    Bag        *bag;
    ItemType    type;
    uint32_t    slot;
    uint32_t    quote_price;

    ArrayItemModifier mods;
} Item;

static void api_make_item(ApiItem *dest, Item *src)
{
    dest->item_id   = src->item_id;
    dest->model_id  = src->model_id;
    dest->quantity  = src->quantity;
    dest->type      = src->type;

    array_init(&dest->mods);
    array_resize(&dest->mods, array_size(&src->mods));

    ItemModifier* mod;
    array_foreach(mod, &src->mods) {
        ApiItemModifier apiMod;
        apiMod.identifier = item_mod_identifier(*mod);
        apiMod.arg1 = item_mod_arg1(*mod);
        apiMod.arg2 = item_mod_arg2(*mod);

        array_add(&dest->mods, apiMod);
    }
}


void remove_item_from_bag(Item *item);
Item *get_item_safe(GwClient *client, int32_t id);
Bag *get_bag_safe(GwClient *client, BagEnum bag_id);
