/*
 * slider.h
 *
 * Created: 23.09.2016 18:06:22
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef SLIDER_H_
#define SLIDER_H_

typedef enum {
	SLIDER_LEFT,
	SLIDER_RIGHT
} Slider;

/* Return (signed char - [-128, 127]) the value returned by the slider slider ((an enumerated value)) */
signed char slider_get_value(Slider slider);

#endif /* SLIDER_H_ */