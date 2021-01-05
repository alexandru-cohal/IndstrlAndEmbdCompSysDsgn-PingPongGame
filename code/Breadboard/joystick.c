/*
 * joystick.c
 *
 * Created: 23.09.2016 17:42:50
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

//#include "control.h"
#include "oled.h"
#include "joystick.h"
#include "ext.h"

#define CALIBRATION_READ_NUMBER	   20
#define CALIBRATION_REACTION_TIME  2000 //time in ms
#define CALIBRATION_READ_DELAY     50 //time in ms
#define ADC_RAW_MIDDLE_VALUE	   127

unsigned char adc_max_x, adc_min_x, adc_max_y, adc_min_y, adc_middle_x, adc_middle_y;
int adc_offset_x, adc_offset_y;

JoystickCoordinates joystick_get_coordinates()
{
	/* Read and return the coordinates of the Joystick (on X and Y axis) */
	
	JoystickCoordinates joy_coord;
	
	/* read channel values and normalise (0..255 => -128..127) */
	joy_coord.x = control_read_channel_raw(2) ^ 0x80;
	joy_coord.y = control_read_channel_raw(1) ^ 0x80;
	
	return joy_coord;
};

JoystickPosition joystick_get_position() {
	/* Read and return (an enumerated value) the Joystick position (6 or 9 positions representations are available) */
	
	JoystickPosition position;
	
	signed char x;
	signed char y;
	
	/* read channel values and normalise (0..255 => -128..127) */
	x = control_read_channel_raw(2) ^ 0x80;
	y = control_read_channel_raw(1) ^ 0x80;

	/* calculate joystick position */
#ifdef USE_NINE_JOYSTICK_POSITIONS
	if (x < -16) {
		position = LEFT;
	} else if (x > +16) {
		position = RIGHT;
	} else {
		position = CENTER;
	}

	if (y < -16) {
		position += 1;
	} else if (y > +16) {
		position -= 1;
	}
#else // USE_NINE_JOYSTICK_POSITIONS
	if (abs(x) < 16 && abs(y) < 16) {
		position = CENTER;
	} else {
		if (abs(x) > abs(y)) {
			if (x < 0) {
				position = LEFT;
			} else {
				position = RIGHT;
			}
		} else {
			if (y < 0) {
				position = DOWN;
			} else {
				position = UP;
			}
		}
	}
#endif // USE_NINE_JOYSTICK_POSITIONS

	return position;
}

const char * joystick_get_string(JoystickPosition position) {
	/* Return (as a char) the Joystick position */
	
	const char *positions[] = {"Left Up", "Left", "Left Down", "Up", "Center", "Down", "Right Up", "Right", "Right Down"};
	return positions[position];
}

