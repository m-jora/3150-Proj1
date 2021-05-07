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
#define F_CPU 8000000UL
#include "util/delay.h"
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

// neopixel garbage
// initialize NeoPixels
void NeoPixel_Init(unsigned char red[], unsigned char green[], unsigned char blue[])
{
	DDRB |= (1<<0); // Set PB0 to an input
	PORTB &= 0; // Output 0x00 to PORTB

	updatePixel(red, green, blue); // Initial NeoPixel Colors

	return;
}

// update all RGB NeoPixel values
void updatePixel(unsigned char red[], unsigned char green[], unsigned char blue[])
{
	for(int i = 0; i < 10; i++) // Loop through for each NeoPixel
	{
		sendPixel(red[i], green[i], blue[i]); // Send a single 24 bit value for RGB
	}

	return;
}

// send RGB for a single NeoPixel
void sendPixel(unsigned char red, unsigned char green, unsigned char blue)
{
	for(int i = 7; i >= 0; i--) // Loop for each bit in the Green Byte
	{
		if(green & (1<<i)) // Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			short_Delay(0xA0); // Leave it high longer than low
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			short_Delay(0x10); // Leave it low longer than high
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Red Byte
	{
		if(red & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			short_Delay(0xA0); // Leave it high longer than low
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			short_Delay(0x10); // Leave it low longer than high
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Blue Byte
	{
		if(blue & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			short_Delay(0xA0); // Leave it high longer than low
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			short_Delay(0x10); // Leave it low longer than high
		}
	}

	return;
}

// clears all NeoPixels
void clearPixel(unsigned char *red, unsigned char *green, unsigned char *blue) // Pass by reference
{
	for(int i = 0; i < 10; i++) // Loop to clear each NeoPixel
	{
		red[i] = 0x00;
		green[i] = 0x00;
		blue[i] = 0x00;
	}

	return;
}