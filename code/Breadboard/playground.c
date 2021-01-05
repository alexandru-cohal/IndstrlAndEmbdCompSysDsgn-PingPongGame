/*
 * playground.c
 *
 * Created: 02.10.2016 14:52:42
 * Author:  Jens
 */ 

#include "config.h"

#include <avr/eeprom.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "bitfield.h"
#include "joystick.h"
#include "oled.h"

#include "playground.h"

#define FIELD_SIZE_X 16
#define FIELD_SIZE_Y 32
#define FIELD_SIZE (FIELD_SIZE_X * FIELD_SIZE_Y)
#define FIELD_MAP_SIZE (FIELD_SIZE / CHAR_BIT)
#define RANDOM_ADDRESS 42
#define HIGHSCORE_ADDRESS 44

char * const field = (char * const) 0x1800;
char * const food = (char * const) 0x1a00;
char * const tail = (char * const) 0x1b00;
char * const worm = (char * const) 0x1c00;

const short symbols[] = {
	0x0000,
	0x6996,
	0x0660
};

void playground_start() {
	/* Start! :D */
	
	srand(eeprom_read_word((void *)RANDOM_ADDRESS));
	eeprom_write_word((void *)RANDOM_ADDRESS, rand());
	int p;
	int l = 3;
	int s = 1;
	int x = rand() % FIELD_SIZE_X;
	int y = rand() % FIELD_SIZE_Y;
	JoystickPosition u;
	JoystickPosition v;
	for (int i = 0; i < l; i++) {
		worm[2 * i + 0] = x;
		worm[2 * i + 1] = y;
	}
	u = (y >= 16) ? LEFT : RIGHT;
	memset(food, 0, FIELD_MAP_SIZE);
	memset(tail, 0, FIELD_MAP_SIZE);

	/* game loop begin */
	loop:
	v = joystick_get_position();

	/* set new direction */
	u = (v != CENTER) ? v : u;

	/* set new position */
	if (u == UP) x--;
	if (u == DOWN) x++;
	if (u == LEFT) y--;
	if (u == RIGHT) y++;

	/* calculate array position */
	p = x * FIELD_SIZE_Y + y;

	/* border collision */
	if (x < 0 || x >= FIELD_SIZE_X || y < 0 || y >= FIELD_SIZE_Y) {
		goto over;
	}

	/* tail collision */
	if (bitfield_get_value(tail, p)) {
		goto over;
	}

	/* eat food */
	if (bitfield_get_value(food, p)) {
		bitfield_set_value(food, p, 0);
		l++;
		s += (l % 8) ? 0 : 1;
	}

	/* move worm */
	for (int i = l - 1; i > 0; i--) {
		worm[2 * i + 0] = worm[2 * i - 2];
		worm[2 * i + 1] = worm[2 * i - 1];
	}

	/* head position */
	worm[0] = x;
	worm[1] = y;

	/* create worm bitfield */
	memset(tail, 0, FIELD_MAP_SIZE);
	for (int i = 0; i < l; i++) {
		bitfield_set_value(tail, worm[2 * i] * FIELD_SIZE_Y + worm[2 * i + 1], 1);
	}

	/* add new food */
	if (!(rand() % 8)) {
		int q = rand() % FIELD_SIZE;
		if (!bitfield_get_value(tail, q)) {
			bitfield_set_value(food, q, 1);
		}
	}

	/* remove rotten food */
	bitfield_set_value(food, rand() % FIELD_SIZE, 0);

	/* prepare output */
	memset(field, 0, FIELD_SIZE);
	for (int i = 0; i < FIELD_SIZE; i++) {
		if (bitfield_get_value(tail, i)) {
			field[i] = 1;
		} else if (bitfield_get_value(food, i)) {
			field[i] = 2;
		}
	}

	/* output */
	oled_reset();
	for (int i = 0;  i < 8; i++) {
		oled_select_page(i);
		for (int j = 0; j < 32; j++) {
			for (int k = 0; k < 4; k++) {
				oled_send_data(((symbols[(unsigned char)field[(2 * i) * FIELD_SIZE_Y + j]] >> (4 * k)) & 0x0f) | (((symbols[(unsigned char)field[(2 * i + 1) * FIELD_SIZE_Y + j]] >> (4 * k)) & 0x0f) << 4));
			}
		}
	}

	/* delay */
	if (s == 1) {
		_delay_ms(200);
	} else if (s == 2) {
		_delay_ms(150);
	} else if (s == 3) {
		_delay_ms(100);
	} else if (s == 4) {
		_delay_ms(50);
	} else {
		_delay_ms(25);
	}

	/* game loop end */
	goto loop;

	/* game over */
	over:
	eeprom_write_word((void *)RANDOM_ADDRESS, rand());
	int h = eeprom_read_word((void *)HIGHSCORE_ADDRESS);
	if (h == 0xffff) {
		h = 0;
	}
	if (l > h) {
		eeprom_write_word((void *)HIGHSCORE_ADDRESS, l);
	}
	char m[24];
	memset(m, 32, 8);
	oled_reset();
	oled_select_page(2);
	sprintf(&m[8], "Game over!");
	oled_write_string(m);
	oled_select_page(4);
	sprintf(&m[6], "Score: %7i", l);
	oled_write_string(m);
	oled_select_page(5);
	sprintf(&m[6], "Highscore: %3i", h);
	oled_write_string(m);
	wait:
	_delay_ms(10000);
	goto wait;
}

void playground_init() {
	/* Playground initialization :D */
	
	if (joystick_get_position() == LEFT) {
		playground_start();
	}
}