void joystick_calibration()
{
	/* Calibrate the joystick. */
	/* Read the X and Y values for the extreme position and for the central position. */
	/* Map the two intervals in the intervals [0, 255] and fix the central point to 127 */
	
	unsigned int adc_raw_value_sum  = 0;
	unsigned int adc_raw_value_sum2 = 0;
	
	oled_select_page(0);
	oled_write_string("Joystick Calibration");
	oled_select_page(2);
	oled_write_string("Move the joystick:");
	
	// -------------- Y Maximum calibration ----------------
	oled_select_page(4);
	oled_write_string("MAXIMUM UP");
	//delay for user's reaction time
	_delay_ms(CALIBRATION_REACTION_TIME);
	//Calculating the average value
	adc_raw_value_sum = 0;
	for(int i = 0 ; i < CALIBRATION_READ_NUMBER; i++)
	{
		adc_raw_value_sum += control_read_channel_raw(1);
		_delay_ms(CALIBRATION_READ_DELAY); 
	}
	adc_max_y = adc_raw_value_sum / CALIBRATION_READ_NUMBER;
	
	// -------------- Y Minimum calibration ----------------
	oled_select_page(4);
	oled_write_string("MAXIMUM DOWN");
	//delay for user's reaction time
	_delay_ms(CALIBRATION_REACTION_TIME);
	//Calculating the average value
	adc_raw_value_sum = 0;
	for(int i = 0 ; i < CALIBRATION_READ_NUMBER; i++)
	{
		adc_raw_value_sum += control_read_channel_raw(1);
		_delay_ms(CALIBRATION_READ_DELAY);
	}
	adc_min_y = adc_raw_value_sum / CALIBRATION_READ_NUMBER;
	
	// -------------- X Maximum calibration ----------------
	oled_select_page(4);
	oled_write_string("MAXIMUM LEFT");
	//delay for user's reaction time
	_delay_ms(CALIBRATION_REACTION_TIME);
	//Calculating the average value
	adc_raw_value_sum = 0;
	for(int i = 0 ; i < CALIBRATION_READ_NUMBER; i++)
	{
		adc_raw_value_sum += control_read_channel_raw(2);
		_delay_ms(CALIBRATION_READ_DELAY);
	}
	adc_min_x = adc_raw_value_sum / CALIBRATION_READ_NUMBER;
	
	// -------------- X Minimum calibration ----------------
	oled_select_page(4);
	oled_write_string("MAXIMUM RIGHT");
	//delay for user's reaction time
	_delay_ms(CALIBRATION_REACTION_TIME);
	//Calculating the average value
	adc_raw_value_sum = 0;
	for(int i = 0 ; i < CALIBRATION_READ_NUMBER; i++)
	{
		adc_raw_value_sum += control_read_channel_raw(2);
		_delay_ms(CALIBRATION_READ_DELAY);
	}
	adc_max_x = adc_raw_value_sum / CALIBRATION_READ_NUMBER;
	
	// -------------- X and Y middle calibration ----------------
	oled_select_page(4);
	oled_write_string("to the CENTER");
	//delay for user's reaction time
	_delay_ms(CALIBRATION_REACTION_TIME);
	//Calculating the average value
	adc_raw_value_sum  = 0;
	adc_raw_value_sum2 = 0;
	for(int i = 0 ; i < CALIBRATION_READ_NUMBER; i++)
	{
		adc_raw_value_sum += control_read_channel_raw(1);
		adc_raw_value_sum2 += control_read_channel_raw(2);
		_delay_ms(CALIBRATION_READ_DELAY);
	}
	adc_middle_y = adc_raw_value_sum / CALIBRATION_READ_NUMBER;
	adc_middle_x = adc_raw_value_sum2 / CALIBRATION_READ_NUMBER;	
	
	//Reseting the OLED
	oled_reset();
	
	//Calculating the offset for the adc values 
	adc_offset_x = adc_middle_x - ADC_RAW_MIDDLE_VALUE;
	adc_offset_y = adc_middle_y - ADC_RAW_MIDDLE_VALUE;
		
	printf("%i %i %i %i %i %i", adc_max_y, adc_min_y, adc_min_x, adc_max_x, adc_middle_y, adc_middle_x);
	
	_delay_ms(5000);
}

unsigned char control_read_channel_raw(char channel) {
	/* Read and return (char value) the raw value from a channel (channel) from the ADC (the results of the calibration are not used) */
	
	// Write channel symbol to read data
	ext_adc[0] = channel + 3;
	// Wait until received flag is true
	//while(!adc_ch_r);
	_delay_us(80);
	// Read result and return
	unsigned char result = ext_adc[0];
	_delay_ms(10);
	return result;
}

unsigned char control_read_channel(char channel) {
	/* Read and return (char value) the value a channel (channel) from the ADC after applying the results of the calibration */
		
	unsigned char adc_raw_value = control_read_channel_raw(channel);
	
	printf("Alternative control read channel called :O\n");
	
	if(channel == 1)
	{
		if(adc_raw_value < adc_middle_x)
		return adc_value_mapping(adc_min_y,adc_middle_y,0,127, adc_raw_value);
		else
		return adc_value_mapping(adc_middle_y,adc_max_y,128,255,adc_raw_value);		
	}
	else if(channel == 2)
	{
		if(adc_raw_value < adc_middle_x)
			return adc_value_mapping(adc_min_x,adc_middle_x,0,127, adc_raw_value);
		else
			return adc_value_mapping(adc_middle_x,adc_max_x,128,255,adc_raw_value);	
	}
	else
		return adc_raw_value;
}

unsigned char adc_value_mapping(unsigned char min_init_interval, unsigned char max_init_interval, unsigned char min_final_interval, unsigned char max_final_interval, unsigned char value_init)
{
	/* Map a value (value_init) from the interval [min_init_interval, max_init_interval] into the interval [min_final_interval, max_final_interval] */
	
	return ((double)(value_init - min_init_interval)/(max_init_interval - min_init_interval))*(max_final_interval-min_final_interval) + min_final_interval;
}

bool joystick_button_pressed() {
	/* Check if the Joystick button is pressed (return 1 when button is pressed, 0 otherwise) */
	
	return !((PIND >> PIND2) & 1);
}