#include <stdint.h>

#include "config.h"


struct Book {
    uint32_t ask;
    uint32_t bid;
    float quantities[PRICE_MAX_STEP];
};

struct Book *book_init(int num_of_book);
void book_update(struct Book *books, int book_index, char side, uint32_t price, float quantity);

float book_get_bid_price(struct Book *books, int book_index);
float book_get_ask_price(struct Book *books, int book_index);
float book_get_bid_quant(struct Book *books, int book_index);
float book_get_ask_quant(struct Book *books, int book_index);