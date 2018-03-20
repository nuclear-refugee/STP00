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
    uint8_t chk,data;
    rev_STDIO_set();
    USART_set();
    sei();

    printf("Start\n");
    while(1)
    {
        // printf("In: ");
        // scanf("%u",&data);
        chk = fifo_pop(&usart_rx_fifo,1, &data);
        if(chk){    // FIFO pop 1 byte
            chk = unpack(&rec_state,&data); // unpack by continuous sending a byte data
            // serial_packet_stat(&rec_state); // print out struct
            // serial_packet_chk(&chk);    // print out ERROR
            if(chk == Packet_OK){
                if(rec_state.WR)  // write
                {
                }
                else    // read
                {

                }
                serial_packet_state_reset(&rec_state);
            }
        }
        else
        {    // FIFO pop no data
            chk = serial_packet_timeout_count(&rec_state);
            if(chk)
                printf("TimeOut!!\n");
        }
    }
}
