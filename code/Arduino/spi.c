/*
 * spi.c
 *
 * Created: 05.10.2016 11:07:58
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/io.h>
#include <stdio.h>

#include "spi.h"

void spi_init() {
	/* Initialization of the SPI communication protocol */
	
	// Set MOSI, SS and SCK as output
	// Explanation: If SS is configured as an output, the pin is a general output pin which does not affect the SPI system. Typically, the pin will be driving the SS pin of the SPI Slave. (page 162 - Atmega162 datasheet)
	SPI_DDR |= 1 << SPI_SS | 1 << SPI_MOSI | 1 << SPI_SCK;

	// Enable SPI, select Master mode, set clock rate at fck/16 */
	SPCR |= 1 << SPE | 1 << MSTR | 1 << SPR0;

	// Select the SPI Mode: CPHA = 0 (sampling on the leading edge), CPOL = 0 (SCK is low when Idle)
	SPCR &= ~(1 << CPOL | 1 << CPHA);
}

void spi_transmit_receive(char * buffer, int size) {
	/* Transmit the buffer buffer of size size over the SPI */
	
	/* Start transmission by setting chip select to low */
	SPI_PORT &= ~(1 << SPI_SS);

	for (int i = 0; i < size; i++) {
		/* Send byte i of buffer */
		SPDR = buffer[i];

		/* Wait until byte transmission is complete (verify SPI interrupt flag from SPSR register) */
		while (!(SPSR & (1 << SPIF)));

		/* Read byte i and override buffer */
		buffer[i] = SPDR;
	}

	/* Stop transmission by setting chip select to high */
	SPI_PORT |= 1 << SPI_SS;
}