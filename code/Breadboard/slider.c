/*
 * slider.c
 *
 * Created: 23.09.2016 18:06:41
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

//#include "control.h"

#include "joystick.h"
#include "slider.h"

signed char slider_get_value(Slider slider) {
	/* Return (signed char - [-128, 127]) the value returned by the slider slider ((an enumerated value)) */
	
	unsigned char value;

	/* select channel to read */
	char channel;
	if (slider == SLIDER_LEFT) {
		channel = 4;
	} else if (slider == SLIDER_RIGHT) {
		channel = 3;
	} else {
		return 0;
	}

	/* read channel value and normalise (0..255 => -128..127) */
	value = control_read_channel_raw(channel) ^ 0x80;

	return value;
}