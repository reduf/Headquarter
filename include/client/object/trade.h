typedef enum TradeState {
    TradeState_Closed   = (1 << 0),
    TradeState_Opened   = (1 << 1),
    TradeState_Received = (1 << 2),
    TradeState_Accepted = (1 << 3),
} TradeState;

typedef struct TradeItem {
    int32_t item_id;
    int quantity;
} TradeItem;
typedef array(TradeItem) TradeItemArray;
