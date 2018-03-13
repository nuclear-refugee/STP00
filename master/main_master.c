//---------- Library -----------//
#include ".\LIB\rev_STDIO.h"
#include ".\LIB\uart.h"
#include ".\LIB\m128.h"
#include ".\LIB\M128_UART.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

//---------- Function -----------//

typedef struct stepper_module_setting_format
{
    uint8_t RW;
    uint8_t index;
    uint8_t data;
    uint8_t result;
}stp_set;

int main(void) {
    stp_set set;
    uint8_t buff;
    uint8_t i=0;
//---------- Initial ---------//
    rev_STDIO_set();
    USART_set();
//---------- Loop------------//
    while(1)
    {
        /*
        for(i=0;i<30;i++)
        {
            USART_transmit(&i);
        }
        */
        USART_transmit(&i);
        printf("continue?\n");
        scanf("%hhu",&i);
        i++;
    }

}
