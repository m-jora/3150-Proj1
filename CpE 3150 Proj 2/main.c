/*
 * main.c
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
 *
 *
 *
 */ 

#include <avr/io.h>

void beep();

void freq_delay();

int main(void)
{
	
    while(1)
	{
		//beep();
	}
}

void beep() {
	DDRC |= 0b01000000;
	
	}
	
	
void freq_delay(){
		TCNT0 = -63;
		TCCR0 = 0b00000011;
		
		TCCR0 = 0x00;
		
}
