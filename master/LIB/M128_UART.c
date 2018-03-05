
#include <avr/io.h>
#include <avr/interrupt.h>
#include "M128_UART.h"



char M128_UART_set( char LSbyte, char Mask, char Shift, void* Data_p)	
{
	char data = *((char*)Data_p); 
	
	if(LSbyte<203 || LSbyte>211) return 1;
	if(Shift<0	  || Shift>7   ) return 1;

	switch(LSbyte)
	{		
		case 203 : DDRD   = (DDRD 	& (~Mask)) | ((data<<Shift) & Mask);break;
		case 204 : DDRE   = (DDRE 	& (~Mask)) | ((data<<Shift) & Mask);break;
		case 206 : UCSR0A = (UCSR0A & (~Mask)) | ((data<<Shift) & Mask);break;
		case 207 : UCSR0B = (UCSR0B & (~Mask)) | ((data<<Shift) & Mask);break;
		case 208 : UCSR0C = (UCSR0C & (~Mask)) | ((data<<Shift) & Mask);break;
		case 209 : UCSR1A = (UCSR1A & (~Mask)) | ((data<<Shift) & Mask);break;
		case 210 : UCSR1B = (UCSR1B & (~Mask)) | ((data<<Shift) & Mask);break;
		case 211 : UCSR1C = (UCSR1C & (~Mask)) | ((data<<Shift) & Mask);break;
	}

	return 0;
}
char M128_UART_fpt(char LSbyte, char Mask, char Shift, char Data)
{
	if(LSbyte!=206 && LSbyte!=207 && LSbyte!=209 && LSbyte!=210) return 1;
	if(Shift<0	   || Shift>7	  )	return 1;

	switch(LSbyte)
	{			
		case 206 : UCSR0A = (UCSR0A & (~Mask)) | ((Data<<Shift) & Mask);break;
		case 207 : UCSR0B = (UCSR0B & (~Mask)) | ((Data<<Shift) & Mask);break;
		case 209 : UCSR1A = (UCSR1A & (~Mask)) | ((Data<<Shift) & Mask);break;	
		case 210 : UCSR1B = (UCSR1B & (~Mask)) | ((Data<<Shift) & Mask);break;	
	}
	return 0;
}

char M128_UART_put(char LSbyte, char Bytes, void *Data_p)	
{
	char data = *((char*)Data_p);
	unsigned int baud = *((unsigned int*)Data_p);
	
	if(LSbyte >7 || LSbyte <0)return 1;
	if(Bytes != 1 && Bytes != 2)return 1;
	

	switch(LSbyte)
	{
		case 0:
				if(Bytes == 1)
				{
					//transmit
					while (!( UCSR0A & (1<<UDRE0)));
					UDR0 = data;
					while (!( UCSR0A & (1<<TXC0)));
					break;
				}
				else
				{
						
				}
				
		case 3:
				if(Bytes == 1)
				{
					//transmit
					while (!( UCSR1A & (1<<UDRE1))); 	
					UDR1 = data;
					while (!( UCSR1A & (1<<TXC1)));
					break;
				}
				else
				{

				}
				
				
		case 1: 
				/* Set baud rate */
				baud = F_CPU/16/baud-1;
				UBRR0H = (unsigned char)(baud>>8);
				UBRR0L = (unsigned char)baud;
				break;
		case 4:
				/* Set baud rate */
				baud = F_CPU/16/baud-1;
				UBRR1H = (unsigned char)(baud>>8);
				UBRR1L = (unsigned char)baud;
				break;
	}	
	return 0;
}

char M128_UART_get( char LSbyte, char Bytes,void *Data_p)
{

	if(LSbyte != 0 && LSbyte != 3) return 1;
	if(Bytes != 1)return 1;

	if(LSbyte==0)
	{
		while ( !(UCSR0A & (1<<RXC0)) );	
		*((char*)Data_p) = UDR0;
	}
	if(LSbyte==3)
	{
		while ( !(UCSR1A & (1<<RXC1)) );	
		*((char*)Data_p) = UDR1;
	}
	
	return 0;
}

char M128_UART_fgt(char LSbyte, char Mask, char Shift, char *Data_p)
{
	if(LSbyte!=206 && LSbyte!=209) return 1;
	if(Shift!=6	   && Shift!=7   ) return 1;

	char Data = 0;

	switch(LSbyte)
	{			
		case 206 : Data = (UCSR0A & Mask) >> Shift;break;
		case 209 : Data = (UCSR1A & Mask) >> Shift;break;	
	}

	*((char*)Data_p) = Data;

	return 0;
}

