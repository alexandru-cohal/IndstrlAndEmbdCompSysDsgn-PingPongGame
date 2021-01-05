/*
 * adc.c
 *
 * Created: 02.11.2016 13:07:06
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include "adc.h"

void adc_setup()
{
	/* ADC0 setup */
	
	// MUX0, MUX1, MUX2, MUX3, MUX4, MUX5 = 0 (select ADC0 - Single Ended Input)
	ADCSRB &= ~(1 << MUX5);
	ADMUX &= ~( (1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
	ADMUX |=  (1 << ADLAR);
	ADCSRA |=   (1<<ADEN) | (1<<ADIE) | (1<<ADATE)|  (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	// Set PF0 as input (Analog pin 0)
	DDRF &= ~(1 << PF0); 
}

void adc_request()
{
	/* Request the converted values from the ADC */
	
	ADCSRA |= (1<<ADSC);
}


