/*
 * pwm.c
 *
 * Created: 27.10.2016 15:12:04
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include <avr/io.h>
#include <limits.h>

#include "pwm.h"

#define DUTY_CYCLE_MIN 1799
#define DUTY_CYCLE_MAX 4199
#define JOYSTICK_MIN SCHAR_MIN
#define JOYSTICK_MAX SCHAR_MAX

void pwm_setup()
{
	/* Setup for PWM */
	
	// Set PB6 as output pin (OC1B / Digital Pin 13)
	DDRB |= (1 << PB6);
	
	// WGM1 bits: Set Fast PWM Mode with ICR1 as TOP
	// COM1A bits: Set non-inverting mode
	// CS1 bits: Set prescaler to 8
	TCCR1A |= (1 << WGM11) | (1 << WGM10) | (1 << COM1B1) | (0 << COM1B0);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
	
	// Set Period: 20 ms
	OCR1A = 39999;
	// Set Duty Cycle: 1799 for 0.9ms, 2999 for 1.5ms, 4199 for 2.1ms
	OCR1B = 2999;
}

unsigned int pwm_saturation(unsigned int val)
{
	/* Keep the values for the Duty cycle in the correct range */
	
	if (val < DUTY_CYCLE_MIN)
		return DUTY_CYCLE_MIN;
		
	if (val > DUTY_CYCLE_MAX)
		return DUTY_CYCLE_MAX;
		
	return val;
}

void pwm_set(unsigned int input_val)
{
	/* Control the servo motor by the joystick position (X axis) */
	
	//Input: val has values in the range [JOYSTICK_MIN, JOYSTICK_MAX]
	//Output: values in the range [DUTY_CYCLE_MIN, DUTY_CYCLE_MAX]
	
	unsigned int output_val;
	
	// Compute the output value
	output_val = ((double)(input_val - JOYSTICK_MIN) / (JOYSTICK_MAX - JOYSTICK_MIN) ) * (DUTY_CYCLE_MAX - DUTY_CYCLE_MIN) + DUTY_CYCLE_MIN;

	// Set the Duty Cycle of the PWM with the obtained value
	OCR1B = pwm_saturation(output_val);
}
