#include "m128.h"
void USART_set(void)
{
    // 12bits with U2X = 0 UBRR = Fosc/16/(BAUD = 38400)-1 = 17
    UBRRH = 0;
    UBRRL = 17;
    UCSRB = 0b10011000;
    UCSRC = 0b00000110;
    USART_DDR |= (1<<USART_TX);
}
/**
    Transmit 8bit data via USART
**/
void USART_transmit(uint8_t *data)
{
    while(!( UCSRA & (1<<UDRE)));
    UDR = *data;
}

void USART_receive(uint8_t *data)
{
    while ( !(UCSRA & (1<<RXC)) );
    *data = UDR;
}
