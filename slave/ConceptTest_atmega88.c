
//--------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <stdio.h>
//#include <util/delay.h>
//#include ".\LIB\rev_STDIO.h"
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
//--------------Variables for Control
#define ratio 10
//f_int = f_io / (2*64*(1+OCR))
// OCR  = f_io/f_int/2/64-1
#define operate_OCR 72

int abs(int n)
{
    if(n & 0x8000)
        return -n;
    else
        return n;
}

volatile int speed_pre=4,phase_speed_pre=1,phase=0,count0=0,count1=0,step = 0;
volatile int phase_switch = 0;
//const uint8_t output[4] = {0x0E,0x0D,0x0B,0x07};


int sign(int n)
{
    if(n & 0x8000)
        return -1;
    else
        return 1;
}


ISR(TIMER0_COMPA_vect)
{
    count0 ++,count1 ++;
    if(speed_pre && (abs(speed_pre) >= count0))
    {
        step += sign(speed_pre);
        count0 = 0;
    }
    if(phase && phase_speed_pre && (abs(phase_speed_pre) >= count1))
    {
        step += sign(phase);
        phase -= sign(phase);
        count1 = 0;
    }

    if(step != 0)
    {
        phase_switch += sign(phase_switch);
        step -= sign(phase_switch);
    }

    if(phase_switch<0)
        phase_switch=3;
    else if(phase_switch >=4 )
        phase_switch=0;

    switch(phase_switch)
    {
        case 0:
            PORTB = (1 << PB1);
            PORTD = (1 << PD6)|(1 << PD5);
            break;
        case 1:
            PORTB = (1 << PB2);
            PORTD = (1 << PD6)|(1 << PD5);
            break;
        case 2:
            PORTB = (1 << PB1)|(1 << PB2);
            PORTD = (1 << PD5);
            break;
        case 3:
            PORTB = (1 << PB1)|(1 << PB2);
            PORTD = (1 << PD6);
            break;
    }
}

void Tim_set(void)
{
    // CTC, prescalar 64, interrupt Enable
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01)|(1 << CS00);
    OCR0A  = operate_OCR;
    TIMSK0 = (1 << OCIE0A);
    // f_int = f_io / (2*pre*(1+OCR))
}

int main(void)
{
    //rev_STDIO_set();
    //Tim_set();
    DDRB = 0x06;
    DDRD = 0x6;
    sei();
    while(1)
    {
        /*
        printf("Wait for Set :\n1.speed prescalar\n2.phase correct speed\n3.add phase correct\n");
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
        }*/
    }
}
