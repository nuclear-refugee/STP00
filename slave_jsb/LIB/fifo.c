#include "fifo.h"

uint8_t fifo_pop(fifo_set *ptr,uint8_t byte,uint8_t *d_ptr)
{
    uint8_t i;
    for(i=0;i < byte; i++)
    {
        if(ptr->in != ptr->out)
            *(d_ptr+i) = ptr->buffer[ptr->out];
        else
        {
            if(ptr->full)
            {
                *(d_ptr+i) = ptr->buffer[ptr->out];
                ptr->full = 0;
            }
            else
                // No data in buffer
                return i;
        }
        ptr->out ++;
        if(ptr->out == fifo_size) ptr->out = 0;
    }
    return i;
}

uint8_t fifo_push(fifo_set *ptr,uint8_t byte,uint8_t *d_ptr)
{
    uint8_t i=0;
    if(!ptr->full)
    {
        for(i=0;i < byte; i++)
        {
            ptr->buffer[ptr->in] = *(d_ptr+i);
            ptr->in ++;
            if(ptr->in == fifo_size) ptr->in=0;
            if(ptr->in == ptr->out)
            {
                // buffer full
                ptr->full = 1;
                return 1;
            }
        }
    }
    return 0;
}
