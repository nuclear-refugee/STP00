#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\serial_packet_jsb.h"
#include ".\LIB\fifo.h"
#include ".\LIB\Reg_sys.h"

fifo_set usart_rx_fifo;
fifo_set usart_tx_fifo;
serial_packet rec_state;

ISR(USART1_RX_vect){
    fifo_push(&usart_rx_fifo,1,&UDR);
}

ISR(USART1_TX_vect)
{
    fifo_pop(&usart_tx_fifo,1,&UDR);
}

// void tx_trig(fifo_set * fifo)
// {
//     uint8_t data;
//     fifo_pop(fifo,1,&data);
//     USART_transmit(data);
// }

int main(void)
{
    uint8_t chk,data;
    uint8_t chksum;
    uint8_t  reg_8[2] = {1,2};
    uint16_t reg_16[2] = {3,4};
    uint8_t trigger = 0,i,j;
    rev_STDIO_set();
    USART_set();

    REG_PTR_SIZE * reg_ptr = Register_info_initial(4);

    Register_info_fillin(&reg_ptr[0], &reg_8[0],  1);
    Register_info_fillin(&reg_ptr[1], &reg_8[1],  1);
    Register_info_fillin(&reg_ptr[2], &reg_16[0], 2);
    Register_info_fillin(&reg_ptr[3], &reg_16[1], 2);

    rec_state.reg_index_max = 4;
    // printf("start\n");
    sei();
    while(1)
    {
        chk = fifo_pop(&usart_rx_fifo,1, &data);
        if(chk)
        {    // FIFO pop 1 byte
            chk = unpack(&rec_state,reg_ptr,&data); // unpack by continuous sending a byte data
            if(chk == Packet_OK)
            {   // get a complete packet
                pack_respond( &rec_state, reg_ptr , &usart_tx_fifo );
                // printf("reg[0] = %d , reg[1] = %d , reg[2] = %d , reg[3] = %d\n",reg_8[0],reg_8[1],reg_16[0],reg_16[1]);
            }
            // else if(chk)
            // {   // ERROR RETURN HEADER_ERROR | chk (error code)
            //     data = P_ERROR;
            //     fifo_push( &usart_tx_fifo , 1 , &data );
            //     fifo_push( &usart_tx_fifo , 1 , &chk);
            //     if(trigger)
            //     {   // if tx not working(tx stop when fifo empty), then trigger
            //         fifo_pop( &usart_tx_fifo , 1 , &data );
            //         USART_transmit( &data );
            //         trigger = 0;
            //     }
            // }
        }
        else
        {    // FIFO pop no data
            chk = serial_packet_timeout_count(&rec_state);
            // if(chk)
            //     printf("TimeOut!!\n");
        }
    }
}
