/*
 * oled.h
 *
 * Created: 23.09.2016 17:54:17
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef OLED_H_
#define OLED_H_

void oled_init();
void oled_reset();

/* low level functions */
void oled_send_command(char command);
void oled_send_data(char data);

void oled_select_page(int page);
void oled_write_char(char c);
void oled_write_string(char *c);

#endif /* OLED_H_ */