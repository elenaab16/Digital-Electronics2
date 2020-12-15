
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
 * @file  keyboard.c
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

#include "keyboard.h"

static int ovtime = 0;		//counter for introducing the password
static int timdoor = 0;		//counter for time that is open the door

// scan digit  

int scan(int row1, int row2, int row3, int row4){

	static int key;				//Key pressed
	static int puls = 0;		//Key state
	static int ppuls = 0;		//Previous key state
	static int state = 3;		//Inform about the keyboard state (writing, correct, wrong)

	static uint8_t columns[] = {COL0, COL1, COL2};
	static uint8_t i = 0;		//Number of time the key is pressed
	
	if(ovtime != 0){			//Key counter
		
		ovtime++;
	}
	
	if(timdoor != 0){			//Door counter
		
		timdoor++;
	}
	
	
	if((GPIO_read(&PINC,ROW1) == 0) && (GPIO_read(&PINC,ROW2) == 0) && (GPIO_read(&PINC,ROW3) == 0) && (GPIO_read(&PINC,ROW4) == 0)){
		
		GPIO_write_low(&DDRD,columns[i]);		//If there is no pressed key, we move the O to the right column
		
		
		if( i == 2){
			
			i = 0;
			
		}else{
				
			i++;
		}
		
		
		GPIO_write_high(&DDRD,columns[i]);
		
		puls = 0;		//No pressed key
		
		
	}else{
		
		puls = 1;		//Pressed key
		
		//Detecting the key pressed
		
		if((i == 0) && ((row1 ==1) && (row2 ==0) && (row3 ==0) && (row4 ==0))){
			
			key = 10; //*	
					
		}else if((i == 0) && ((row1 == 0) && (row2 == 1) && (row3 == 0) && (row4 == 0))){
			
			key = 7; //7		
					
		}else if((i == 0) && ((row1 == 0) && (row2 == 0) && (row3 == 1) && (row4 == 0))){
			
			key = 4; //4	
					
		}else if((i == 0) && ((row1 == 0) && (row2 == 0) && (row3 == 0) && (row4 == 1))){
			
			key = 1; //1
					
		}else if((i == 1)  && ((row1 == 1) && (row2 == 0) && (row3 == 0) && (row4 == 0))){
			
			key = 0; //0
					
		}else if((i == 1)  && ((row1 == 0) && (row2 == 1) && (row3 == 0) && (row4 == 0))){
			
			key = 8; //8
					
		}else if((i == 1)  && ((row1 == 0) && (row2 == 0) && (row3 == 1) && (row4 == 0))){
			
			key = 5; //5
					
		}else if((i == 1)  && ((row1 == 0) && (row2 == 0) && (row3 == 0) && (row4 == 1))){
			
			key = 2; //2
					
		}else if((i == 2)  && ((row1 == 1) && (row2 == 0) && (row3 == 0) && (row4 == 0))){
			
			key = 11; //#
					
		}else if((i == 2)  && ((row1 == 0) && (row2 == 1) && (row3 == 0) && (row4 == 0))){
			
			key = 9; //9
					
		}else if((i == 2)  && ((row1 == 0) && (row2 == 0) && (row3 == 1) && (row4 == 0))){
			
			key = 6; //6
					
		}else if((i == 2)  && ((row1 == 0) && (row2 == 0) && (row3 == 0) && (row4 == 1))){
			
			key = 3; //3
					
		}

		
	}
	
	
		  if( timdoor >= 91){			//It takes 3 second to close the door (3/33ms is aproximatly 91)
		 
				 state=check(key,1);	
				 timdoor = 0;
		 
		  }else{
			  
				if(ovtime >= 120){		//It takes 4 second to reset the password if there are not any more keys pressed (4/33ms is aproximatly 120)
				  
					state=check(key,1);
					ovtime = 0;
				  
				 }else{
				  
					if((ppuls == 0) && (puls == 1)){		//Detect if any key is pressed
					  
						state = check(key,0);
					  
					}
				  
				  
				 }
			  
			  
			}
		   
		   
			ppuls = puls;		//Save the previous value to know if it changes
			
			return state;
	
}

/* Checking Passwords*/
/*
*If ok=0 --> There is an error
*If ok=1 --> The code is right
*If ok=3 --> The code is still being introduced
*/

int check(int digit, int ot){ 
	
	static int i = 0;
	int j = 0;
	static int password = 0;
	static int correct[4] ={1234,0000,3939,1998};		//The right passwords 
	static int ok = 3;
	
	
	if(ot == 1){			//If door/password time expired, password is reseted
		
		i = 0;
		
		if(ok == 1){
			
			ok = 3;
		}else{
			
			ok = 0;
		}
				
		lcd_gotoxy(8,2);
	    lcd_puts("----");
		
		
		password = 0;
		
	}else{					//If time don not expire, evaluate the key pressed
		
		
		if(i != 4) {		//Until password is full
			
			ok = 3;
			
			if(i == 0){
				
				ovtime = 1; //The first digit in the password is introduced, the password count start
		    }
			
			
			if((digit >= 0) && (digit <= 9) ){
				
				password = password*10 + digit;
				lcd_gotoxy(i+8,2);
				lcd_puts("*");
				i++;
				
			 }
			
		}else{
			
			i = 0;
			ovtime = 0;
			
			lcd_gotoxy(8,2);
			lcd_puts("----");
			
			ok = 0;
			
			for(j=0;j<4;j++){
				
				if(password == correct[j]){			//Check the password with the correct codes
					
					ok = 1;
					timdoor = 1;					//Start the door counter
					
				}
		 }
		 
		 password = 0;
			
		
		
		
	   }
	
	}
		
	return ok;
}
	