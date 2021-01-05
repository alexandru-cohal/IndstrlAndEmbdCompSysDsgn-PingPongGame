/*
 * oled.c
 *
 * Created: 23.09.2016 17:54:25
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/pgmspace.h>
#include <string.h>
#include <util/delay.h>

#include "ext.h"
#include "font.h"

#include "oled.h"

void oled_init() {
	/* Initialization of the OLED */	
	
	oled_send_command(0xae); // display off
	oled_send_command(0xa1); // segment remap
	oled_send_command(0xda); // common pads hardware: alternative
	oled_send_command(0x12);
	oled_send_command(0xc8); // common output scan direction: com63~com0
	oled_send_command(0xa8); // multiplex ration mode: 63
	oled_send_command(0x3f);
	oled_send_command(0xd5); // display divide ratio/osc. freq. mode
	oled_send_command(0x80);
	oled_send_command(0x81); // contrast control
	oled_send_command(0x50);
	oled_send_command(0xd9); // set pre-charge period
	oled_send_command(0x21);
	oled_send_command(0x20); // Set Memory Addressing Mode
	oled_send_command(0x02);
	oled_send_command(0xdb); // VCOM deselect level mode
	oled_send_command(0x30);
	oled_send_command(0xad); // master configuration
	oled_send_command(0x00);
	oled_send_command(0xa4); // out follows RAM content
	oled_send_command(0xa6); // set normal display
	oled_send_command(0xaf); // display on
}

void oled_reset() {
	/* Reset the OLED */
	
	for (int i = 0; i < 8; i++) {
		oled_select_page(i);
		memset((char *)ext_oled_d, 0x00, 0x80);
	}
}

void oled_send_command(char command) {
	/* Send a command (command) to the OLED */
	
	ext_oled_c[0] = command;
}

void oled_send_data(char data) {
	/* Send data (data) to the OLED */
	
	ext_oled_d[0] = data;
}

void oled_select_page(int page) {
	/* Select a page (page) of the OLED */
	
	oled_send_command(0xb0 | (page & 0x0f));
	oled_send_command(0x00);
	oled_send_command(0x10);
}

void oled_write_char(char c) {
	/* Write a character (c) on the OLED */
	
	c -= 0x20;
	for (int i = 0; i < 5; i++) {
		oled_send_data(pgm_read_byte(&(font[(unsigned char)c][i])));
	}
}

void oled_write_string(const char *str) {
	/* Write a string (str) on the OLED */
	
	for (int i = 0; str[i]; i++) {
		oled_write_char(str[i]);
	}
}