/*
 * main.c
 *
 * Author: kwest
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include "util/delay.h"


#define _NOP() __asm__ __volatile__("nop")

void clear_pixels();

void update_pixels();

void read_light();

void neopixel_init();

void send_pixel(unsigned char red, unsigned char green, unsigned char blue);

struct rgb // Red-Green-Blue struct
{
	unsigned char r[10];
	unsigned char g[10];
	unsigned char b[10];
};
struct rgb neo_arr = {
	{0x30, 0x30, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20},
	{0x00, 0x10, 0x20, 0x30, 0x30, 0x20, 0x10, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x30, 0x20, 0x10}
};

int main(void)
{
	neopixel_init();
	int light_level = 25;
	int temp = 0;

	while(1)
	{
		// clear_pixels();
		// reset to debug values
		light_level &= 0x3F; // Limit to 0-63
		int to_loop = ((float)(light_level/64))*100;
		to_loop = 5;
		for (int i = 0; i < to_loop; i++) // Set brightness for first 9 neopixels based on light_lvl
		{
			neo_arr.r[i] = 0x00;
			neo_arr.g[i] = 0x30;
			neo_arr.b[i] = 0x00;
		}
		if (temp < 32) {
			// All blue (...-31)
			neo_arr.r[0] = 0;
			neo_arr.g[0] = 0;
			neo_arr.b[0] = 0x30;
		} else if (temp > 96) { // (97-...)
			// All red
			neo_arr.r[0] = 0x30;
			neo_arr.g[0] = 0;
			neo_arr.b[0] = 0;
		} else if (temp < 64) {  // LT_64   (32-63)
			neo_arr.r[0] = 0;
			neo_arr.g[0] = (temp-32)*2;
			neo_arr.b[0] = (-temp+64)*2;
		} else { // GT_64 (64-96)
			neo_arr.r[0] = (temp-64)*2;
			neo_arr.g[0] = (-temp+96)*2; 
			neo_arr.b[0] = 0;
		}
		update_pixels();
	}
}

// initialize NeoPixels
void neopixel_init()
{
	DDRB |= 0x01; // Set PB0 to an input
	PORTB &= 0; // Output 0x00 to PORTB
	update_pixels(); // Initial NeoPixel Colors

	return;
}

// update all RGB NeoPixel values
void update_pixels()
{
	for (int i = 0; i < 9; i++) // Loop through for each NeoPixel
	{
		send_pixel(neo_arr.r[i], neo_arr.g[i], neo_arr.b[i]); // Send a single 24 bit value for RGB
	}

	return;
}

// send RGB for a single NeoPixel
void send_pixel(unsigned char red, unsigned char green, unsigned char blue)
{
	for(int i = 7; i >= 0; i--) // Loop for each bit in the Green Byte
	{
		if(green & (1<<i)) // Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			_NOP();
			_NOP();
			_NOP();
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_NOP();
			_NOP();
			_NOP();
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Red Byte
	{
		if(red & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			_NOP();
			_NOP();
			_NOP();
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_NOP();
			_NOP();
			_NOP();
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Blue Byte
	{
		if(blue & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			_NOP();
			_NOP();
			_NOP();
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_NOP();
			_NOP();
			_NOP();
		}
	}

}

// clears all NeoPixels
void clear_pixels()
{
	for(int i = 0; i < 10; i++) // Loop to clear each NeoPixel
	{
		neo_arr.r[i] = 0x00;
		neo_arr.g[i] = 0x00;
		neo_arr.b[i] = 0x00;
	}

	return;
}