#pragma once

#include "defines.h"
#include "config.h"

struct Plain {
    f64 asks[MAX_LEVEL];
    f64 bids[MAX_LEVEL];
    u64 ask; /* level of the best ask */
    u64 bid; /* level of the best bid */
};

void plain_init(struct Plain *book);
void plain_update(struct Plain *book, char side, f64 price, f64 quantity);

f64 plain_get_ask_price(struct Plain *book);
f64 plain_get_ask_quantity(struct Plain *book);
f64 plain_get_bid_price(struct Plain *book);
f64 plain_get_bid_quantity(struct Plain *book);
f64 plain_get_quantity_at_price(struct Plain *book, char side, f64 price);