char M128_UARTM_trm(char OneSLA, char OneReg, char UARTID, char RegAdd, char Bytes, void *Data_p)
{
	if(OneSLA!=0 && OneSLA!=1 )	return 1;
	if(OneReg!=0 && OneReg!=1 )	return 1;
	if(RegAdd<0) return 1;

	if(!OneSLA) 
	{
		DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
		PORTF |= 0X10;								//DIO0  1:write  0:read
		M128_UART_put(3,1, &UARTID);				//send UARTID
	}
	RegAdd |= 0x80; 								//force write action
	if(OneReg != 0)
	{
		M128_UART_put(3, 1, &RegAdd);					//send RegAdd
	}
				
	for(int i=0;i<Bytes;i++)
	{
		M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
	}

	return 0;
}

char M128_UARTM_rec(char OneSLA,char OneReg,char UARTID, char RegAdd, char Bytes, void *Data_p)
{
	if(OneSLA!=0 && OneSLA!=1) return 1;
	if(OneReg!=0 && OneReg!=1) return 1;
	if(RegAdd<0) return 1;

	if(!OneSLA) 
	{
		DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
		PORTF |= 0X10;								//DIO0  1:write  0:read
		M128_UART_put(3,1, &UARTID);				//send UARTID
	}

	RegAdd &= 0x7F; 								//force read action

	if(OneReg != 0)
	{
		M128_UART_put(3, 1, &RegAdd);				//send RegAdd
	}

	if(!OneSLA) 
	{		
		PORTF &= (~0X10);							//DIO0  1:write  0:read
	}

	for(int i=0;i<Bytes;i++)
	{
		M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data		
	}
	return 0;
}

char M128_UARTM_ftm(char OneSLA,char OneReg,char UARTID, char RegAdd, char Mask,  char Shift, char Data)
{
	char result = 0;
	//char Bytes = 1;

	if(OneSLA!=0 && OneSLA!=1 )	return 1;
	if(OneReg!=0 && OneReg!=1 )	return 1;
	if(RegAdd<0) return 1;
	
	if(!OneSLA) 
	{
		DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
		PORTF |= 0X10;								//DIO0  1:write  0:read
		M128_UART_put(3,1, &UARTID);				//send UARTID
	}

	RegAdd &= 0x7F; 								//force read action

	if(OneReg != 0)
	{
		M128_UART_put(3, 1, &RegAdd);				//send RegAdd
	}
	
	if(!OneSLA) 
	{
		PORTF &= (~0X10);							//DIO0  1:write  0:read
	}

	M128_UART_get(3, 1, &result);

	result = (result & (~Mask)) | ((Data<<Shift) & Mask);	//change value by mask shift

	if(!OneSLA) 
	{
		PORTF |= 0X10;								//DIO0  1:write  0:read
		M128_UART_put(3,1, &UARTID);				//send UARTID
	}

	RegAdd |= 0x80; 								//force write action

	if(OneReg != 0)
	{
		M128_UART_put(3, 1, &RegAdd);				//send RegAdd
	}

	//M128_UART_put(3, 1, &Bytes);					//send Bytes						
	M128_UART_put(3, 1, &result);					//send Data
	

	return 0;
}

char M128_UARTM_frc(char OneSLA,char OneReg,char UARTID, char RegAdd, char Mask, char Shift, char* Data_p)
{
	char result = 0;	//check varible
	//char Bytes = 1;
		
	if(OneSLA!=0 && OneSLA!=1 )	return 1;
	if(OneReg!=0 && OneReg!=1 )	return 1;
	if(RegAdd<0) return 1;

	if(!OneSLA) 
	{
		DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
		PORTF |= 0X10;								//DIO0  1:write  0:read
		M128_UART_put(3,1, &UARTID);				//send UARTID
	}

	RegAdd &= 0x7F; 								//force read action
	if(OneReg != 0)
	{
		M128_UART_put(3, 1, &RegAdd);				//send RegAdd
	}

	
	if(!OneSLA) 
	{
		PORTF &= (~0X10);							//DIO0  1:write  0:read
	}
	
	M128_UART_get(3, 1, &result);

	result = (result & (~Mask)) | ((*Data_p<<Shift) & Mask);	//change value by mask shift

	*Data_p = result;	//return the data from slave and give it to "char* Data"
	
	return 0;
}
