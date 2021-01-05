/*
 * serial.h
 *
 * Created: 23.09.2016 16:46:10
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>

/*Initialization of the Serial Communication*/
void serial_init();

/* Receive a character over the Serial Communication */
int serial_getc(FILE *f);

/* Send a character (c) over the Serial Communication */
int serial_putc(char c, FILE *f);

#endif /* SERIAL_H_ */