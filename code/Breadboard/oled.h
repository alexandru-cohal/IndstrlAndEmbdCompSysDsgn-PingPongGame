/*
 * oled.h
 *
 * Created: 23.09.2016 17:54:17
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef OLED_H_
#define OLED_H_

/* Initialization of the OLED */
void oled_init();

/* Reset the OLED */
void oled_reset();


/* Low level functions */

/* Send a command (command) to the OLED */
void oled_send_command(char command);

/* Send data (data) to the OLED */
void oled_send_data(char data);

/* Select a page (page) of the OLED */
void oled_select_page(int page);

/* Write a character (c) on the OLED */
void oled_write_char(char c);

/* Write a string (str) on the OLED */
void oled_write_string(const char *c);

#endif /* OLED_H_ */