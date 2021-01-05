/*
 * main.c
 *
 * Created: 02.11.2016 10:03:15
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "oled.h"

int main(void) {
	XMCRA |= (1 << SRE);
	//DDRE |=  0x01;
	//PORTE |= 0x01;
	//DDRD |= 0xff;
	_delay_ms(100);
	oled_init();
	//oled_reset();
	//oled_select_page(0);
	//oled_write_string("Hello World!");
	//oled_send_command(0XAF);
	//
	
	oled_send_command(0XAF);
	while (true) {
		oled_reset();
		oled_select_page(0);
		oled_write_string("Hello World!");
		//oled_send_command(0XAF);
		////_delay_ms(20);
		//oled_send_command(0XA5);
		
		_delay_ms(10);
	}
}