#include <stdio.h>
#include <string.h>

#if T1
#include "t1.h"
#endif

#if T3
#inlcude "t3.h"
#endif

/* search in the symbol map */
static inline int search(char map[5][10], char symbol[10])
{
    /* if we separe calls from puts we can prefetch */
    for (int i = 0 ; i < 5; i++)
    {
        if (strncmp(symbol, map[i], 10) == 0)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // initalize 5 books
    struct Book *books = book_init(5);
    char symbol_map[5][10] = {
        "65000-C",
        "65000-P",
        "67500-C",
        "67500-P",
        "70000-C"
    };

    char symb1[10] = "65000-C";
    char symb2[10] = "65000-P";
    char symb3[10] = "65400-P";

#if T1
    /* NOTE: price step is 5 for bybit options */

    book_update(books, search(symbol_map, symb1), 'b', 100.0,  5.0);
    book_update(books, search(symbol_map, symb1), 'b', 105.0, 50.0);
    book_update(books, search(symbol_map, symb1), 'b', 100.0, 50.0);
    book_update(books, search(symbol_map, symb1), 'b', 100.0,  0.0);
    book_update(books, search(symbol_map, symb1), 'b',  95.0, 50.0);
    book_update(books, search(symbol_map, symb1), 'b', 105.0,  0.0);
    float bid_price = book_get_bid_price(books, search(symbol_map, symb1)); // should be 95.0
    float bid_quant = book_get_bid_quant(books, search(symbol_map, symb1)); // should be 50.0
    printf("best bid(q@p): %f@%f\n", bid_quant, bid_price);

    book_update(books, search(symbol_map, symb1), 'a', 105.0, 50.0);
    book_update(books, search(symbol_map, symb1), 'a', 100.0,  5.0);
    book_update(books, search(symbol_map, symb1), 'a', 100.0, 50.0);
    book_update(books, search(symbol_map, symb1), 'a', 100.0,  0.0);

    float ask_price = book_get_ask_price(books, search(symbol_map, symb1)); // should be 105.0
    float ask_quant = book_get_ask_quant(books, search(symbol_map, symb1)); // should be 50.0
    printf("best ask(q@p): %f@%f\n", ask_quant, ask_price);

    /*
    book_update(books, search(symbol_map, symb2), 'b', 100.0, 50.0);
    book_update(books, search(symbol_map, symb2), 'b', 101.0, 50.0);
    book_update(books, search(symbol_map, symb2), 'b', 100.0, 50.0);
    book_update(books, search(symbol_map, symb3), 'a', 100.0, 50.0);
    */
#endif

#if T3
    book_update();
#endif

    return 0;
}