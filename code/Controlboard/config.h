/*
 * config.h
 *
 * Created: 25.09.2016 14:28:47
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 16000000UL
#define SERIAL_BAUD_RATE 9600

#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_SS	 PB7
#define SPI_MOSI PB2
#define SPI_MISO PB3
#define SPI_SCK  PB1

#endif /* CONFIG_H_ */