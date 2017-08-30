
/*
#include "../ASALIB/ASA_DEVICE.h"
#include "STP00.h"


int main() {
    char phase[4] = {15-(1<<PHA),15-(1<<PHB),15-(1<<PHC),15-(1<<PHD)};
    uint8_t i=0;
    PH_DDR = 15;
    while (1) {
        PH_PORT = phase[i];
        i = (i+1)%4;
        _delay_ms(10);
    }
    printf("kd\n" );


    return 0;
}
*/
//void uart_init(/* arguments */) {}

//--------------------------------------------------------
//幹你娘//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
//--------------Motor phase
#define Phase1 PA0
#define Phase2 PA1
#define Phase3 PA2
#define Phase4 PA3
//--------------SPI
#define SS   PB2
#define MOSI PB3
#define MISO PB4
#define SCK  PB5
//-------------SPI_buffer
char Data_Buffer[3]={0};
//--------------SPI_INI
void spi_ini(void) {

printf("SPI_INI\n");
      DDRB|=(0<<MOSI)|(1<<MISO)|(0<<SCK)|(0<<SS);   //set MISO
    //  PORTB|=(1<<MISO);                             //MISO Output ,others input
      SPCR=(1<<SPIE)|(1<<SPE)|(0<<MSTR);			//Enable
printf("End\n");

}
//--------------SPI_DATA_SWAP
char data_swap(char Tdata) {

	 char timeout=0;
	 char Rdata=0;
	 static char count=0;
	 SPDR=Tdata;
	 while(!(SPSR&(1<<SPIF))){}
	 Rdata=SPDR;
	 count=count%3;
	 Data_Buffer[count]=Rdata;
	 count++;
	 return 0;

}
//-------------timer
void timer_set(void) {

	TCCR0A = (1<<WGM01);
  TCCR0B = (1<<CS01);
	TIMSK0 = 1<<OCIE0A;
	OCR0A  = 255;

}
//-------------main

int main (void) {
	spi_ini();
	timer_set;
	sei();
  char i=0;

	while(1) {

		switch (Data_Buffer[i]) {

			case 1: printf("1\n" );
            break;
      case 2: printf("2\n", );
            break;
      case 3: printf("3\n", );
            break;
      default: break;


		}


	}







}



ISR(TIMER0_COMP) {

	data_swap(1);

}
