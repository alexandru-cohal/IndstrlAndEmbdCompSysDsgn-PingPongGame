/*
 * system.c
 *
 * Created: 23.09.2016 17:10:30
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "playground.h"
#include "system.h"
#include "joystick.h"
#include "oled.h"

/* basic system setup */
void system_init() {
	/* Basic system initialization */
	
	//Global disable of interrupts
	cli();
	
	/* Releasing PC7-PC4 from address to JTAG use */
	SFIOR |= 1 << XMM2;
	SFIOR &= ~(1 << XMM1);
	SFIOR &= ~(1 << XMM0);

	//Setting Pins as Input or Output
	DDRA = 0xFF;
	DDRC = 0xFF;

	// Setting PE1 (ALE) as output
	DDRE |=  0x02;
	//Setting PE0 (INT2) as input 
	DDRE &= ~(1<<PE0);
	//PD2 as Input (Joystick Button)
	DDRD &= ~(1 << PD2);
	//PD3 as Input (INT1)
	DDRD &= ~(1<<PD3);
	
	PORTA = 0x00;
	PORTE |= 0x02; // Latch enable

	/* Enable external memory interface */
	MCUCR |= (1 << SRE);
	
	// INT2 activated by rising edge
	EMCUCR |= (1<<ISC2);
	
	//Configuring the falling edge of INT1 to generate asynchronously an interrupt request
	MCUCR |= 1 << ISC11;
	MCUCR &= ~(1<<ISC10);
	//External INT1 request and INT2 request enable
	GICR |= (1<<INT1) | (1<<INT2);
	
	// Timer 0 setup (counts 1 second; used for the game)
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 4799;
	// The timer is started (interrupt is enabled) and stopped using start_timer_1second() and stop_timer_1second() functions
	
	//Global enable of interrupts
	sei();
}

void start_timer_1second()
{
	/* Start the 1 second timer (Enable the interrupt) */
	
	TIMSK |= (1 << OCIE1A);
}

void stop_timer_1second()
{
	/* Stop the 1 second timer (Disable the interrupt) */
	
	TIMSK &= ~(1 << OCIE1A);
}

void system_setup() {
	/* Basic system setup */
	
	printf("\nHi, I am ATmega162 at Beardboard.\n\n");
	
	//joystick_calibration();
	
	playground_init();
}



