#define MAX_PRICE 100000L // 100k USDT
#define STEP_SIZE 1 // in cents of USDT, BTC on Bybit
#define MIN_LEVEL 0

#define BOOK_INCREMENT (100 / STEP_SIZE)
#define MAX_LEVEL (MAX_PRICE * 100 / STEP_SIZE)
