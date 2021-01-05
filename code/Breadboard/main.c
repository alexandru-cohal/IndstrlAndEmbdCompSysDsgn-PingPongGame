/*
 * main.c
 *
 * Created: 31.08.2016 09:37:01
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/eeprom.h>

#include "can.h"
#include "joystick.h"
#include "l10n.h"
#include "menu.h"
#include "slider.h"
#include "oled.h"
#include "serial.h"
#include "system.h"
#include "test.h"
#include "mcp.h"

// IDs of the CAN messages
#define MID_PWM_SET		  1048
#define MID_AC_MOTOR_SET  512
#define MID_SOLENOID_TRIG 444
#define MID_GOAL 11
#define HIGHSCORE_ADDRESS 0x42

void print_can_message(can_message *msg);

int goal_counter;
int game_time;

ISR(TIMER1_COMPA_vect)
{
	// Periodic interrupt - 1 second timer
	game_time++;
}

void send_control_values() {
	/* Send the values from the joystick (x axis) and from the slider (right) */
	can_message msg;

	int8_t w = 0;
	int8_t w2 = 0;

	uint8_t x_joy = control_read_channel_raw(2) ^ 0x80;

	int8_t x = slider_get_value(SLIDER_RIGHT);

	if (abs(x - w) > 0) {
		w = x;
		msg.id = MID_PWM_SET;
		msg.length = 1;
		*(msg.data) = -x;
		can_message_send(&msg);
	}

	if (abs(x_joy - w2) > 0) {
		w2 = x_joy;
		msg.id = MID_AC_MOTOR_SET;
		msg.length = 1;
		*(msg.data) = x_joy;

		can_message_send(&msg);
	}
	_delay_ms(10);
}

void start_game_1() {
	/* Start the Game 1 (goals limit) */
	
	goal_counter = 0;
	int last = -1;
	char s[32];

	/* main loop */
	while (true) {
		if (goal_counter >= 5) {
			break;
		} else if (goal_counter >= last) {
			last = goal_counter;
			oled_reset();
			oled_select_page(0);
			sprintf(s, _(S_GOALS), goal_counter);
			oled_write_string(s);
		}
		send_control_values();
	}

	oled_reset();
	oled_select_page(0);
	oled_write_string(_(S_GAME_OVER));
	oled_select_page(1);
	sprintf(s, _(S_GOALS), goal_counter);
	oled_write_string(s);
}

void start_game_2() {
	/* Start the Game 2 (time limit) */
	
	goal_counter = 0;
	game_time = 0;
	int last_counter = -1;
	int last_time = -1;
	char s[32];

	start_timer_1second();

	/* main loop */
	while (true) {
		if (game_time >= 10) {
			break;
		} else 	if (goal_counter >= last_counter || game_time >= last_time) {
			last_counter = goal_counter;
			last_time = game_time;
			oled_reset();
			oled_select_page(0);
			sprintf(s, _(S_TIME), game_time);
			oled_write_string(s);
			oled_select_page(1);
			sprintf(s, _(S_GOALS), goal_counter);
			oled_write_string(s);
		}
		send_control_values();
	}
	stop_timer_1second();

	int highscore = eeprom_read_word((void *)HIGHSCORE_ADDRESS);
	if (highscore == 0xffff) {
		highscore = 0;
	}
	if (goal_counter > highscore) {
		eeprom_write_word((void *)HIGHSCORE_ADDRESS, goal_counter);
	}

	oled_reset();
	oled_select_page(0);
	oled_write_string(_(S_TIME_OVER));
	oled_select_page(1);
	sprintf(s, _(S_GOALS), goal_counter);
	oled_write_string(s);
	oled_select_page(2);
	sprintf(s, _(S_HIGHSCORE), highscore);
	oled_write_string(s);
}

void show_highscore() {
	/* Print the highscore */
		
	char s[32];
	oled_reset();
	oled_select_page(0);
	sprintf(s, _(S_HIGHSCORE), eeprom_read_word((void *)HIGHSCORE_ADDRESS));
	oled_write_string(s);
}

ISR(INT1_vect)
{
	// Interrupt routine called when a CAN message is received
	
	can_message msgRX0;
	unsigned char CANINTF_value = mcp_read(CANINTF);

	if((CANINTF_value)&(1<<RX0IF))
	{
		mcp_write(CANINTF,CANINTF_value&(~(1<<RX0IF)));
		can_message_receive(&msgRX0);
		_delay_ms(1);
		//print_can_message(&msgRX0);
		if (msgRX0.id == MID_GOAL) {
			goal_counter++;
		}
	}
}

ISR(INT2_vect)
{
	// Interrupt routine called when the right touch button pressed
	
	can_message msgint2;

	msgint2.id = MID_SOLENOID_TRIG;
	msgint2.length = 1;
	msgint2.data[0] = 'R';  

	can_message_send(&msgint2);
}

void print_can_message(can_message *msg) {
	/* Print a CAN message (received as a CAN message structure) */
	
	printf("Node2 -> ID = %i    Length = %i    Data : ", msg->id, msg->length);
	for (int i = 0 ; i < msg->length ; i++ )
	{
		printf("%c", msg->data[i]);
	}
	printf("\n");
}

int main() {
	/* Initialize subsystems */
	system_init();
	serial_init();
	oled_init();
	oled_reset();
	system_setup();
	can_init();

	/* SRAM test */
	//test_sram();

	/* Select Language */
	language = menu_show_menu(language_menu, 4);

	/* Menu loop begin */
	while (true) {
		/* Show main menu */
		char *menu[] = {_(S_GAME_START_1), _(S_GAME_START_2), _(S_SHOW_HIGHSCROE)};
		uint8_t action = menu_show_menu(menu, sizeof(menu)/sizeof(char *));

		switch (action) {
			case 0:
				start_game_1();
				break;
			case 1:
				start_game_2();
				break;
			case 2:
				show_highscore();
				break;
		}
		oled_select_page(6);
		oled_write_string(_(S_JOYSTICK_MESSAGE));
		oled_select_page(7);
		oled_write_string(_(S_JOYSTICK_MESSAGE + 1));
		while (joystick_get_position() != LEFT);
	}
	/* Menu loop end */
}