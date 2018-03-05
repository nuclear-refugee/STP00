#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\fifo.h"

typedef struct stepper_module_setting_format
{
    uint8_t RW;
    uint8_t index;
    uint8_t data;
    uint8_t result;
}stp_set;

fifo_set rx_fifo;

// 封包解讀
uint8_t package_read(fifo_set *ptr)
{
    
    return 0;
}


ISR(USART_RX_vect)
{
    fifo_push(&rx_fifo,1,&(UDR));
}

int main(void)
{
    rev_STDIO_set();
    USART_set();
    uint8_t chk,data;
    sei();

    while(1)
    {
        chk = fifo_pop(&rx_fifo,1,&data);
        if(!chk)
        {
            //printf("%u\n",data);
        }
    }
}
