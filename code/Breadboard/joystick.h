/*
 * joystick.h
 *
 * Created: 23.09.2016 17:42:41
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdbool.h>

/* Structure for storing Joystick coordinates */
typedef struct JoystickCoordinates
{
	signed char x;
	signed char y;
} JoystickCoordinates;

/* ENUM for Joystick direction */
typedef enum {
	LEFT_UP,
	LEFT,
	LEFT_DOWN,
	UP,
	CENTER,
	DOWN,
	RIGHT_UP,
	RIGHT,
	RIGHT_DOWN
} JoystickPosition;

/* Read and return the coordinates of the Joystick (on X and Y axis) */
JoystickCoordinates joystick_get_coordinates();

/* Read and return (an enumerated value) the Joystick position (6 or 9 positions representations are available) */
JoystickPosition joystick_get_position();

/* Return (as a char) the Joystick position */
const char * joystick_get_string(JoystickPosition position);

/* Calibrate the joystick. */
void joystick_calibration();

/* Read and return (char value) the raw value from a channel (channel) from the ADC (the results of the calibration are not used) */
unsigned char control_read_channel_raw(char channel);

/* Read and return (char value) the value a channel (channel) from the ADC after applying the results of the calibration */
unsigned char control_read_channel(char channel);

/* Map a value (value_init) from the interval [min_init_interval, max_init_interval] into the interval [min_final_interval, max_final_interval] */
unsigned char adc_value_mapping(unsigned char min_init_interval, unsigned char max_init_interval, unsigned char min_final_interval, unsigned char max_final_interval, unsigned char value_init);

/* Check if the Joystick button is pressed (return 1 when button is pressed, 0 otherwise) */
bool joystick_button_pressed();

#endif /* JOYSTICK_H_ */