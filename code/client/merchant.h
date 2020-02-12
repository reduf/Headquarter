#ifdef CORE_MERCHANT_H
#error "merchant.h included more than once"
#endif
#define CORE_MERCHANT_H

typedef enum TransactionType {
    TRANSACT_TYPE_MerchantBuy = 1,
    TRANSACT_TYPE_CollectorBuy,
    TRANSACT_TYPE_CrafterBuy,
    TRANSACT_TYPE_WeaponsmithCustomize,
    TRANSACT_TYPE_MerchantSell = 11,
    TRANSACT_TYPE_TraderBuy,
    TRANSACT_TYPE_TraderSell,
    TRANSACT_TYPE_UnlockRunePriestOfBalth = 15
} TransactionType;

typedef struct TransactionInfo {
    int item_count;
    int item_ids[16];
    int item_quants[16];
} TransactionInfo;

typedef struct QuoteInfo {
    int unk1;
    int item_count;
    int item_ids[16];
} QuoteInfo;
