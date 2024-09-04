#pragma once

#include "defines.h"
#include "config.h"

/*
    TODO:
        is it better to have best ask and bid as pointer
        or as offset from the beginning?
*/
struct Fast {
    f64 quantities[MAX_LEVEL];
    f64 *ask; /* pointer to the best ask level */
    f64 *bid; /* pointer to the best bid level */
};

void fast_init(struct Fast *book);
void fast_update(struct Fast *book, char side, f64 price, f64 quantity);

f64 fast_get_ask_price(struct Fast *book);
f64 fast_get_ask_quantity(struct Fast *book);
f64 fast_get_bid_price(struct Fast *book);
f64 fast_get_bid_quantity(struct Fast *book);
f64 fast_get_quantity_at_price(struct Fast *book, f64 price);
char fast_get_side_at_price(struct Fast *book, f64 price);
