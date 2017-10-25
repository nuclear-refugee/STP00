
//--------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include ".\LIB\rev_STDIO.h"
/*
//--------------Motor phase
#define Phase1
#define Phase2
#define Phase3
#define Phase4
//--------------SPI
#define SS
#define MOSI
#define MISO
#define SCK
*/


int abs(int n)
{
    if(n < 0)
        return -n;
    else
        return n;
}

volatile int speed_pre=0,phase_speed_pre=0,phase=0,count0=0,count1=0,step = 0;
volatile int phase_switch = 0;
const uint8_t output[8] = {0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09};


int sign(int n)
{
    if(n & 0x8000)
        return -1;
    else
        return 1;
}

ISR(TIMER0_COMP_vect)
{
    if(speed_pre || (phase_speed_pre && phase))
    {
        if(speed_pre)
        {
            count0++;
            if(abs(speed_pre) <= count0){
                step += sign(speed_pre);
                count0 = 0;
            }
        }
        if(phase && phase_speed_pre)
        {
            count1 ++;
            if(abs(phase_speed_pre) <= count1){
                step += sign(phase);
                phase -= sign(phase);
                count1 = 0;
            }
        }

        if(step > 0)
        {
            phase_switch++;
            step--;
        }
        else if(step < 0)
        {
            phase_switch--;
            step++;
        }

        if(phase_switch<0)
            phase_switch=3;
        else if(phase_switch >=8 )
            phase_switch=0;
        PORTC = output[phase_switch];
    }
    else
        PORTC = 0x00;
}

void Tim_set(void)
{
    TCCR0 = 0b00001100;   //prescalar 64
    OCR0 = 255;
    TIMSK = 0b00000010;
}

int main(void)
{
    unsigned int n;
    int num;
    rev_STDIO_set();
    Tim_set();
    DDRC = 0x0F;
    DDRA = 0xFF;
    sei();
    while(1)
    {

        /*
        PORTC = output[phase_switch];
        phase_switch ++;
        //printf("%u\n", PORTA);
        if(phase_switch >= 4)
            phase_switch = 0;
        _delay_ms(2);
        */

        printf("Wait for Set :\n1.speed prescalar\n2.phase correct speed\n3.add phase correct\n4.OCR\n");
        scanf("%u",&n);
        scanf("%d",&num);
        switch (n)
        {
            case 1:
                speed_pre = num;
                break;
            case 2:
                phase_speed_pre = num;
                break;
            case 3:
                phase = num;
                break;
            case 4:
                OCR0 = (uint8_t)(num);
                break;
        }

    }
}
