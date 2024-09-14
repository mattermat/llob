#include <bits/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>


#include "defines.h"
#include "plain.h"
#include "fast.h"
#include "fastnsq.h"

#define CONVERGENCE_TEST 0

static inline uint64_t get_timestamp(struct timespec* ts)
{
    clock_gettime(CLOCK_MONOTONIC, ts);
    return (ts->tv_nsec + (ts->tv_sec * 1.0e9));
}

struct Plain *plain_book;
struct Fast *fast_book;
struct Fast_nsq *fastnsq_book;

int main()
{
    /* NOTE: this version of data.mng is not sequential */
    const char *filename = "/home/mattia/Projects/llob/single_lob/data.mgn";
    FILE* file = fopen(filename, "r");
    char line[4096];

    if(file == NULL) return 1;

#if CONVERGENCE_TEST
    printf("Convergence test...\n");

    plain_book = malloc(sizeof(struct Plain));
    plain_init(plain_book);
    fastnsq_book = malloc(sizeof(struct Fast_nsq));
    fastnsq_init(fastnsq_book);
    /*
    fast_book = malloc(sizeof(struct Fast));
    fast_init(fast_book);
    */

    f64 plain_ask, plain_bid, fast_ask, fast_bid;

    int count = {0};
    while (fgets(line, sizeof(line), file))
    {
        char str_price[11] = {0};
        char str_quantity[15] = {0};
        char side = {0};
        memcpy(&side, &line[28], 1);
        size_t price_chr_len = strchr(&line[35], '\'') - &line[35];
        memcpy(str_price, &(line[35]), price_chr_len);
        int next = 35 + price_chr_len + 4;
        size_t quant_chr_len = strchr(&line[next], '\'') - &line[next];
        memcpy(str_quantity, &(line[next]), quant_chr_len);
        f64 price = strtod(str_price, NULL);
        f64 quantity = strtod(str_quantity, NULL);

        plain_update(plain_book, side, price, quantity);
        fastnsq_update(fastnsq_book, side, price, quantity);

        plain_ask = plain_get_ask_price(plain_book);
        plain_bid = plain_get_bid_price(plain_book);
        fast_ask = fastnsq_get_ask_price(fastnsq_book);
        fast_bid = fastnsq_get_bid_price(fastnsq_book);

        bool ask_test = (plain_ask == fast_ask);
        bool bid_test = (plain_bid == fast_bid);
        /*
        printf("changing: %c[%f]\n", side, price);
        printf("%i\n", count);
        printf("ask in plain: %f\n", plain_ask);
        printf("ask in fast:  %f\n", fast_ask);
        printf("ok\n\n");
        */
        if (!ask_test)
        {
            printf("error {%i} (it starts from 0)\n", count);

            printf("ask in plain: %f\n", plain_ask);
            printf("ask in fast:  %f\n", fast_ask);

            printf("bid in plain: %f\n", plain_bid);
            printf("bid in fast:  %f\n", fast_bid);
            return 1;
        }
        ++count;
    }
    printf("Convergence test passed!\n");

#else
    /* Plain book */
    plain_book = malloc(sizeof(struct Plain));
    plain_init(plain_book);

    /* get timestamp */
    uint64_t start, end;
    struct timespec ts;
    start = get_timestamp(&ts);

    /* feed book */
    f64 plain_a, plain_b;
    while (fgets(line, sizeof(line), file))
    {
        //char str_timestamp[14] = {0};
        //char str_symbol[8] = {0};
        //char str_side[4] = {0};
        char str_price[11] = {0};
        char str_quantity[15] = {0};
        //memcpy(str_timestamp, &(line[1]), 13);
        //memcpy(str_symbol, &(line[17]), 7);
        //memcpy(str_side, &(line[28]), 3);
        char side = {0};
        memcpy(&side, &line[28], 1);
        size_t price_chr_len = strchr(&line[35], '\'') - &line[35];
        memcpy(str_price, &(line[35]), price_chr_len);
        int next = 35 + price_chr_len + 4;
        size_t quant_chr_len = strchr(&line[next], '\'') - &line[next];
        memcpy(str_quantity, &(line[next]), quant_chr_len);
        f64 price = strtod(str_price, NULL);
        f64 quantity = strtod(str_quantity, NULL);
        //printf("new: [p: %f\tq: %f]\n", price, quantity);
        plain_update(plain_book, side, price, quantity);
        #if 0
        plain_a = plain_get_ask_price(plain_book);
        plain_b = plain_get_bid_price(plain_book);
        #endif
    }
    plain_a = plain_get_ask_price(plain_book);
    plain_b = plain_get_bid_price(plain_book);
    printf("[plain] ask: %f\tbid: %f\n", plain_a, plain_b);
    /* get timestamp */
    end = get_timestamp(&ts);

    /* measure */
    printf("plain book: %li ns.\n", (end-start));

    /* reset the file pointer */
    rewind(file);

    /* Fast nsq book */
    fastnsq_book = malloc(sizeof(struct Fast_nsq));
    fastnsq_init(fastnsq_book);
    /* get timestamp */
    start = get_timestamp(&ts);

    /* feed book */
    f64 fast_a, fast_b;
    int count = 0;
    while (fgets(line, sizeof(line), file))
    {
        char str_price[11] = {0};
        char str_quantity[15] = {0};
        char side = {0};
        memcpy(&side, &line[28], 1);
        size_t price_chr_len = strchr(&line[35], '\'') - &line[35];
        memcpy(str_price, &(line[35]), price_chr_len);
        int next = 35 + price_chr_len + 4;
        size_t quant_chr_len = strchr(&line[next], '\'') - &line[next];
        memcpy(str_quantity, &(line[next]), quant_chr_len);
        f64 price = strtod(str_price, NULL);
        f64 quantity = strtod(str_quantity, NULL);
        fastnsq_update(fastnsq_book, side, price, quantity);
        #if 0
        fast_a = fastnsq_get_ask_price(fastnsq_book);
        fast_b = fastnsq_get_bid_price(fastnsq_book);
        #endif
    }
    fast_a = fastnsq_get_ask_price(fastnsq_book);
    fast_b = fastnsq_get_bid_price(fastnsq_book);
    printf("[fast]  ask: %f\tbid: %f\n", fast_a, fast_b);

    /* get timestamp */
    end = get_timestamp(&ts);

    /* measure */
    printf("fast book:  %li ns.\n", (end-start));

#endif

    /* free */
    fclose(file);

    return 0;
}
