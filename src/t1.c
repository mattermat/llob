#include <stdlib.h>
#include <string.h>

#include "t1.h"

#include <stdio.h>

struct Book *book_init(int num_of_books)
{
    struct Book *book = malloc(num_of_books * sizeof(struct Book));
    memset(book, 0, num_of_books * sizeof(struct Book));
    for (int i = 0; i < num_of_books; i++)
    {
        (book + i)->ask = PRICE_MAX_STEP - 1;
    }
    return book;
}

void book_update(struct Book *books, int book_index, char side, uint32_t price, float quantity)
{
    if (book_index == -1) return;

    struct Book *book_p = books + book_index;

    /* flow:
      - target book pointer is the pointer of books offsetted by index of target book
      - cast target book pointer to struct Book *
      - convert price to level
      - get the quantities array and index it using with the price
      - set the price level with the new quantity
     */
    uint32_t level = price / PRICE_STEP_SIZE + PRICE_MIN;
    book_p->quantities[level] = quantity;

    /* now update the bid / ask */
    if (quantity == 0)
    {
        // remove
        if (side == 'a')
        {
            // remove ask
            if (level == book_p->ask)
            {
                int previous = book_p->ask;
                for (int i = 1; i < previous; i++)
                {
                    if (book_p->quantities[previous+i] > 0.0)
                    {
                        book_p->ask = previous+i;
                        break;
                    }
                }
            }
        } else {
            // if removed bid is the best bid then update the bid
            if (level == book_p->bid)
            {
                int previous = book_p->bid;
                for (int i = 1; i < previous; i++)
                {
                    if (book_p->quantities[previous - i] > 0.0)
                    {
                        book_p->bid = previous-i;
                        break;
                    }
                }
            }
        }
    } else {
        // update
        if (side == 'a')
        {
            // update ask if the price level is lower than previous ask
            if (level < book_p->ask) book_p->ask = level;
        } else {
            // update bid if the price level is greater than previous bid
            if (level > book_p->bid) book_p->bid = level;
        }
    }

    return;
}


float book_get_bid_price(struct Book *books, int book_index)
{
    float result = (float) ((books+book_index)->bid * PRICE_STEP_SIZE);
    return result;
}

float book_get_ask_price(struct Book *books, int book_index)
{
    float result = (float) ((books+book_index)->ask * PRICE_STEP_SIZE);
    return result;
}

float book_get_bid_quant(struct Book *books, int book_index)
{
    struct Book *book_p = (books + book_index);
    return (book_p)->quantities[(book_p)->bid];
}

float book_get_ask_quant(struct Book *books, int book_index)
{
    struct Book *book_p = (books + book_index);
    return (book_p)->quantities[(book_p)->ask];
}