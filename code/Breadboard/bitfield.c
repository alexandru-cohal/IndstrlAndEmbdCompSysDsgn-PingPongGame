/*
 * bitfield.c
 *
 * Created: 02.10.2016 17:42:20
 * Author:  Jens
 */ 

#include <limits.h>

#include "bitfield.h"

bool bitfield_get_value(char *field, int pos) {
	/* Get the value of a bit from a number (field) knowing its position (pos) */
	
	return field[pos / CHAR_BIT] >> pos % CHAR_BIT & 1;
}

void bitfield_set_value(char *field, int pos, bool value) {
	/* Modify the value (value) of a bit from a number (field) knowing its position (pos) */
		
	if (value) {
		field[pos / CHAR_BIT] |= 1 << pos % CHAR_BIT;
	} else {
		field[pos / CHAR_BIT] &= ~(1 << pos % CHAR_BIT);
	}
}