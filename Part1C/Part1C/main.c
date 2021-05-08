/*
 * main.c
 *
 * Author: kwwest
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"

#define F_CPU 8000000UL
#define _NOP() __asm__ __volatile__("nop")

void clear_pixels();

void update_pixels();

void read_light();

void neopixel_init();

struct rgb // Red-Green-Blue struct
{
	unsigned char r = [0x30, 0x30, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20];
	unsigned char g = [0x00, 0x10, 0x20, 0x30, 0x30, 0x20, 0x10, 0x00, 0x00, 0x00];
	unsigned char b = [0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x30, 0x30, 0x20, 0x10];
};
struct rgb neo_arr;

int main(void)
{
	int test = 0x01>>4;
	neopixel_init(neo_arr.r, neo_arr.g, neo_arr.b);
	int light_level = 25;
	int temp = 50;

	while(1)
	{
		clear_pixels();
		// reset to debug values
		light_level = 25;
		temp = 50;
		light_level &= 0x3F; // Limit to 0-63
		int to_loop = (light_level/64);
		for (int i = 0; i < to_loop; i++) // Set brightness for first 9 neopixels based on R24
		{
			neo_arr.r[i] = 0x10;
			neo_arr.g[i] = 0x10;
			neo_arr.b[i] = 0x10;
		}
		if (temp < 32) {
			// All blue
			neo_arr.r[10] = 0;
			neo_arr.g[10] = 0;
			neo_arr.b[10] = 64;
		} else if (temp > 96) {
			// All red
			neo_arr.r[10] = 64;
			neo_arr.g[10] = 0;
			neo_arr.b[10] = 0;
		} else if (temp < 64) {
			// LT_64
			neo_arr.r[10] = 0;
			neo_arr.g[10] = (temp-32)*2;
			neo_arr.b[10] = (-temp+64)*2;
		} else {
			neo_arr.r[10] = (temp-64)*2;
			neo_arr.g[10] = (-temp+96)*2;
			neo_arr.b[10] = 0;
		}
		update_pixels();
	}
}

// initialize NeoPixels
void neopixel_init()
{
	DDRB |= (1<<0); // Set PB0 to an input
	PORTB &= 0; // Output 0x00 to PORTB

	update_pixels(); // Initial NeoPixel Colors

	return;
}

// update all RGB NeoPixel values
void update_pixels()
{
	for(int i = 0; i < 10; i++) // Loop through for each NeoPixel
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
			_delay_ms(160); // Leave it high longer
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_delay_ms(16); // Leave it low longer than high
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Red Byte
	{
		if(red & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			_delay_ms(160); // Leave it high longer
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_delay_ms(16); // Leave it low longer than high
		}
	}

	for(int i = 7; i >= 0; i--) // Loop for each bit in the Blue Byte
	{
		if(blue & (1<<i))// Send a 1
		{
			PORTB |= (1<<0); // Set PB0
			_delay_ms(160); // Leave it high longer
			PORTB &= 0; // Clear PB0
		}
		else // Send a 0
		{
			PORTB |= (1<<0); // Set PB0
			PORTB &= 0; // Clear PB0
			_delay_ms(16); // Leave it low longer than high
		}
	}

	return;
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