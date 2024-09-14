#pragma once

#define MAX_PRICE 100000L // 100k USDT
#define STEP_SIZE 1 // in cents of USDT, BTC on Bybit
#define MIN_LEVEL 0

#define MAX_LEVEL (MAX_PRICE * 100 / STEP_SIZE)

/* SEQUENTIAL should be 1 only when limit order updates do not
    overlap between bid and ask.
   If order updates are provided as bulk updates, maybe the client
   could update to the new best ask before getting the update of the
   new best bid.
*/
#define SEQUENTIAL 0
