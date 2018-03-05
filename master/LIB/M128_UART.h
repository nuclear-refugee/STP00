//################################
// 	 Date     :  2017.11.23 	 #
// 	 Author   :  Wang Liang		 #
// 	 Location :  NCU MVMC Lab 	 #
//	 Version  :  1.1			 #
//################################


#ifndef _M128_UART_
#define _M128_UART_



char M128_UART_set( char LSbyte, char Mask, char Shift, void* Data_p);
char M128_UART_put( char LSbyte, char Bytes, void *Data_p);
char M128_UART_get( char LSbyte, char Bytes, void *Data_p);

char M128_UART_fpt(char LSbyte, char Mask, char Shift, char Data);
char M128_UART_fgt(char LSbyte, char Mask, char Shift, char *Data_p);

char M128_UARTM_trm(char OneSLA, char OneReg, char UARTID, char RegAdd, char Bytes, void *Data_p);
char M128_UARTM_rec(char OneSLA, char OneReg, char UARTID, char RegAdd, char Bytes, void *Data_p);

char M128_UARTM_ftm(char OneSLA, char OneReg, char UARTID, char RegAdd, char Mask,  char Shift, char Data);
char M128_UARTM_frc(char OneSLA, char OneReg, char UARTID, char RegAdd, char Mask,  char Shift, char* Data_p);



#endif
