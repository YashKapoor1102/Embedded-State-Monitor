/*

Name: Yash Kapoor

Student ID: 101163338

SYSC3310A Lab 6: Simple PC Application

*/

#include "msp.h"
#include <stdint.h>

// NOTE: C99 Mode must be enabled

static uint8_t index = 0;


/*
Configuring UART0 and its interrupts
*/
void UART0_init(void) {
	
	// Unlock CS Module for register access
	CS->KEY = CS_KEY_VAL;
	
	// Putting UART0 in reset mode for configuration
	EUSCI_A0->CTLW0 |= (uint16_t) (1 << 0);
	
	// Disabling parity and setting number of stop bits to 1
	// Selecting the SMCLK Clock Source
	EUSCI_A0->CTLW0 = (uint16_t) 0x0081;
	
	// Initializing EUSCI
	EUSCI_A0->CTLW0 &= ~(EUSCI_A_CTLW0_SWRST); 
	
	// Setting the deglitch time to 30 ns
	EUSCI_A0->CTLW1 = (uint16_t) 0x0002;
	
	// Disabling oversampling 
	EUSCI_A0->MCTLW = (uint16_t) 0x0000;
	
	// Clock prescaler setting of the baud-rate generator
	// (3,000,000 MHz / 115200 bps = 26 = 0x1A)
	EUSCI_A0->BRW = (uint16_t) 0x1A;
	
	// Clearing the receive interrupt flag
	EUSCI_A0->IFG &= (uint16_t) ~(1 << 0);
	
	// Enabling the receive interrupt
	EUSCI_A0->IE |= (uint16_t)(1 << 0);
	
	// Setting PxSEL0 and Clearing PxSEL1 to select UART0
	P1->SEL0 |= (uint16_t)((1 << 2) | (1 << 3));
	P1->SEL1 &= (uint16_t)(~((1 << 2) | (1 << 3)));
		
	// Configuring NVIC for UART0
	NVIC_SetPriority(EUSCIA0_IRQn, 2);
	NVIC_ClearPendingIRQ(EUSCIA0_IRQn);
	NVIC_EnableIRQ(EUSCIA0_IRQn);
	
	// Taking UART0 out of reset mode 
	// since configuration is done
	EUSCI_A0->CTLW0 &= (uint16_t)~(0x0001);
	
	CS->KEY = 0;		// Lock CS Module
}

int main() {
	
	// Disabling watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;
	
	// Configuring the LEDs and switches below
	
	// Setting PxSEL0 and PxSEL1 to 0 to select GPIO
	// Setting bits 0, 1, and 4 to 0 in port 1
	P1->SEL0 &= (uint8_t)(~((1 << 4) | (1 << 1) | (1 << 0)));
	P1->SEL1 &= (uint8_t)(~((1 << 4) | (1 << 1) | (1 << 0)));
	
	// Setting bits 0, 1, and 2 to 0 in port 2 
	P2->SEL0 &= (uint8_t)(~((1 << 2) | (1 << 1) | (1 << 0)));
	P2->SEL1 &= (uint8_t)(~((1 << 2) | (1 << 1) | (1 << 0)));
	
	// Regular Drive Strength
	P1->DS &= (uint8_t)~(1 << 0);
	P2->DS &= (uint8_t)~(((1 << 2) | (1 << 1) | (1 << 0)));
	
	// Indicating which pins we are using for input/output
	// LEDs are an output and the buttons are input 
	P1->DIR &= (uint8_t)(~((1 << 4) | (1 << 1)));			// input (both buttons)
	
	P1->DIR |= (uint8_t)(1 << 0);					// output (RED LED)
	P2->DIR |= (uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));		// output (RGB LED)
	
	// Configuring the switches (P1.1 and P1.4) as inputs
	// using pull-up internal resistors
	P1->OUT |= (uint8_t)((1 << 4) | (1 << 1));
	P1->REN |= (uint8_t)((1 << 4) | (1 << 1));
	
	// Clearing the bits for active high
	// Initializing LED states (all turned off)
	P1->OUT &= ~(uint8_t)(1 << 0);		// clearing bit 0 
	P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));		// clearing bits 0, 1, and 2
	// Configuring switch interrupts below
	
	// Disabling the interrupts for the RED LED and RGB LED
	P2->IE &= (uint8_t)(~((1 << 2) | (1 << 1) | (1 << 0)));
	P1->IE &= (uint8_t)(~(1 << 0));
	
	// Configuring device interrupts for P1.1 and P1.4
	P1->IES |= (uint8_t) 0x12;
	P1->IFG &= (uint8_t) ~0x12;
	P1->IE |= (uint8_t) 0x12;
	
	// Configuring NVIC for the switches
	NVIC_SetPriority(PORT1_IRQn, 2);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);

	// Enabling global interrupts in CPU
	__asm("CPSIE I");
	
	// Configuring UART0 by calling the function 
	UART0_init();

	// infinite loop 
	while (1) {
		
			__asm("WFI");
		
	}

}

