//---------- Library -----------//
#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\device.h"
#include ".\LIB\M128_UART.h"
#include <stdio.h>
#include <avr/io.h>
#include <inttypes.h>

//---------- Function -----------//

typedef struct stepper_module_setting_format
{
    uint8_t RW;
    uint8_t index;
    uint8_t data;
    uint8_t result;
}stp_set;

#define HEADER 0xAA
#define ERROR  0xA0

#define ERROR_HEADER    0
#define W_OK            1
#define R_OK            2
#define R_CHK_ERROR     3
uint8_t usart_tranceiver( uint8_t UID , uint8_t WR_ADDR , uint8_t BYTES , void * DATA_p )
{
    uint8_t chksum = 0 , data , i;

    chksum += HEADER;
    data = HEADER;
    USART_transmit( &data );

    chksum += UID;
    data = UID;
    USART_transmit( &data );

    chksum += WR_ADDR;
    data = WR_ADDR;
    USART_transmit( &data );

    if((WR_ADDR & 0x80))
    {   // write
        for( i = 0 ; i != BYTES ; ++i )
        {
            chksum += *((uint8_t *)DATA_p+i);
            data = *((uint8_t *)DATA_p+i);
            USART_transmit( &data );
        }
    }

    USART_transmit( &chksum );

    USART_receive( &data );
    switch( data )
    {
        case HEADER:
            if((WR_ADDR & 0x80))    // write ok
            {
                printf("write OK\n");
                return W_OK;
            }
            else    // read
            {
                chksum = HEADER;
                for( i = 0 ; i != BYTES ; i++ )
                {
                    USART_receive( &data );
                    *((uint8_t *)DATA_p + i) = data;
                    chksum += data;
                }
                USART_receive( &data );
                if( data == chksum )    // read ok
                {
                    printf("Read OK\n");
                    printf("chksum %u\n" , chksum );
                    return R_OK;
                }
                else    // read fail
                {
                    printf("data %u chksum %u\n",data,chksum);
                    return R_CHK_ERROR;
                }
            }
            break;
        case ERROR:
            USART_receive( &data );
            printf("ERROR %X\n",data);
            return data;
        default:
            printf("ERRORHEADER: %X\n",data);
            return ERROR_HEADER;
    }

}

int main(void) {
    uint8_t buff[8];
    uint16_t reg,bytes,wr,chk;
    uint8_t i=0;
//---------- Initial ---------//
    rev_STDIO_set();
    USART_set();
//---------- Loop------------//
    // while(1)
    // {
        /*
        for(i=0;i<30;i++)
        {
            USART_transmit(&i);
        }
        */

        // uint8_t rec_buff[3];
        // buff = 0xAA;
        // USART_transmit( &buff );
        // buff = 0x12;
        // USART_transmit( &buff );
        // buff = 0x01;
        // USART_transmit( &buff );
        // // buff = 0x81;
        // // USART_transmit( &buff );
        // buff = 189;
        // USART_transmit( &buff );
        //
        // USART_receive(rec_buff);
        // USART_receive(rec_buff+1);
        // USART_receive(rec_buff+2);
        // printf("%u %u %u\n",rec_buff[0],rec_buff[1],rec_buff[2]);
        // while(1);
        while(1){
            printf("START\n");
            scanf("%u",&wr);
            scanf("%u",&reg);
            scanf("%u",&bytes);
            if(wr)
            {
                for( i = 0 ; i != bytes ; i++ )
                {
                    scanf("%u",&buff[i]);
                }
                chk = M128_UARTM_trm(8,0x12,reg,bytes,buff);

            }
            else
            {
                chk = M128_UARTM_rec(8,0x12,reg,bytes,buff);
                printf("buff:");
                for( i = 0 ; i != bytes ; i++ )
                {
                    printf(" %u",buff[i]);
                }
                printf("\n");
            }
            printf("chk %u\n" , chk );
            // printf("reg: %u bytes: %u buff %u wr %u\n",reg,bytes,buff,(uint8_t)reg&0x80);
            // i = usart_tranceiver(0x12,(uint8_t)reg,(uint8_t)bytes,&buff);
            // printf("chk: %u\n",i);
            // if( !(reg & 0x80) )
            // {
            //     printf("buff:");
            //     for( i = 0 ; i != bytes ; i++ )
            //     {
            //         printf(" %u",(uint8_t)buff[i]);
            //     }
            //     printf("\n");
            // }

        }
        //USART_transmit(&i);
    // }

}
