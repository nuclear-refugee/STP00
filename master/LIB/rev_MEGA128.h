#ifndef __REV_MEGA128_H__
#define __REV_MEGA128_H__

#define USART_baud 38400


//USART0
#define USART0_PORT     PORTE
#define USART0_PIN      PINE
#define USART0_DDR      DDRE
#define USART0_RXD      PE0
#define USART0_TXD      PE1
#define USART0_DATA     UDR0
#define USART0_CONTROL_STATUSA UCSR0A
#define USART0_CONTROL_STATUSB UCSR0B
#define USART0_CONTROL_STATUSB UCSR0C
#define USART0_BAUD_H    UBRR0H
#define USART0_BAUD_L    UBRR0L


//USART1
#define USART1_PORT     PORTD
#define USART1_PIN      PIND
#define USART1_DDR      DDRD
#define USART1_RXD      PD2
#define USART1_TXD      PD3
#define USART1_DATA     UDR1
#define USART1_CONTROL_STATUSA UCSR1A
#define USART1_CONTROL_STATUSB UCSR1B
#define USART1_CONTROL_STATUSB UCSR1C
#define USART1_BAUD_H    UBRR1H
#define USART1_BAUD_L    UBRR1L

//TWI
#define TWI_PORT	PORTD
#define TWI_PIN		PIND
#define TWI_DDR		DDRD
#define TWI_SCL		PD0
#define TWI_SDA		PD1

//SPI
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SPI_DDR		DDRB
#define SPI_MISO	PB3
#define SPI_MOSI	PB2
#define SPI_SCK		PB1
#define SPI_SS      PB0


//DIO
#define DIO_PORT	PORTF
#define DIO_PIN		PINF
#define DIO_DDR		DDRF
#define DIO_RW_PORT	PORTG
#define DIO_RW_PIN	PING
#define DIO_RW_DDR	DDRG
#define DIO0		PF4
#define DIO1		PF5
#define DIO2		PF6
#define DIO3		PF7
#define DIO_RD		PG3
#define DIO_WR		PG4

//ASA_ID
#define ADDR_PORT	PORTB
#define ADDR_PIN	PINB
#define ADDR_DDR	DDRB
#define ADDR0       PB5
#define ADDR1       PB6
#define ADDR2       PB7

//ASA_ID
#define ADDR_PORT_num 1
#define ADDR_PORT_msk (1<<5)|(1<<6)|(1<<7)
#define ADDR_PORT_sht 5
// SPI ADDR use PORTB bit 5,6,7 as id 0~7
// use DIO_put to set ADDR

#define DDR_SPI_num 1
#define DD_MISO DDB3
#define DD_MOSI DDB2
#define DD_SCK  DDB1
#define DD_SS   DDB0
// SPI use PORTB

#define CS_pin PF4
#define CS_PORT_NUM 5
#define CS_PORT_MSK (1<<CS_pin)
#define CS_PORT_SHT CS_pin
// CS : PF4

#endif
