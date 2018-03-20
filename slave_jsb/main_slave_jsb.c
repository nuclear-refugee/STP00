#include <avr\io.h>
#include <util\delay.h>
#include <avr\interrupt.h>
#include <stdio.h>
#include <inttypes.h>

#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\serial_packet_jsb.h"
#include ".\LIB\serial_packet_print_jsb.h"
#include ".\LIB\fifo.h"
#include ".\LIB\Reg_sys.h"
//#include "..\rev_lib\ASA\ASA_UARTS.h"

fifo_set usart_rx_fifo = fifo_initial;
serial_packet rec_state = serial_packet_initial;

ISR(USART1_RX_vect){
    fifo_push(&usart_rx_fifo,1,&UDR);
}

int main(void)
{
    uint8_t chk,data;
    uint8_t  reg_8[2];
    uint16_t reg_16[2];
    rev_STDIO_set();
    USART_set();

    REG_PTR_SIZE * reg_ptr = Register_info_initial(4);

    Register_info_fillin(&reg_ptr[0], &reg_8[0],  1);
    Register_info_fillin(&reg_ptr[1], &reg_8[1],  1);
    Register_info_fillin(&reg_ptr[2], &reg_16[0], 2);
    Register_info_fillin(&reg_ptr[3], &reg_16[1], 2);

    sei();

    printf("Start\n");
    while(1)
    {
        printf("In: ");
        scanf("%u",&data);
        // chk = fifo_pop(&usart_rx_fifo,1, &data);
        if(chk){    // FIFO pop 1 byte
            chk = unpack(&rec_state,reg_ptr,&data); // unpack by continuous sending a byte data
            serial_packet_stat(&rec_state); // print out struct
            serial_packet_chk(&chk);    // print out ERROR
        }
        else
        {    // FIFO pop no data
            chk = serial_packet_timeout_count(&rec_state);
            if(chk)
                printf("TimeOut!!\n");
        }
    }
}
