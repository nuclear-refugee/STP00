#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"

typedef struct serial_packet_format
{
    volatile uint8_t status;
    volatile uint8_t Header_count;
    volatile uint8_t RW;
    volatile uint8_t index;
    volatile uint8_t data;
    volatile uint8_t result;
} serial_packet;

#define serial_packet_initial {0,0,0,0,0,0}

serial_packet rec_state = serial_packet_initial;

// Packet receiver status
#define Pst_Header 1
#define Pst_UID    2
#define Pst_ADDR   3
#define Pst_BYTES  4
#define Pst_DATA   5
#define Pst_Chksum 6

#define P_Header 0xAA

ISR(USART_RX_vect)
{
    switch (rec_state.status) {
        case Pst_Header:{
            if(UDR == P_Header)
                rec_state.Header_count++;
            break;
        }
        case

    }
}

int main(void)
{
    rev_STDIO_set();
    USART_set();
    uint8_t chk,data;
    sei();

    while(1)
    {
        if(!chk)
        {
            //printf("%u\n",data);
        }
    }
}
