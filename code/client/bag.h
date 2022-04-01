#ifdef CORE_BAG_H
#error "bag.h included more than once"
#endif
#define CORE_BAG_H

typedef array(struct Item *) ArrayItem;

typedef struct Bag {
    uint16_t bag_id;
    
    int item_count;
    BagEnum model;
    BagType type;
    ArrayItem items;
} Bag;
typedef array(Bag) BagArray;
