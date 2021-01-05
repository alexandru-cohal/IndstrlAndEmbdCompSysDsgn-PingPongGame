/*
 * acmotor.c
 *
 * Created: 09.11.2016 11:12:23
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"
#include "ac_motor.h"
#include "TWI_Master.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void ac_motor_init()
{
	/* Initialize the interface with the AC motor */
	
	//PORT H as output
	DDRH = 0xFF;
	//PORT K as input (encoder data)
	DDRK = 0x00;
	//Initial values (Motor disabled and DIR = 1)
	MJ1 |=  (1<<DIR); 
	MJ1 &= ~(1<<EN);
	//Initializes TWI for I2C comm. with the DAC IC
	TWI_Master_Initialise();
	
	//toggle !reset to reset encoder
	MJ1 &= ~(1<<RST);
	_delay_us(20);
	MJ1 |= (1<<RST);
}

void ac_motor_stop()
{	/* Disables the motor */

	MJ1 &= ~(1<<EN);
}

void ac_motor_set_speed(unsigned char ac_motor_speed)
{
	/* Set the speed ac_motor_speed to AC motor */
	
	unsigned char messageBuf[4];
	unsigned char TWI_targetSlaveAddress = 0b0101000;
	

	messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (FALSE<<TWI_READ_BIT); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = 0x00;             // The first byte is used for commands.
	messageBuf[2] = AC_MOTOR_DEFAULT_SPEED;   // The second byte is used for the data.
	TWI_Start_Transceiver_With_Data( messageBuf, 3 );
}

void ac_motor_move_right(unsigned char ac_motor_speed)
{
	/* Set the AC motor to move to the right with the speed ac_motor_speed */
	
	//Setting direction
	MJ1 |= (1<<DIR);
	//Setting speed
	ac_motor_set_speed(ac_motor_speed);
	//Enabling motor
	MJ1 |= (1<<EN);
}

void ac_motor_move_left(unsigned char ac_motor_speed)
{
	/* Set the AC motor to move to the left with the speed ac_motor_speed */
	
	//Setting direction
	MJ1 &= ~(1<<DIR);
	ac_motor_set_speed(ac_motor_speed);
	//Enabling motor
	MJ1 |= (1<<EN);
}

void ac_motor_set_pos(int8_t ac_motor_pos)
{
	/* Move the AC motor by using the Joystick (Open loop) */
	
	if(ac_motor_pos < -5)
	{
		ac_motor_move_left(AC_MOTOR_DEFAULT_SPEED);
	//	printf("NODE2 : moving left\n");
	}
	else if((ac_motor_pos >= -5) && (ac_motor_pos <= 5))
	{
		ac_motor_stop();
		//printf("NODE2 : stop\n");
	}
	else if(ac_motor_pos > 5)
	{
		ac_motor_move_right(AC_MOTOR_DEFAULT_SPEED);
	//	printf("NODE2 : moving right\n");
	}	
}

int ac_motor_encoder()
{
	/* Read the value returned by the Encoder of the AC motor */
	
	// do0 - do7 => pk (A8) - pk (A15)
	
	int encoder_value = -1;
	char encoder_low, encoder_high;
	
	////toggle !reset to reset encoder
	//MJ1 &= ~(1<<RST);
	//_delay_us(20);
	//MJ1 |= (1<<RST);
	
	//enables output of encoder
	MJ1 &= ~(1 << OE);
	//select high byte
	MJ1 &= ~(1 << SEL);
	//wait
	_delay_us(30);
	//read high byte
	encoder_high = PINK;
	//select low byte
	MJ1 |= (1<<SEL);
	//wait
	_delay_us(30);
	//ready low byte
	encoder_low = PINK;
	
	////toggle !reset to reset encoder
	//MJ1 &= ~(1<<RST);
	//_delay_us(20);
	//MJ1 |= (1<<RST);
	
	//disable output of encoder
	MJ1|= (1<<OE);
	
	//calculates the counting
	encoder_value = (encoder_high<<8) | (encoder_low);
	
	//printf("Encoder output = %i\n",encoder_value);
	
	return encoder_value;
}

void ac_motor_timer_setup()
{	
	/* Set the sampling period of the PI controller (initialize a timer) */

	TCCR3B |= (1<<WGM32)  | (1<< CS31) | (1<<CS30);
	
	OCR3A = ((F_CPU / CONTROL_PRESCALER) * SAMPLING_TIME)-1;
	
	cli();
	
	//enabling Compare A match interrupt
	TIMSK3 |= (1<< OCIE3A);
	sei();
	
	//printf("OCR3A %i\n", OCR3A);
}

int ac_motor_speed_saturation(int min_limit, int max_limit, int initial_value)
{
	/* Keep initial_value in the [min_limit, max_limit] interval */
	
	if (initial_value < min_limit)
		return min_limit;
		
	if (initial_value > max_limit)
		return max_limit;
		
	return initial_value;
}
