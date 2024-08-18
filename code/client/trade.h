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

void free_trade_session(TradeSession *session)
{
    array_reset(&session->trader_items);
    array_reset(&session->player_items);
}
