/*
 * menu.c
 *
 * Created: 28.09.2016 07:51:42
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#include "joystick.h"
#include "oled.h"

#include "menu.h"

void menu_print_menu(char **structure, int size, int element) {
	/* Print the menu */
	
	char selector[] = {0,0,8,20,8,0,0};
	oled_reset();
	for (int i = 0; i < size; i++) {
		oled_select_page(i);
		if (i == element) {
			for (int j = 0; j < 7; j++)
			oled_send_data(selector[j]);
		}
		else {
			for (int j = 0; j < 7; j++)
			oled_send_data(0x00);
		}
		oled_write_string(structure[i]);
	}
}

int menu_show_menu(char **structure, int size) {
	/* Menu functionality (option selection) */	
	
	int element = 0;
	int selected = 0;
	JoystickPosition position;
	bool button = false;
	do {
		menu_print_menu(structure, size, element);
		while (((position = joystick_get_position()) == CENTER) && !(button = joystick_button_pressed())) {
			_delay_ms(100);
		}
		if ((position == RIGHT) || button) {
			selected = true;
		} else if (position == UP) {
			if (element > 0) {
				element--;
			} else {
				element = size - 1;
			}
		} else if (position == DOWN) {
			if (element < (size - 1)) {
				element++;
			} else {
				element = 0;
			}
		}
		_delay_ms(500);
	} while (!selected);
	return element;
}