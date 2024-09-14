#pragma once

#include "defines.h"
#include "config.h"

struct Fast_nsq {
    /* INVESTIGATE THIS: two consecutive arrays (like now) or array of (bidq, askq)? */
    f64 bids[MAX_LEVEL];
    f64 asks[MAX_LEVEL];
    f64 *bid;
    f64 *ask;
};

void fastnsq_init(struct Fast_nsq *book);
void fastnsq_update(struct Fast_nsq *book, char side, f64 price, f64 quantity);
f64 fastnsq_get_ask_price(struct Fast_nsq *book);
f64 fastnsq_get_ask_quantity(struct Fast_nsq *book);
f64 fastnsq_get_bid_price(struct Fast_nsq *book);
f64 fastnsq_get_bid_quantity(struct Fast_nsq *book);
f64 fastnsq_get_quantity_at_price(struct Fast_nsq *book, f64 price);
char fastnsq_get_side_at_price(struct Fast_nsq *book, f64 price);
