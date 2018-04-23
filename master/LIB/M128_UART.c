
#include <avr/io.h>
#include <avr/interrupt.h>
#include "M128_UART.h"

UART_TXRX M128_UART_isr =  initial;

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
	unsigned long int baud = *((unsigned long int*)Data_p);
	unsigned long int count = 0;

	if(LSbyte >7 || LSbyte <0)return 1;
	if(Bytes != 1 && Bytes != 2)return 1;


	switch(LSbyte)
	{
		case 0:
				//transmit UART0
				count = 0;
				while (!( UCSR0A & (1<<UDRE0)))
				{
					count++;
					if(count == 65536)
					{
						UDR0 = 0;
						return 2;
					}
				}
				UDR0 = data;
				count = 0;
				while (!( UCSR0A & (1<<TXC0)))
				{
					count++;
					if(count == 65536)
					{
						UDR0 = 0;
						return 2;
					}
				}
				UCSR0A |= 1<<TXC0;
				break;
		case 3:
				//transmit UART1
				count = 0;
				while (!( UCSR1A & (1<<UDRE1)))
				{
					count++;
					if(count == 65536)
					{
						UDR1 = 0;
						return 2;
					}
				}
				UDR1 = data;
				count = 0;
				while (!( UCSR1A & (1<<TXC1)))
				{
					count++;
					if(count == 65536)
					{
						UDR1 = 0;
						return 2;
					}
				}
				UCSR1A |= 1<<TXC1;
				break;
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
	unsigned long int count = 0;

	if(LSbyte != 0 && LSbyte != 3) return 1;
	if(Bytes != 1)return 1;

	if(LSbyte==0)
	{
		while ( !(UCSR0A & (1<<RXC0)) )
		{
			count++;
			if(count == 65536)
			{
				*((char*)Data_p) = 0;
				return 2;
			}
		}
		*((char*)Data_p) = UDR0;
	}
	if(LSbyte==3)
	{
		count = 0;
		while ( !(UCSR1A & (1<<RXC1)) )
		{
			count++;
			if(count == 65536)
			{
				*((char*)Data_p) = 0;
				return 2;
			}
		}
		*((char*)Data_p) = UDR1;
	}
	return 0;
}

char M128_UART_fgt(char LSbyte, char Mask, char Shift, char *Data_p)
{
	if(LSbyte!=206 && LSbyte!=209) return 1;
	if(Shift!=6	   && Shift!=7   ) return 1;

	char Data = 0;

	switch((int)LSbyte)
	{
		case 206 : Data = (UCSR0A & Mask) >> Shift;break;
		case 209 : Data = (UCSR1A & Mask) >> Shift;break;
	}
	*((char*)Data_p) = Data;
	return 0;
}

char M128_UARTM_trm(char Mode, char UID, char RAdd, char Bytes, void *Data_p)
{
	if(Mode<0 || Mode>8)return 1;
	if(UID<0)			return 2;
	if(RAdd<0)			return 3;
	if(Bytes<0)			return 4;

	char trans_temp = 0, Header = HEADER_UART, get_back_data = 0;
	uint8_t check,checksum = 0;

	switch((int)Mode)
	{
		case 0:	// Mode 0
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 1:	//Mode 1
			trans_temp = (RAdd | 0x80);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return 5;
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 2: //Mode 2
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return 5;
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 3:	// Mode 3
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return 5;
			trans_temp = (RAdd | 0x80);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return 5;
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 4: // Mode 4 �줸�դϧ�(Mode 0)
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return check;
			}
			break;
		case 5:	// Mode 5 �줸�դϧ�(Mode 1)
			trans_temp = (RAdd | 0x80);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return 5;
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 6: // Mode 6 �줸�դϧ�(Mode 2)
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return 5;
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 7: // Mode 7 �줸�դϧ�(Mode 3)
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return 5;
			trans_temp = (RAdd | 0x80);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return 5;
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
			}
			break;
		case 8: //Mode 8
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &Header);					//send RAdd
			if(check) return 5;
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return 5;
			trans_temp = (RAdd | 0x80);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return 5;
            checksum = UID + trans_temp;
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_put(3, 1, (char*)Data_p+i);		//send Data
				if(check) return 5;
				checksum += *((char*)Data_p+i);
			}
			check = M128_UART_put(3, 1, &checksum);
			if(check) return 5;
			RS485_READ;							//DIO0  1:write  0:read
			check = M128_UART_get(3, 1, &get_back_data);		//receive Data
			if(check) return 5;
			break;
	}

	return 0;
}

