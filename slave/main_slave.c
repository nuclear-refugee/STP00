#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\serial_packet.h"
#include ".\LIB\serial_packet_print.h"
#include ".\LIB\fifo.h"
//#include "..\rev_lib\ASA\ASA_UARTS.h"

fifo_set usart_rx_fifo = fifo_initial;
serial_packet rec_state = serial_packet_initial;

ISR(USART1_RX_vect){
    fifo_push(&usart_rx_fifo,1,&UDR);
}

int main(void)
{
    rev_STDIO_set();
    USART_set();
    sei();
    uint8_t chk,data;
    printf("Start\n");
    while(1)
    {
        // printf("In: ");
        // scanf("%u",&data);
        chk = fifo_pop(&usart_rx_fifo,1, &data);
        if(chk){
            chk = unpack(&rec_state,&data); // unpack by continuous sending a byte data
            serial_packet_stat(&rec_state); // print out struct
            serial_packet_chk(&chk);    // print out ERROR
        }
        else
            serial_packet_timeout_count(&rec_state);
    }
}
