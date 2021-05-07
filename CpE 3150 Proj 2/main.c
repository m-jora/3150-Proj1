/*
 * main.
 *
 * Created: 4/30/2021 4:36:49 PM
 *  Author: hheselbarth1
 * 
 * Left button for start/stop
 * count up to 2s max
 * play beep when start/stop button pushed
 * each timer tick is .1s
 * increase brightness with each tick 
 * set initial brightness value
 * if timer not stopped before 2s strobe neopixels with short beep
 * Problem 5 from hw 5 for .1s delay with 2s total
 *
 *
 */ 

// TCNT0 is 8 bit
// TCNT1 & TCNT3 is 16bit
// TCNT4 is 10 bit

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

#define _NOP() __asm__ __volatile__("nop")
//#include "Adafruit_NeoPixel.h"

void beep();

void freq_delay();

//void setpixcolor(int n, int r, int g, int b);

int main(void)
{
    while(1)
    {
        TCNT1H = -0xF4;
        TCNT1L = 0x24;
        TCCR1B = 0b00000100;
        
        while(!(TIFR1 & (1<<TOV1)))
        {
            TCNT3H = -0x30;
            TCNT3L = 0xD4;
            TCCR3B = 0b00000011;
            while (!(TIFR3 & (1<<TOV3)))
            {
            
                if(PIND & (1<<4)) // button get pressed
                {
                    int hold_low = TCNT1L;
                    int hold_high = TCNT1H;
                    beep(); // beep time
                    while(PIND & (1<<4)); // waiting for button depress
                    
                    while (!(PIND & (1<<4)))
                    {
						TCNT1H = hold_high;
						TCNT1L = hold_low;
						TIFR1 = 1<<TOV1;
                    }
					beep();
					while(PIND & (1<<4));
                }
        
            }
            TCCR3B = 0x00;
            TIFR3 = 1<<TOV3;
        }
        TCCR1B = 0x00;
        TIFR1 = 1<<TOV1;

        beep(); // makes speaker go brrrrrrrrr
    }
}

/*void setpixcolor(int n, int r, int g, int b)
{
    
}*/

void beep() {
    DDRC |= 0b01000000;
    TCNT0 = -0xFF;
    TCCR0B = 0b00000101;
    
    while(!(TIFR0 & (1<<TOV0))){
        _delay_ms(1);
        PORTC ^= 0b01000000;    
    }
    TCCR0B = 0x00;
    TIFR0 = 1<<TOV0;
}
