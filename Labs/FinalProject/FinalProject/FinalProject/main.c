
/***********************************************************************
 * 
 * Control a door with a password
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) David García Torres and Elena Arjona Bustos
 * BUT Digital Electronics 2
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"			// Lcd library for AVR-GCC
#include "keyboard.h"		// Keyboard library for AVR-GCC
#include "uart.h"			// Uart library for AVR-GCC
#define F_CPU 16000000      // CPU frequency in Hz 


int state = 3;


void initialize(){
	
	//Keyboard columns
	GPIO_config_output(&DDRD, COL0);
	GPIO_config_output(&DDRD, COL1);
	GPIO_config_output(&DDRD, COL2);
	
	GPIO_write_high(&PORTD, COL0);
	GPIO_write_high(&PORTD, COL1);
	GPIO_write_high(&PORTD, COL2);
	
	GPIO_config_input_pullup(&DDRC, ROW1);
	GPIO_config_input_pullup(&DDRC, ROW2);
	GPIO_config_input_pullup(&DDRC, ROW3);
	GPIO_config_input_pullup(&DDRC, ROW4);
	
	//Timers
	TIM1_overflow_interrupt_enable();
	TIM1_overflow_33ms();

	//Initialize LCD display
	lcd_init(LCD_DISP_ON);
	 
	//LEDs
	GPIO_config_output(&DDRB, LED_GREEN);
	GPIO_write_low(&PORTB, LED_GREEN);
	 
	GPIO_config_output(&DDRB, LED_RED);
	GPIO_write_low(&PORTB, LED_RED);
	  
	//RELAY
	GPIO_config_output(&DDRC, RELE);
	GPIO_write_low(&PORTC, RELE);
	  
	//UART
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	
	//Enable interrupts
	sei();
}



int main(void){

	initialize();
	
	lcd_gotoxy(1, 0);
	lcd_puts("Enter password:");
	
	lcd_gotoxy(8,2);
	lcd_puts("----");
	
	uart_puts("Welcome. Press * to verify the code.\n\n");
	
    while (1){
		
		// Error signaling
		if(state == 1){			// Right code
			
			GPIO_write_high(&PORTB, LED_GREEN);
			GPIO_write_low(&PORTC, RELE);
			
			
		}else if(state == 0){	// Wrong code 
			
			GPIO_write_high(&PORTB, LED_RED);
			GPIO_write_high(&PORTC, RELE);
			
			
		}else{					// Introducing the code
			
			GPIO_write_low(&PORTB, LED_RED);
			GPIO_write_low(&PORTB, LED_GREEN);
			GPIO_write_high(&PORTC, RELE);
			
		}
		
    }
	
	
}

//Timer handler

ISR(TIMER1_OVF_vect){

	static char* open = "The door is open.\n\n";
	static char* close = "The door is close.\n\n";
	static char* code = "The code is being introduced.\n\n";
	static int p_state = 3;

	state = scan(GPIO_read(&PINC,ROW1), GPIO_read(&PINC,ROW2), GPIO_read(&PINC,ROW3), GPIO_read(&PINC,ROW4));
	
	if(p_state != state){			//If there is a change on state a message is sent to uart
		
		if(state == 0){
			
			uart_puts(close);
			
		}else if(state == 1){
			
			uart_puts(open);
			
		}else{
			
			uart_puts(code);
			
		}
		
	}
	
	p_state = state;
	
}
