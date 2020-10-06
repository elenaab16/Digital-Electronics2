/***********************************************************************
 * 
 * Knight Rider application.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Author: Elena Arjona Bustos
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_RED1   PC0     // AVR pin where red LED 1 is connected
#define LED_RED2   PC1     // AVR pin where red LED 2 is connected
#define LED_RED3   PC2     // AVR pin where red LED 3 is connected
#define LED_RED4   PC3     // AVR pin where red LED 4 is connected
#define LED_RED5   PC4     // AVR pin where red LED 5 is connected
#define BUTTON   PD0     // AVR pin where the button is connected

#define SHORT_DELAY 250      // Delay in milliseconds

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins.
 */

int leds[] ={LED_RED1,LED_RED2,LED_RED3,LED_RED4,LED_RED5};

int main(void)
{
	
	int i = 0;
	int r = 0;
	
	// Set pin as output in Data Direction Register
	// DDRC = DDRC or 0010 0000
	DDRC = DDRC | (1<<LED_RED1);
	DDRC = DDRC | (1<<LED_RED2);
	DDRC = DDRC | (1<<LED_RED3);
	DDRC = DDRC | (1<<LED_RED4);
	DDRC = DDRC | (1<<LED_RED5);
	

	// Set pin LOW in Data Register (LED off)
	// PORTC = PORTC and 1101 1111
	PORTC = PORTC | (1<<LED_RED1);
	PORTC = PORTC | (1<<LED_RED2);
	PORTC = PORTC | (1<<LED_RED3);
	PORTC = PORTC | (1<<LED_RED4);
	PORTC = PORTC | (1<<LED_RED5);
	
	/*PUSH BUTTON*/
	
	DDRD = DDRD & ~(1<<BUTTON);  // input
	PORTD = PORTD | (1<<BUTTON); // enable internal pull up 
	
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(SHORT_DELAY);
		
		PORTC = PORTC | (1<<leds[i]);	
		
		if(bit_is_clear(PIND,BUTTON)){
			
		
		   if(i == 4){	
				
			   r = 1;
			   PORTC = PORTC | (1<<leds[4]);	
			
		   }else if(i == 0){
			
			   r = 0;
			   PORTC = PORTC | (1<<leds[0]);
			
		   }
		
		
		   if(r == 0){
			
			   i++;
			
		   }else{
			
			   i--;
			
		   }
		
		
		PORTC = PORTC & ~(1<<leds[i]);	
			
		}

			
		}
 

    // Will never reach this
    return 0;
}

