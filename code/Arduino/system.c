/*
 * system.c
 *
 * Created: 22.10.2016 14:33:11
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "system.h"
#include "adc.h"

void system_init() {
	/* Basic system initialization */
	
	// Disable interrupts
	cli();
	
	// PB0 is the SS pin from the SPI interface (should be output)
	DDRB |= 1 << PB0;
	// PE4 as input (INT4)
	DDRE &= ~(1<<PE4);
	// PJ1 as output (Digital pin 14 - Solenoid)
	DDRJ |= (1<<PJ1);
	// Initial value is High (Solenoid trigger is active low)
	PORTJ |= (1<<PJ1);
	
	// === Interrupt set up
	// Configuring the falling edge of INT4 to generate asynchronously an interrupt request
	EICRA |= 1 << ISC41;
	EICRA &= ~(1<<ISC40);
	
	// External INT4 request enable
	EIMSK |= 1 << INT4;
		
	// ADC intialization
	adc_setup();
	
	//Enabling global interrupt
	sei();
}

void system_setup() {
	/* Basic system setup */
	
	printf("\nHi, I am ATmega2560 at Arduino.\n\n");
}