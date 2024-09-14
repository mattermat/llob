#include "fastnsq.h"
#include <string.h>
#include "config.h"

void fastnsq_init(struct Fast_nsq *book)
{
    memset(book, 0, sizeof(struct Fast_nsq));
    book->ask = book->asks + MAX_LEVEL;
    book->bid = book->bids;
}

static inline void search_level_up(struct Fast_nsq *book)
{
    while (1)
    {
        book->ask += STEP_SIZE;
        if (*(book->ask) > 0.0) return;
    }
}

static inline void search_level_down(struct Fast_nsq *book)
{
    while (1)
    {
        book->bid -= STEP_SIZE;
        if (*(book->bid) > 0.0) return;
    }
}

void fastnsq_update(struct Fast_nsq *book, char side, f64 price, f64 quantity)
{
    u64 price_level = (u64) (price * 100);

    if (side == 'a')
    {
        size_t ask_level = book->ask - book->asks;
        book->asks[price_level] = quantity;
        if (quantity != 0.0)
        {
            if (price_level < ask_level) book->ask = &(book->asks[price_level]);
        } else {
            if (price_level == ask_level) search_level_up(book);
        }
    } else {
        size_t bid_level = book->bid - book->bids;
        book->bids[price_level] = quantity;
        if (quantity != 0.0)
        {
            if (price_level > bid_level) book->bid = &(book->bids[price_level]);
        } else {
            if (price_level == bid_level) search_level_down(book);
        }
    }
}

f64 fastnsq_get_ask_price(struct Fast_nsq *book)
{
    f64 result = (f64) (book->ask - book->asks) * STEP_SIZE / 100;
    return result;
}

f64 fastnsq_get_ask_quantity(struct Fast_nsq *book)
{
    return *(book->ask);
}

f64 fastnsq_get_bid_price(struct Fast_nsq *book)
{
    f64 result = (f64) (book->bid - book->bids) * STEP_SIZE / 100;
    return result;
}

f64 fastnsq_get_bid_quantity(struct Fast_nsq *book)
{
    return *(book->bid);
}

f64 fastnsq_get_quantity_at_price(struct Fast_nsq *book, f64 price)
{
    /* NOTE: this version doesn't work on nsq */
    u64 price_level = (u64) (price * 100);
    f64 ask_q = book->asks[price_level];
    if (ask_q > 0.0)
    {
        return ask_q;
    } else {
        return book->bids[price_level];
    }
}

f64 fastnsq_get_quantity_at_price_(struct Fast_nsq *book, char side, f64 price)
{
    u64 price_level = (u64) (price * 100);
    if (side == 'a')
    {
        return book->asks[price_level];
    } else {
        return book->bids[price_level];
    }
}

char fastnsq_get_side_at_price(struct Fast_nsq *book, double price)
{
    u64 price_level = (u64) (price * 100);
    if (book->asks[price_level] > 0.0) return 'a';
    if (book->bids[price_level] > 0.0) return 'b';
    return '0';
}
