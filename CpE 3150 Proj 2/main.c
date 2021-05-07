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

#include <avr/io.h>
#define _NOP() __asm__ __volatile__("nop")
//#include "Adafruit_NeoPixel.h"

void beep();

void freq_delay();

//void setpixcolor(int n, int r, int g, int b);

int main(void)
{
    while(1)
	{
		while ()
		{
		
			if(PIND & (1<<4)) // button get pressed
			{
				beep(); // beep time
				while(PIND & (1<<4)); // waiting for button depress
			}
		
			//beep(); makes the speaker go brrrr
		}
	}
}

/*void setpixcolor(int n, int r, int g, int b)
{
	
}*/

void beep() {
	DDRC |= 0b01000000;
	TCNT1H = -0x61;
	TCNT1L = 0xBE;
	TCCR1B = 0b00000011; 
	while(!(TIFR1 & (1<<TOV1))){
		freq_delay();
		PORTC ^= 0b01000000;	
	}
	TCCR1B = 0x00;
	TIFR1 = 1<<TOV1;
}

	
void freq_delay(){
		TCNT0 = -63;
		TCCR0B = 0b00000011;
		while(!(TIFR0 & (1<<TOV0)));
		TCCR0B = 0x00;
		TIFR0 = 1<<TOV0;
}

