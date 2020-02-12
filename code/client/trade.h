#ifdef CORE_TRADE_H
#error "trade.h included more than once"
#endif
#define CORE_TRADE_H

typedef struct TradeSession {
    uint32_t   trader;
    TradeState state;

    bool request_pending;

    int trader_gold;
    int player_gold;

    TradeItemArray trader_items;
    TradeItemArray player_items;
} TradeSession;
