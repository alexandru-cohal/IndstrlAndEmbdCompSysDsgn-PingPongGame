/*
 * bitfield.h
 *
 * Created: 02.10.2016 17:42:10
 * Author:  Jens
 */ 

#ifndef BITFIELD_H_
#define BITFIELD_H_

#include <stdbool.h>

/* Get the value of a bit from a number (field) knowing its position (pos) */
bool bitfield_get_value(char *field, int pos);

/* Modify the value (value) of a bit from a number (field) knowing its position (pos) */
void bitfield_set_value(char *field, int pos, bool value);

#endif /* BITFIELD_H_ */