char M128_UARTM_rec(char Mode, char UID, char RAdd, char Bytes, void* Data_p)
{
	if(Mode<0 || Mode>8)return 1;
	if(UID<0)			return 2;
	if(RAdd<0)			return 3;
	if(Bytes<0)			return 4;

	char trans_temp = 0, Header = HEADER_UART;
	uint8_t check, checksum = 0;

	switch((int)Mode)
	{
		case 0:
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 1:
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			PORTF |= 0X10;								//DIO0  1:write  0:read
			trans_temp = (RAdd & 0x7F);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return check;
			PORTF &= (~0X10);							//DIO0  1:write  0:read
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 2:
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			PORTF |= 0X10;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);				//send UID
			if(check) return check;
			PORTF &= (~0X10);							//DIO0  1:write  0:read
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 3:
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);				//send UID
			if(check) return check;
			trans_temp = (RAdd & 0x7F);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return check;
			RS485_READ;										//DIO0  1:write  0:read
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 4: // Mode 4 �줸�դϧ�(Mode 0)
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 5: // Mode 5 �줸�դϧ�(Mode 1)
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			trans_temp = (RAdd & 0x7F);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return check;
			RS485_READ;							//DIO0  1:write  0:read
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 6: // Mode 6 �줸�դϧ�(Mode 2)
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);				//send UID
			if(check) return check;
			RS485_READ;									//DIO0  1:write  0:read
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 7: // Mode 7 �줸�դϧ�(Mode 3)
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;								//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &UID);				//send UID
			if(check) return check;
			trans_temp = (RAdd & 0x7F);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return check;
			RS485_READ;										//DIO0  1:write  0:read
			for(int i=Bytes-1;i<0;i--)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
			}
			break;
		case 8:
			DDRF  |= 0X10;								//RS-485 use ASA 20PIN.DIO0
			RS485_WRITE;									//DIO0  1:write  0:read
			check = M128_UART_put(3, 1, &Header);					//send RAdd
			if(check) return check;
			check = M128_UART_put(3, 1, &UID);						//send RAdd
			if(check) return check;
			trans_temp = (RAdd & 0x7F);
			check = M128_UART_put(3, 1, &trans_temp);				//send RAdd
			if(check) return check;
            checksum = UID + trans_temp;
			check = M128_UART_put(3, 1, &checksum);					//send checksum
			if(check) return check;
			RS485_READ;
			check = M128_UART_get(3, 1, &Header);					//receive header
			if(check) return check;
			for(int i=0;i<Bytes;i++)
			{
				check = M128_UART_get(3, 1, (char*)Data_p+i);		//receive Data
				if(check) return check;
				checksum += *((char*)Data_p+i);
			}
			check = M128_UART_get(3, 1, &Header);					//receive checksum
			if(check) return check;

			break;
	}
	return 0;
}

char M128_UARTM_ftm(char Mode,char UID,char RAdd,char Mask,char Shift, char Data)
{
	if(Mode<0 || Mode>8)return 1;
	if(UID<0)			return 2;
	if(RAdd<0)			return 3;

	if (Mode == 4) Mode = 0;
	else if (Mode == 5) Mode = 1;
	else if (Mode == 6) Mode = 2;
	else if (Mode == 7) Mode = 3;

	char result = 0;
	uint8_t check = 0;

	check = M128_UARTM_rec( Mode, UID, RAdd, 1, &result);
	if (check) return 5;
	result = (result & (~Mask)) | ((Data>>Shift) & Mask);	//change value by mask shift
	check = M128_UARTM_trm( Mode, UID, RAdd, 1, &Data);
	if (check) return 5;

	return 0;
}

char M128_UARTM_frc(char Mode, char UID, char RAdd, char Mask,  char Shift, char* Data_p)
{
	if(Mode<0 || Mode>8)return 1;
	if(UID<0)			return 2;
	if(RAdd<0)			return 3;

	if (Mode == 4) Mode = 0;
	else if (Mode == 5) Mode = 1;
	else if (Mode == 6) Mode = 2;
	else if (Mode == 7) Mode = 3;

	char result = 0;
	uint8_t check = 0;

	check = M128_UARTM_rec( Mode, UID, RAdd, 1, &result);
	if (check) return 5;
	result = (result & Mask) >> Shift;	//change value by mask shift
	*Data_p = result;	//return the data from slave and give it to "char* Data"

	return 0;
}

char M128_UARTTX_isr(char Number, void (*function)(void))
{
	if(Number<0 || Number>Register_max)   return 1;
	if (M128_UART_isr.U_TX1[(int)Number] != 0) return 2;
	M128_UART_isr.U_TX1[(int)Number] = function;
	return 0;
}

char M128_UARTRX_isr(char Number, void (*function)(void))
{
	if(Number<0 || Number>Register_max)   return 1;
	if (M128_UART_isr.U_RX1[(int)Number] != 0) return 2;
	M128_UART_isr.U_RX1[(int)Number] = function;
	return 0;
}

ISR(USART1_TX_vect)
{
	for(int i=0;i<Register_max;i++)
	{
		if(M128_UART_isr.U_TX1[i] != 0)
		{
			M128_UART_isr.U_TX1[i]();
		}
	}
}

ISR(USART1_RX_vect)
{
	for(int i=0;i<Register_max;i++)
	{
		if(M128_UART_isr.U_TX1[i] != 0)
		{
			M128_UART_isr.U_TX1[i]();
		}
	}
}
