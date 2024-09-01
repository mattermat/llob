#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "plain.h"
#include "fast.h"

struct Plain plain_book;

/* helper function to compare floating point numbers */
int f64_is_equal(f64 a, f64 b)
{
    return fabs(a - b) < 1e-6;
}


int main()
{
    plain_init(&plain_book);

    /* Test Plain */

    // Test 1: Add a bid
    plain_update(&plain_book, 'b', 100.0, 10.0);
    assert(f64_is_equal(plain_get_bid_price(&plain_book), 100.0));
    assert(f64_is_equal(plain_get_quantity_at_price(&plain_book, 'b', 100.0), 10.0));
    assert(f64_is_equal(
        plain_get_bid_quantity(&plain_book),
        plain_get_quantity_at_price(&plain_book, 'b', 100.0)
    ));

    // Test 2: Add an ask
    plain_update(&plain_book, 'a', 101.0, 5.0);
    assert(f64_is_equal(plain_get_ask_price(&plain_book), 101.0));
    assert(f64_is_equal(plain_get_quantity_at_price(&plain_book, 'a', 101.0), 5.0));
    assert(f64_is_equal(
        plain_get_ask_quantity(&plain_book),
        plain_get_quantity_at_price(&plain_book, 'a', 101.0)
    ));

    // Test 3: Update existing bid
    plain_update(&plain_book, 'b', 100.0, 15.0);
    assert(f64_is_equal(plain_get_quantity_at_price(&plain_book, 'b', 100.0), 15.0));

    // Test 5: Add multiple levels
    plain_update(&plain_book, 'b', 99.0, 20.0);
    plain_update(&plain_book, 'b', 98.0, 30.0);
    plain_update(&plain_book, 'a', 102.0, 15.0);
    plain_update(&plain_book, 'a', 103.0, 25.0);
    assert(f64_is_equal(plain_get_bid_price(&plain_book), 100.0));
    assert(f64_is_equal(plain_get_ask_price(&plain_book), 101.0));

    // Test 4: Remove a bid (set quantity to 0)
    plain_update(&plain_book, 'b', 100.0, 0.0);
    assert(f64_is_equal(plain_get_quantity_at_price(&plain_book, 'b', 100.0), 0.0));
    assert(f64_is_equal(plain_get_bid_price(&plain_book), 99.0));
    assert(f64_is_equal(
        plain_get_bid_quantity(&plain_book),
        plain_get_quantity_at_price(&plain_book, 'b', 99.0)
    ));

    // TODO: remove ask (set quantity to 0)
    plain_update(&plain_book, 'a', 101.00, 0.0);
    assert(f64_is_equal(plain_get_quantity_at_price(&plain_book, 'a', 101.00), 0.0));
    assert(f64_is_equal(plain_get_ask_price(&plain_book), 102.0));
    assert(f64_is_equal(
        plain_get_ask_quantity(&plain_book),
        plain_get_quantity_at_price(&plain_book, 'a', 102.0)
    ));

    // Test 6: Update best bid/ask
    plain_update(&plain_book, 'b', 100.5, 5.0);
    plain_update(&plain_book, 'a', 100.75, 3.0);
    assert(f64_is_equal(plain_get_bid_price(&plain_book), 100.5));
    assert(f64_is_equal(plain_get_ask_price(&plain_book), 100.75));

    printf("All tests passed!\n");

    return 0;
}
