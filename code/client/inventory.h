#ifdef CORE_INVENTORY_H
#error "inventory.h included more than once"
#endif
#define CORE_INVENTORY_H

typedef struct Inventory {
    union {
        Bag *bags[BagEnum_Count];
        struct {
            Bag *backpack;
            Bag *beltpouch;
            Bag *bag1;
            Bag *bag2;
            Bag *equipment_pack;
            Bag *material_storage;
            Bag *unclaimed_items;
            Bag *storage1;
            Bag *storage2;
            Bag *storage3;
            Bag *storage4;
            Bag *storage5;
            Bag *storage6;
            Bag *storage7;
            Bag *storage8;
            Bag *storage9;
            Bag *equipped_items;
        };
    };

    int gold_character;
    int gold_storage;
} Inventory;
