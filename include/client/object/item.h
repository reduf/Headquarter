typedef enum ItemType {
    ItemType_Salvage       = 0,
    ItemType_Leadhand      = 1,
    ItemType_Axe           = 2,
    ItemType_Bag           = 3,
    ItemType_Feet          = 4,
    ItemType_Bow           = 5,
    ItemType_Bundle        = 6,
    ItemType_Chest         = 7,
    ItemType_Rune          = 8,
    ItemType_Consumable    = 9,
    ItemType_Dye           = 10,
    ItemType_Material      = 11,
    ItemType_Focus         = 12,
    ItemType_Arms          = 13,
    ItemType_Sigil         = 14,
    ItemType_Hammer        = 15,
    ItemType_Head          = 16,
    ItemType_SalvageItem   = 17,
    ItemType_Key           = 18,
    ItemType_Legs          = 19,
    ItemType_Coins         = 20,
    ItemType_QuestItem     = 21,
    ItemType_Wand          = 22,
    ItemType_Shield        = 24,
    ItemType_Staff         = 26,
    ItemType_Sword         = 27,
    ItemType_Kit           = 29,
    ItemType_Trophy        = 30,
    ItemType_Scroll        = 31,
    ItemType_Daggers       = 32,
    ItemType_Present       = 33,
    ItemType_Minipet       = 34,
    ItemType_Scythe        = 35,
    ItemType_Spear         = 36,
    ItemType_Handbook      = 43,
    ItemType_CostumeBody   = 44,
    ItemType_CostumeHead   = 45,
} ItemType;

typedef struct ApiItemModifier {
    uint32_t identifier;
    uint32_t arg1;
    uint32_t arg2;
} ApiItemModifier;
typedef array(ApiItemModifier) ArrayApiItemModifier;

typedef struct ApiItem {
    uint32_t             item_id;
    uint32_t             model_id;
    uint32_t             quantity;
    ItemType             type;
    ArrayApiItemModifier mods;
} ApiItem;
typedef array(ApiItem) ArrayApiItem;
