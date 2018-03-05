#ifndef __M128_H__
#define __M128_H__

#include <avr\io.h>

// USART
#define UDR	    UDR1
#define UCSRA   UCSR1A
#define UCSRB   UCSR1B
#define UCSRC   UCSR1C
#define UBRRH   UBRR1H
#define UBRRL   UBRR1L
#define USART_RX_vect USART1_RX_vect
#define USART_DDR   DDRD
#define USART_PORT  PORTD
#define USART_RX    PD2
#define USART_TX    PD3

#endif
