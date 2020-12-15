
/***********************************************************************
 * 
 * keyboard library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) David García Torres and Elena Arjona Bustos
 * BUT Digital Electronics 2
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/**
 * @file  keyboard.h
 * @brief keyboard library for AVR-GCC.
 *
 * @details
 * The library contains functions for controlling the keyboard from the door application
 *
 * @note
 * 
 * 
 * @copyright (c) David García Torres and Elena Arjona Bustos
 * BUT Digital Electronics 2
 * This work is licensed under the terms of the MIT license.
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gpio.h"
#include "timer.h"
#include "lcd.h"

#define COL0 PD0			// AVR pin where COLUM 0 from keyboard is connected
#define COL1 PD1			// AVR pin where COLUM 1 from keyboard is connected
#define COL2 PD2			// AVR pin where COLUM 2 from keyboard is connected

#define ROW1 PC1			// AVR pin where ROW 1 from keyboard is connected
#define ROW2 PC2			// AVR pin where ROW 2 from keyboard is connected
#define ROW3 PC3			// AVR pin where ROW 3 from keyboard is connected
#define ROW4 PC4			// AVR pin where ROW 4 from keyboard is connected

#define LED_GREEN   PB4     // AVR pin where green LED is connected
#define LED_RED     PB3     // AVR pin where red LED is connected

#define RELE PC0			// AVR pin where relay is connected


/**
 * @brief Scan the key that is press and count for the door code reset
 * @param row1,row2,row3,row4 the inputs for each row of the keyboard
 * @return integer that shows the state of the door
 */
int scan(int row1, int row2, int row3, int row4);

/**
 * @brief Join the digits, and verify the password
 * @param digit the number pressed in the keyboard
 * @param ot door and password timers
 * @return integer that shows the state of the door
 */

int check(int digit,int ot);
	
#endif /* KEYBOARD_H_ */