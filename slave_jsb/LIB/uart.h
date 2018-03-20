#ifndef __UART_H__
#define __UART_H__

#include <inttypes.h>
#include "m128.h"

void USART_set(void);
void USART_transmit(uint8_t *data);
void USART_receive(uint8_t *data);

#endif
