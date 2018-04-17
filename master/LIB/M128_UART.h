//################################
// 	 Date     :  2018.03.27 	 #
// 	 Author   :  Wang Liang		 #
// 	 Location :  NCU MVMC Lab 	 #
//	 Version  :  2.0			 #
//################################


#ifndef _M128_UART_
#define _M128_UART_

#define RS485_WRITE (PORTF |= 0X10)
#define RS485_READ  (PORTF &= (~0X10))

#define initial {{0},{0}}
#define Register_max 5

typedef void (*UART_int)(void);
typedef struct
{
	UART_int U_TX1[Register_max];
	UART_int U_RX1[Register_max];
}UART_TXRX;

#define HEADER_UART 0xAA

char M128_UART_put( char LSbyte, char Bytes, void *Data_p);
char M128_UART_set( char LSbyte, char Mask, char Shift, void* Data_p);

char M128_UART_get( char LSbyte, char Bytes, void *Data_p);

char M128_UART_fpt(char LSbyte, char Mask, char Shift, char Data);
char M128_UART_fgt(char LSbyte, char Mask, char Shift, char *Data_p);

char M128_UARTM_trm(char Mode, char UID, char RAdd, char Bytes, void* Data_p);
char M128_UARTM_rec(char Mode, char UID, char RAdd, char Bytes, void* Data_p);

char M128_UARTM_ftm(char Mode, char UID, char RAdd, char Mask, char Shift, char Data);
char M128_UARTM_frc(char Mode, char UID, char RAdd, char Mask, char Shift, char* Data_p);

char M128_UARTTX_isr(char Number, void (*function)(void));
char M128_UARTRX_isr(char Number, void (*function)(void));

#endif
