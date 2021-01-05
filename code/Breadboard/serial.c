/*
 * serial.c
 *
 * Created: 23.09.2016 16:46:21
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */

#include "config.h"

#include <avr/io.h>

#include "serial.h"

void serial_init() {
	/* Initialization of the Serial Communication */
	
	unsigned int ubrr = F_CPU / 16 / SERIAL_BAUD_RATE - 1;
	/* Baud rate config */
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;

	/* RXEN0 -> RX enable; TXEN0-> TX Enable. Setting both ON. */
	UCSR0B = 1 << RXEN0 | 1 << TXEN0;

	/* Selecting frame format: 8N2 */
	UCSR0C = 1 << URSEL0 | 1 << USBS0 | 3 << UCSZ00;

	/* Set input / output functions */
	fdevopen(serial_putc, serial_getc);
}

int serial_getc(FILE *f) {
	/* Receive a character over the Serial Communication */
	
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

int serial_putc(char c, FILE *f) {
	/* Send a character (c) over the Serial Communication */
	
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
	return c;
}