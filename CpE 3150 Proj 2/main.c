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
#define F_CPU 8000000UL
#include "util/delay.h"
//#include "Adafruit_NeoPixel.h"

void beep();

void freq_delay();

struct rgb
{
	unsigned red;
	unsigned green;
	unsigned blue;
};


void set_all_pixels(unsigned, unsigned, unsigned);
void enable_pixels();
void delay_15ms_ctc();

struct rgb neopixel_arr[10];
//void setpixcolor(int n, int r, int g, int b);

int main(void)
{
	DDRB |= (1 << 0); // sets portB.0 to output
	set_all_pixels(0,0,0); // pixels set to 0 (off)
	enable_pixels();	// zero value output
	int brightness = 0;
	while(1)
    {
        TCNT1H = -0xF4;
        TCNT1L = 0x24;
        TCCR1B = 0b00000100;
		brightness = 0;
        while(!(TIFR1 & (1<<TOV1))) //2 second timer
        {
			//if(brightness >255)
				//brightness = 0;
            TCNT3H = -0x30;
            TCNT3L = 0xD4;
            TCCR3B = 0b00000011;
            while (!(TIFR3 & (1<<TOV3))) //.1 second timer
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
			set_all_pixels(brightness,0,0);
			enable_pixels();
			brightness += 13;
            TCCR3B = 0x00;
            TIFR3 = 1<<TOV3;
        }
        TCCR1B = 0x00;
        TIFR1 = 1<<TOV1;

        beep(); // makes speaker go brrrrrrrrr
		for(int i = 0; i<10; i ++) // strobe
		{
			set_all_pixels(0,0,0);
			enable_pixels();
			delay_15ms_ctc();
			set_all_pixels(255,0,0);
			enable_pixels();	
			delay_15ms_ctc();
		}
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

// neopixel garbage
static inline void send_bit(unsigned bit) // function that handles bit sending
{
	int is_on = bit & 1;
	if (is_on)
	{
		PORTB |= 0b00000001;
		_NOP();
		_NOP();
		_NOP();
		PORTB &= 0b11111110;
	}
	else
	{
		PORTB |= 0b00000001;
		PORTB &= 0b11111110;
		_NOP();
		_NOP();
		_NOP();
	}
	return;
}


void set_all_pixels(unsigned red, unsigned green, unsigned blue) // pixel setter
{
	for (int i = 0; i < 10; i++)
	{
		neopixel_arr[i].red = red;
		neopixel_arr[i].green = green;
		neopixel_arr[i].blue = blue;
	}
	return;
}

void enable_pixels() // sends bit values to the NeoPixels
{
	for (int i = 0; i < 10; i++)
	{
		for (int b = 7; b >= 0; b--)
		{
			send_bit(neopixel_arr[i].green >> b);
		}
		for (int b = 7; b >= 0; b--)
		{
			send_bit(neopixel_arr[i].red >> b);
		}
		for (int b = 7; b >= 0; b--)
		{
			send_bit(neopixel_arr[i].blue >> b);
		}
	}
	return;
}


void delay_15ms_ctc()
{
	OCR0B = 0x75;
	TCNT0 = 0x00;
	TCCR0B = (TCCR0B & 0x00) | (1<<CS02) | (1<<CS00);
	while((TIFR0 & (1<<OCF0B)) == 0);
	TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
	TIFR0 |= 1<<OCF1A;
	TCNT0 = 0;
}
