/*
 * ac_motor.h
 *
 * Created: 09.11.2016 11:13:24
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef AC_MOTOR_H_
#define AC_MOTOR_H_

#include <stdint.h>

#define AC_MOTOR_DEFAULT_SPEED 128
#define TWI_GEN_CALL 0x00 // The General Call address is 0
#define CONTROL_PRESCALER	64
#define SAMPLING_TIME	0.01 // time in seconds
#define MJ1  PORTH

#define DIR  PH1
#define SEL  PH3
#define EN	 PH4
#define OE   PH5 //OE is active low
#define RST  PH6 		

/* Initialize the interface with the AC motor */
void ac_motor_init();

/* Disables the motor */
void ac_motor_stop();

/* Set the speed ac_motor_speed to AC motor */
void ac_motor_set_speed(unsigned char ac_motor_speed);

/* Set the AC motor to move to the right with the speed ac_motor_speed */
void ac_motor_move_right(unsigned char ac_motor_speed);

/* Set the AC motor to move to the left with the speed ac_motor_speed */
void ac_motor_move_left(unsigned char ac_motor_speed);

/* Move the AC motor by using the Joystick (Open loop) */
void ac_motor_set_pos(int8_t ac_motor_pos);

/* Read the value returned by the Encoder of the AC motor */
int ac_motor_encoder();

/* Set the sampling period of the PI controller (initialize a timer) */
void ac_motor_timer_setup();

/* Keep initial_value in the [min_limit, max_limit] interval */
int ac_motor_speed_saturation(int min_limit, int max_limit, int initial_value);

#endif /* AC_MOTOR_H_ */