typedef enum BagEnum {
    BagEnum_Backpack            = 0,
    BagEnum_BeltPouch           = 1,
    BagEnum_Bag1                = 2,
    BagEnum_Bag2                = 3,
    BagEnum_EquipmentPack       = 4,
    BagEnum_MaterialStorage     = 5,
    BagEnum_UnclaimedItems      = 6,
    BagEnum_Storage1            = 7,
    BagEnum_Storage2            = 8,
    BagEnum_Storage3            = 9,
    BagEnum_Storage4            = 10,
    BagEnum_Storage5            = 11,
    BagEnum_Storage6            = 12,
    BagEnum_Storage7            = 13,
    BagEnum_Storage8            = 14,
    BagEnum_Storage9            = 15,
    BagEnum_Storage10           = 16,
    BagEnum_Storage11           = 17,
    BagEnum_Storage12           = 18,
    BagEnum_Storage13           = 19,
    BagEnum_StorageAnniversary  = 20,
    BagEnum_EquippedItems       = 21,

    BagEnum_Count,
    BagEnum_Invalid             = -1,
} BagEnum;

typedef enum EquipedItemSlot {
    EquipedItemSlot_Weapon      = 0,
    EquipedItemSlot_OffHand     = 1,
    EquipedItemSlot_Chest       = 2,
    EquipedItemSlot_Leggings    = 3,
    EquipedItemSlot_Helm        = 4,
    EquipedItemSlot_Boots       = 5,
    EquipedItemSlot_Gloves      = 6,
    EquipedItemSlot_Costume     = 7,
    EquipedItemSlot_CostumeHead = 8,
} EquipedItemSlot;

typedef enum BagType {
    BagType_Bag          = 1,
    BagType_Equiped      = 2,
    BagType_NotCollected = 3,
    BagType_Storage      = 4,
    BagType_MatsStorage  = 5,
} BagType;

typedef struct BagLocation {
    BagEnum bag;
    int     slot;
} BagLocation;
