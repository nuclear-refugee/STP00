//Last Build : 2016/11/8 4pm
#include "rev_STDIO.h"
#include <stdio.h>
#include <avr\io.h>
#define USART_baud 38400
//Internal Function Prototypes

int rev_stdio_putchar(char c, FILE *stream);
int rev_stdio_getchar(FILE *stream);

static FILE STDIO_BUFFER = FDEV_SETUP_STREAM(rev_stdio_putchar, rev_stdio_getchar, _FDEV_SETUP_RW);

int rev_stdio_putchar(char c, FILE *stream)
{
   if (c == '\n')
        rev_stdio_putchar('\r',stream);

    while((UCSR0A&(1<<UDRE0))==0);

    UDR0 = c;

    return 0;
}

int rev_stdio_getchar(FILE *stream)
{
	int UDR_Buff;

    while((UCSR0A&(1<<RXC0))==0);

	UDR_Buff = UDR0;

	rev_stdio_putchar(UDR_Buff,stream);

	return UDR_Buff;
}


/**
    Link STDIO buffer to USART0
**/
char rev_STDIO_set(void)
{
	unsigned int baud;

	baud = F_CPU/16/USART_baud-1;
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);
	UCSR0C |= (3<<UCSZ00);

	stdout = &STDIO_BUFFER;
	stdin = &STDIO_BUFFER;

	return 0;
}
