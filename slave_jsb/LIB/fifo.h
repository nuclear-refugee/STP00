#ifndef __FIFO_H__
#define __FIFO_H__

#include <inttypes.h>

#define fifo_size 32
#define fifo_initial {0,0,0,{0}}

typedef struct fifo
{
    volatile uint8_t in;
    volatile uint8_t out;
    volatile uint8_t full;
    volatile uint8_t buffer[fifo_size];
} fifo_set;

uint8_t fifo_pop(fifo_set *ptr,uint8_t byte,uint8_t *d_ptr);
uint8_t fifo_push(fifo_set *ptr,uint8_t byte,uint8_t *d_ptr);


#endif
