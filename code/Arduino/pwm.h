/*
 * pwm.h
 *
 * Created: 27.10.2016 15:43:01
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef PWM_H_
#define PWM_H_

/* Setup for PWM */
void pwm_setup();

/* Keep the values for the Duty cycle in the correct range */
unsigned int pwm_saturation(unsigned int val);

/* Control the servo motor by the joystick position (X axis) */
void pwm_set(unsigned int input_val);

#endif /* PWM_H_ */