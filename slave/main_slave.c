#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\serial_packet.h"
#include ".\LIB\serial_packet_print.h"
//#include "..\rev_lib\ASA\ASA_UARTS.h"

serial_packet rec_state = serial_packet_initial;

int main(void)
{
    rev_STDIO_set();
    USART_set();
    uint8_t chk,data;
    //sei();
    printf("Start\n");
    // DDRB |= 7 << 5;
    // DDRF |= 1 << 4;
    // PORTB = 1 << 5;
    // PORTF = 1 << 4;
    while(1)
    {
    //    USART_receive(&data);
        printf("In: ");
        scanf("%u",&data);
        chk = unpack(&rec_state,&data);
        serial_packet_stat(&rec_state);
        serial_packet_chk(&chk);
        // chk = uarts_decode_buf_step();
        // if(chk != BUF_NULL)
        // {
        //     printf("UID:   %u\n", UartDecoder.uid);
        //     printf("WR :   %u\n", UartDecoder.wr);
        //     printf("ADDR:  %u\n", UartDecoder.address);
        //     printf("BYTES: %u\n", UartDecoder.bytes);
        //     printf("DATA:  %u\n", UartDecoder.data);
        //     printf("CHKSUM:%u\n", UartDecoder.chksum);
        // }

    }
}
