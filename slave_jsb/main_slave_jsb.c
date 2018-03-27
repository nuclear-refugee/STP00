#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\serial_packet_jsb.h"
// #include ".\LIB\serial_packet_print_jsb.h"
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
    uint8_t  reg_8[2] ;
    uint16_t reg_16[2];
    rev_STDIO_set();
    USART_set();

    REG_PTR_SIZE * reg_ptr = Register_info_initial(4);

    Register_info_fillin(&reg_ptr[0], &reg_8[0],  1);
    Register_info_fillin(&reg_ptr[1], &reg_8[1],  1);
    Register_info_fillin(&reg_ptr[2], &reg_16[0], 2);
    Register_info_fillin(&reg_ptr[3], &reg_16[1], 2);

    rec_state.reg_index_max = 4;

    // for(int i = 0 ; i < 2 ; i++ )
    // {
    //     printf("reg_[%u] = %u Bytes %u\n",i,*(uint8_t*)(reg_ptr[i].ptr),reg_ptr[i].bytes);
    // }
    // for(int i = 2 ; i < 4 ; i++ )
    // {
    //     printf("reg_[%u] = %u Bytes %u\n",i,*(uint16_t*)(reg_ptr[i].ptr),reg_ptr[i].bytes);
    // }

    sei();

    // printf("Start\n");
    while(1)
    {
        // printf("In: ");
        // scanf("%u",&data);
        chk = fifo_pop(&usart_rx_fifo,1, &data);
        if(chk)
        {    // FIFO pop 1 byte
            chk = unpack(&rec_state,reg_ptr,&data); // unpack by continuous sending a byte data
            // serial_packet_stat(&rec_state); // print out struct
            // serial_packet_chk(&chk);    // print out ERROR
            if(chk == Packet_OK)
            { // get a complete packet
                serial_packet_state_reset(&rec_state);
                if(rec_state.WR)   // written
                {
                    // if ok, then return header as acknowledge
                    data = P_HEADER;
                    fifo_push( &usart_tx_fifo , 1, &data );
                }
                else    // read
                {
                    // return HEADER | Data | chksum
                    chksum = P_HEADER;
                    fifo_push( &usart_tx_fifo , 1 , &data );
                    for(int i = 0; i < reg_ptr[rec_state.addr].bytes ; i++ )
                    {
                        fifo_push( &usart_tx_fifo , 1 , (uint8_t *)(reg_ptr[rec_state.addr].ptr)+i );
                        chksum += *((uint8_t *)(reg_ptr[rec_state.addr].ptr)+i);
                    }
                    fifo_push( &usart_tx_fifo, 1 , &chksum);
                }
                break;
            }
            else if(chk)    // return error, except zero(ok)
            {
                data = P_ERROR;
                fifo_push( &usart_tx_fifo , 1 , &data );
                fifo_push( &usart_tx_fifo , 1 , &chk);
            }
            if(fifo_empty( &usart_tx_fifo ))
            {   // if tx not working(tx stop when fifo empty), then trigger
                fifo_pop( &usart_tx_fifo , 1 , &data);
                USART_transmit(&data);
            }
        }
        else
        {    // FIFO pop no data
            chk = serial_packet_timeout_count(&rec_state);
            // if(chk)
            //     printf("TimeOut!!\n");
        }
    }
}
