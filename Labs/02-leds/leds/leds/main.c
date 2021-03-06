/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED   PC0     // AVR pin where red LED is connected
#define BUTTON   PD0     // AVR pin where the button is connected

#define SHORT_DELAY 500      // Delay in milliseconds
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle one LED 
 * and use function from the delay library.
 */
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);
	
	
	
	// Set pin as output in Data Direction Register
	// DDRC = DDRC or 0010 0000
	DDRC = DDRC | (1<<LED_RED);
	

	// Set pin LOW in Data Register (LED off)
	// PORTC = PORTC and 1101 1111
	PORTC = PORTC & ~ (1<<LED_RED);
	
	/*PUSH BUTTON*/
	
	DDRD = DDRD & ~(1<<BUTTON);  // input
	PORTD = PORTD | (1<<BUTTON); // enable internal pull up 
	
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);
		
		if(bit_is_clear(PIND,BUTTON)){
			
		// Invert LED in Data Register
		// PORTB = PORTB xor 0010 0000
		PORTB = PORTB ^ (1<<LED_GREEN);
		PORTC = PORTC ^ (1<<LED_RED);	
			
			
		}

        
    }

    // Will never reach this
    return 0;
}