/*
Sends a character via the serial port
to the computer.
*/
void printChar(char c) 	{
	while(!(EUSCI_A0->IFG & (uint16_t) 0x02));
	// ensuring any previous transmission has ended
	
	EUSCI_A0->TXBUF = c;
}

	
// P1.1 and P1.4 ISR
void PORT1_IRQHandler(void) {
	
	uint8_t cycle[4] = {00, 01, 10, 11};
	// initializing an array of 4 elements to keep track of
	// which state we are in
	
	if ((P1IFG & (uint8_t)(1 << 1)) && (P1IFG & (uint8_t)(1 << 4)))	{
		P1IFG &= (uint8_t)(~((1 << 1) | (1 << 4)));
	}
	else 	{
		
		if (P1->IFG & (uint8_t)(1 << 1)) {
			// Test if pin 1 caused the interrupt
			
			uint16_t i = 10000;
		
			// Debouncing using a delay loop
			while (i--) {
				
				// Convince the compiler that this loop contains important code
				__asm volatile ("");
			
			}
		
			P1->IFG &= ~(uint8_t)(1 << 1);
			// Clear flag if port 1, bit 1 (button 1) caused an interrupt
		
			// when button 1 is pressed
			// transition to the next state (move up the array) 
			
			index = (index + 1) % 4;
			// variable index increases when button 1 is pressed
			
			switch(cycle[index]) {
				
				case 11: 			// Both LEDs, RED (first number) and RGB (second number), are ON (11 state)
					
					printChar('3');
					// Transmitting the current state to the serial port, so it can be read 
					// by the monitoring app
				
					P1->OUT |= (uint8_t)(1 << 0);
					P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));
				
					break;
				
				case 01:			// Only RGB LED is ON	(01 state)
					
					printChar('1');
				
					P1->OUT &= ~(uint8_t)(1 << 0);
					P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));
				
					break;
				
				case 10:			// Only RED LED is ON (10 state)
					
					printChar('2');
				
					P1->OUT |= (uint8_t)(1 << 0);
					P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));
				
					break;
				
				default:			// Both LEDs are OFF (00 state)
					
					printChar('0');
				
					P1->OUT &= ~(uint8_t)(1 << 0);
					P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));
			
					break;
				
		}
			
	}
		else {

			uint16_t i = 10000;
		
			// Debouncing using a delay loop
			while (i--) {
			
				// Convince the compiler that this loop contains important code
				__asm volatile ("");
			
			}

			// Test if pin 4 caused the interrupt 
		
			P1->IFG &= ~(uint8_t)(1 << 4);
			// Clear flag if port 1, bit 4 (button 2) caused an interrupt
		
			// when button 2 is pressed
			// transition to the previous state (move down the array) 
				
			index = (index + 0xFF) % 4;
			// Variable index decreases when button 2 is pressed
			
			switch(cycle[index]) {
				
				case 11: 			// Both LEDs, RED (first number) and RGB (second number), are ON (11 state)
					
					printChar('3');
					// Transmitting the current state to the serial port, so it can be read 
					// by the monitoring app
				
					P1->OUT |= (uint8_t)(1 << 0);
					P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));
				
					break;
				
				case 01:			// Only RGB LED is ON	(01 state)
					
					printChar('1');
				
					P1->OUT &= ~(uint8_t)(1 << 0);
					P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));
				
					break;
				
				case 10:			// Only RED LED is ON (10 state)
					
					printChar('2');
				
					P1->OUT |= (uint8_t)(1 << 0);
					P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));
				
					break;
				
				default:			// Both LEDs are OFF (00 state)
						
					printChar('0');
				
					P1->OUT &= ~(uint8_t)(1 << 0);
					P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));
				
					break;
				
				}
			
		}
		

	}
}
	
		


/*
Receives a character via the serial port
from the computer and returns that character.
*/
char readChar(void) 	{
	char c;
	c = EUSCI_A0->RXBUF;
	
	return c;
}


/*

UART0 RX ISR: used to receive messages from the monitoring app
via the serial port (RS232) over the USB connection

*/
void EUSCIA0_IRQHandler(void) {
		
	if (EUSCI_A0->IFG & (uint16_t) 0x01) {
		
		// Clearing the receive interrupt flag 
		EUSCI_A0->IFG &= (uint16_t) (~(1 << 0));
		
		char userInput = readChar();
		// Reading the data received from the monitoring app via the serial port

		switch(userInput) {
				
			case '3': 			// Both LEDs, RED (first number) and RGB (second number), are ON (11 state)
				// User wants to go to 11 state
			
				P1->OUT |= (uint8_t)(1 << 0);
				P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));
				
				index = 3;
				break;
				
			case '2':			// Only RED LED is ON (10 state) 
				// User wants to go to 10 state
			
				P1->OUT |= (uint8_t)(1 << 0);
				P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));

				index = 2;
				break;
				
			case '1':			// Only RGB LED is ON	(01 state)
				// User wants to go to 01 state
			
				P1->OUT &= ~(uint8_t)(1 << 0);
				P2->OUT |= (uint8_t)((1 << 2) | (1 << 1) | (1 << 0));

				index = 1;
				break;
				
			default:			// Both LEDs are OFF (00 state) 
				// Default case: if user types anything other than 3, 2, or 1 in the monitoring app.
				// User wants to go to 00 state
			
				P1->OUT &= ~(uint8_t)(1 << 0);
				P2->OUT &= ~(uint8_t)(((1 << 2) | (1 << 1) | (1 << 0)));

				index = 0;
				break;
		}
	}	
}
