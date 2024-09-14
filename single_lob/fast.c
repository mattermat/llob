#include "fast.h"
#include <string.h>
#include "config.h"

void fast_init(struct Fast *book)
{
    memset(book, 0, sizeof(struct Fast));
    book->ask = book->quantities + MAX_LEVEL; // highest price level
    book->bid = book->quantities;             // lowest price level
}


static inline void search_level_up(struct Fast *book)
{
    // TODO: check for overflow
    while (1)
    {
        book->ask += STEP_SIZE;
        if (*(book->ask) > 0.0) return;
    }
}

static inline void search_level_down(struct Fast *book)
{
    // TODO: check for overflow
    while (1)
    {
        book->bid -= STEP_SIZE;
        if (*(book->bid) > 0.0) return;
    }
}

void fast_update(struct Fast *book, char side, f64 price, f64 quantity)
{
    u64 price_level = (u64) (price * 100);
    /* We don't need to know if the update is on ask or on bid side to update the book */
    book->quantities[price_level] = quantity;

    // TODO: size_t or u64 or u32?
    size_t ask_level = book->ask - book->quantities;
    size_t bid_level = book->bid - book->quantities;
    size_t is_ask = (price_level == ask_level && side == 'a');
    size_t is_bid = (price_level == bid_level && side == 'b');

    /* We need to know if it is an ask or bid update just to update the best levels */
    if (quantity != 0.0 )
    {
        /* If price level fall within */
        if (side == 'a' && price_level < ask_level) book->ask = &(book->quantities[price_level]);
        if (side == 'b' && price_level > bid_level) book->bid = &(book->quantities[price_level]);
    } else { // quantity == 0.0 => level deleted
        if (is_ask) search_level_up(book);
        if (is_bid) search_level_down(book);
    }
}

f64 fast_get_ask_price(struct Fast *book)
{
    f64 result = (f64) (book->ask - book->quantities) * STEP_SIZE / 100;
    return result;
}

f64 fast_get_ask_quantity(struct Fast *book)
{
    return *(book->ask);
}

f64 fast_get_bid_price(struct Fast *book)
{
    f64 result = (f64) (book->bid - book->quantities) * STEP_SIZE / 100;
    return result;
}

f64 fast_get_bid_quantity(struct Fast *book)
{
    return *(book->bid);
}

f64 fast_get_quantity_at_price(struct Fast *book, f64 price)
{
    u64 price_level = (u64) (price * 100);
    return book->quantities[price_level];
}

char fast_get_side_at_price(struct Fast *book, double price)
{
    u64 price_level = (u64) (price * 100);
    // TODO: size_t or u64 or u32?
    size_t ask_level = book->ask - book->quantities;
    size_t bid_level = book->bid - book->quantities;
    if (book->quantities[price_level] == 0.0) return '0';
    if (price_level >= ask_level) return 'a';
    if (price_level <= bid_level) return 'b';
    return '0';
}
