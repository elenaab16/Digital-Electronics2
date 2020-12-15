/***********************************************************************
 * 
 * Control LEDs Knight Rider
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_RED1   PC0     // AVR pin where red LED 1 is connected
#define LED_RED2   PC1     // AVR pin where red LED 2 is connected
#define LED_RED3   PC2     // AVR pin where red LED 3 is connected
#define LED_RED4   PC3     // AVR pin where red LED 4 is connected
#define LED_RED5   PC4     // AVR pin where red LED 5 is connected
#define BUTTON   PD0     // AVR pin where the button is connected

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle three LEDs
 * on Multi-function shield with internal 8- and 16-bit timer modules.
 */
int main(void)
{
    /* Configuration of  LEDs */
	GPIO_config_output(&DDRC, LED_RED1);
	GPIO_write_low(&DDRC, LED_RED1);
	
    GPIO_config_output(&DDRC, LED_RED2);
    GPIO_write_low(&DDRC, LED_RED2);
	
    GPIO_config_output(&DDRC, LED_RED3);
    GPIO_write_low(&DDRC, LED_RED3);
	
	GPIO_config_output(&DDRC, LED_RED4);
	GPIO_write_low(&DDRC, LED_RED4);
	
	GPIO_config_output(&DDRC, LED_RED5);
	GPIO_write_low(&DDRC, LED_RED5);	

    /* Configuration of 8-bit Timer/Counter0 */
    //TIM0_overflow_1s();
	TIM0_overflow_interrupt_enable();

    /* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

    /* Configuration of 8-bit Timer/Counter2 */
	//TIM2_overflow_33ms();
	TIM2_overflow_interrupt_enable();
   

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
       if(bit_is_clear(PIND,BUTTON)){
		   
		   TIM1_overflow_262ms();
		   
	   }else{
		   
		   TIM1_overflow_1s();
	   }
	   
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Toggle LED D2 on 
 * Multi-function shield. */
ISR(TIMER1_OVF_vect)
{
    uint8_t r = 0;
	uint8_t i = 0;
	
	uint8_t leds[] ={LED_RED1,LED_RED2,LED_RED3,LED_RED4,LED_RED5};
		
	if(i == 4){
			
		r = 1;
		GPIO_write_high(&DDRC,leds[4]);
			
	}else if(i == 0){
			
		r = 0;
		GPIO_write_high(&DDRC,leds[4]);
			
	}
	
	 if(r == 0){
		 
		 i++;
		 
	 }else{
		 
		 i--;
		 
	 }
		
	 GPIO_write_low(&DDRC,leds[i]);
	
}