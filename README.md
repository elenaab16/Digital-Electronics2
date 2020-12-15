# DOOR CONTROL ACCESS BY PASSWORD

## Team members

Elena Arjona Bustos

[Repository](https://github.com/elenaab16/Digital-Electronics2)

David Garcia Torre

[Repository](https://github.com/davidgarcia23/digital-electronics-2/edit/main/Labs/FinalProject)

## Project objectives
Our goal is to develope a door lock system with a 4x3 keyboard control, a relay, a display, an uart, leds controlled by atmega328p microcontroller.
The system we have designed has to open the relay that represent a door, when a right code is introduced and signal the operation.


There are four correct password, if case one of them is introduced, the door relay will be opened and the green led is turn on. In case a wrong code is introduced, the relay is going to remain closed and the red led will be turned on.


While the user is introducing the code the relay will remain always closed, if it takes more than four seconds to the user to write the code, it will be reseted.
When the code is right, and the door is opened, it takes three seconds to automaticly close the door. By the time the code is beeing introduced, is going to be shown the number of digits introduced on a LCD screen with an asterisk.


Every time the state of the door change, the system send a message by uart. It was considered using an audio output, but the system was more inconveninet than usefull.

![alt text](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/ed2.jpg "ED2")

## Hardware description

The atmega328p microcontroller process the signals that come from the keyboard as input, and elaborate an output that serves to modify the status of the relay and turn the LEDs on or off when necessary. The resistors are used to protect the LEDs, and the transistor and diode to implementate relay circuit.

The yellow led is turned on when the door is closed, green one when the password is right and red one when the password is wrong.


All imputs are initialized with an internal pull up resistor



![alt text](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/circuito.png "Circuit")

 - [ ] *HARDWARE*
 - MICRO atmega328p
 - LCD Hd44780-1
 - UART
 - Relay
  - LEDs
 - Transistor
 - Diode
 - Resistor
 - KeyPad
 - Fixed Voltage

## Code description and simulations

We have created keyboard librarie and used the other ones from other exercises (gpio,lcd,timer,uart)

### Main


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
			
			
		}else if(state == 0){		// Wrong code 
			
			GPIO_write_high(&PORTB, LED_RED);
			GPIO_write_high(&PORTC, RELE);
			
			
		}else{				// Introducing the code
			
			GPIO_write_low(&PORTB, LED_RED);
			GPIO_write_low(&PORTB, LED_GREEN);
			GPIO_write_high(&PORTC, RELE);
			
		}
		
    }
	
	
}

Everything is initialized calling "initialize" function. A message is sent to the uart to explain user how to introduce the code. After that, in a loop we always evaluate the state.
We use this state to open/close the relay, or to turn on/of the LEDs.




*Timer handler*

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

Every 33ms the timer handler scan the keyboard and receive the state. If it detects any changes on it, it is sent a message to the uart.


[FULL CODE](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/FinalProject/main.c)


### Keyboard


*Scan digit* 

int scan(int row1, int row2, int row3, int row4){

	static int key;			//Key pressed
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
	
	
		  if( timdoor >= 91){						//It takes 3 second to close the door (3/33ms is aproximatly 91)
		 
				 state=check(key,1);	
				 timdoor = 0;
		 
		  }else{
			  
				if(ovtime >= 120){				//It takes 4 second to reset the password if there are not any more keys pressed (4/33ms is aproximatly 120)
				  
					state=check(key,1);
					ovtime = 0;
				  
				 }else{
				  
					if((ppuls == 0) && (puls == 1)){	//Detect if any key is pressed
					  
						state = check(key,0);
					  
					}
				  
				  
				 }
			  
			  
			}
		   
		   
			ppuls = puls;						//Save the previous value to know if it changes
			
			return state;
	
}


In this function, the state of the rows from the keyboard are passed as inputs to the function. 


We compare these states and if they are equal to zero that means that none of the buttons are pressed. When this happen we move the 0 activating the next column and put a 1 in the actual column deactivating it. If there are any pressed buttons, we select the digit based on which row is activated. Also, we send to the function "check" with a 1 as an input if any timer has been finished , if not, it calls the function "check" with 0 as an input when it detects that there has been one key pressed.


The function return the state of the system, 0 for a wrong code, 1 for the right one and 3 for when the code is being introduced.




*Checking Passwords*


* If ok=0 --> There is an error

* If ok=1 --> The code is right

* If ok=3 --> The code is still being introduced




int check(int digit, int ot){ 
	
	static int i = 0;
	int j = 0;
	static int password = 0;
	static int correct[4] ={1234,0000,3939,1998};		//The right passwords 
	static int ok = 3;
	
	
	if(ot == 1){						//If door/password time expired, password is reseted
		
		i = 0;
		
		if(ok == 1){
			
			ok = 3;
		}else{
			
			ok = 0;
		}
				
		lcd_gotoxy(8,2);
	    lcd_puts("----");
		
		
		password = 0;
		
	}else{							//If time don not expire, evaluate the key pressed
		
		
		if(i != 4) {					//Until password is full
			
			ok = 3;
			
			if(i == 0){
				
				ovtime = 1;			 //The first digit in the password is introduced, the password count start
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
					timdoor = 1;				//Start the door counter
					
				}
		 }
		 
		 password = 0;
			
		
		
		
	   }
	
	}
		
	return ok;
}
	
	
There has been declared 4 correct codes. At the begining, we evaluate if the call of the function has specified that one timer has expired with the input "ot". If so, it detects which one of them, and send state "error" (ok = 0) if it has been the password time the one that has expired or to the state "writing" when it comes from the door time. After that, the password is reset.
	
	
If the input ot is equal to 0, that means that one key has been pressed. In case that is one of the 4 keys necessaries for the code, the function evaluate if it is the first pressed key, if it so, it start the password count. Besides that, it add the new digit to the password variable, moving the previous digit one place to the left. 
	
	
When it detects that the 4 digits of the password has been introduced, it compares the current password with the 4 correct codes, if any coincidences are detected, it returns the state correct (ok = 1), if not, it returns the state error (ok = 0).
	
	
[FULL CODE](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/FinalProject/keyboard.c)

## Video / Animation

### Right code

In this image are used three right codes,it is shown how is automatly in three seconds reset the relay, and for last, a wrong code.

![alt text](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/gif11.gif "Right")

### Wrong code

In this image it is shown one wrong code and for last, how to reset the code if is not completed in four seconds.

![alt text](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/gif2.gif "Wrong")

### Uart 

Here it is shown how uart works

![alt text](https://github.com/davidgarcia23/digital-electronics-2/blob/main/Labs/FinalProject/FinalProject/gifuart.gif "Uart")


## References

* [RELAY](https://docs.onion.io/omega2-docs/relay-expansion-c-library.html)
* [RELAY2](http://diwo.bq.com/utilizar-rele-arduino-zum-core/)
* [KEYPAD](https://learn.parallax.com/tutorials/language/propeller-c/propeller-c-simple-devices/read-4x4-matrix-keypad)
* All elearning resources
