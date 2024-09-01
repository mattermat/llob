#include <string.h>
#include "plain.h"

// for debugging
#include <stdio.h>

void plain_init(struct Plain *book)
{
    memset(book, 0, sizeof(struct Plain));
    book->ask = MAX_LEVEL;
}

static inline u64 search_level_up(f64 levels[], u64 starting_level)
{
    /* scan the book upward to find the next populated level */
    /* used to find the next ask price starting from the input pointer */
    u64 index = starting_level;
    while (1)
    {
        index += BOOK_INCREMENT;
        if (levels[index] > 0.0) return index;
    }
}

static inline u64 search_level_down(f64 levels[], u64 starting_level)
{
    /* scan the book downward to find the next populated level */
    /* used to find the next bid price starting from the input pointer */
    u64 index = starting_level;
    while (1)
    {
        index -= BOOK_INCREMENT;
        if (levels[index] > 0.0) return index;
    }
}

void plain_update(struct Plain *book, char side, f64 price, f64 quantity)
{
    u64 price_level = (u64) (price * 100);
    if (side == 'a')          /* case 1: side is ask */
    {
        if (quantity == 0.0)  /* case 1.1: quantity = 0 */
        {
            /* step 1: put quantity to 0 in the related price level */
            book->asks[price_level] = 0.0; // TODO: check
            /* step 2: if the related price level is the best ask, update the new best ask */
            if (price_level == book->ask) book->ask = search_level_up(book->asks, book->ask);
        } else                /* case 1.2: quantity != 0 */
        {
            /* step 1: update quantity in the related price level */
            book->asks[price_level] = quantity; // TODO: check
            /* step 2: if the related price level is below the best ask, update the new best ask */
            if (book->ask > price_level) book->ask = price_level; // TODO: check
        }

    } else {                  /* case 2: side is bid */
        if (quantity == 0.0)  /* case 2.1: quantity = 0 */
        {
            /* step 1: put quantity to 0 in the related price level */
            book->bids[price_level] = 0.0; // TODO: check
            /* step 2: if the related price level is the best bid, update the new best bid */
            if (price_level == book->bid) book->bid = search_level_down(book->bids, book->bid);
        } else               /* case 2.2: quantity != 0 */
        {
            /* step 1: update quantity in the related price level */
            book->bids[price_level] = quantity; // TODO: check
            /* step 2: if the related price level is below the best bid, update the new best bid */
            if (book->bid < price_level) book->bid = price_level; // TODO: check
        }
    }
}

f64 plain_get_ask_price(struct Plain *book)
{
    f64 result = (f64) book->ask * STEP_SIZE / 100;
    return result;
}

f64 plain_get_ask_quantity(struct Plain *book)
{
    f64 result;
    u64 price_level = book->ask;
    result = book->asks[price_level];
    return result;
}

f64 plain_get_bid_price(struct Plain *book)
{
    f64 result = (f64) book->bid * STEP_SIZE / 100;
    return result;
}

f64 plain_get_bid_quantity(struct Plain *book)
{
    f64 result;
    u64 price_level = book->bid;
    result = book->bids[price_level];
    return result;
}

f64 plain_get_quantity_at_price(struct Plain *book, char side, f64 price)
{
    f64 quantity;
    if (side == 'a')
    {
        quantity = book->asks[(int)(price * 100 / STEP_SIZE)];
    } else {
        quantity = book->bids[(int)(price * 100 / STEP_SIZE)];
    }
    return quantity;
}
