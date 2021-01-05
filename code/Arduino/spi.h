/*
 * spi.h
 *
 * Created: 05.10.2016 11:07:49
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 


#ifndef SPI_H_
#define SPI_H_

/* Initialization of the SPI communication protocol */
void spi_init();

/* Transmit the buffer buffer of size size over the SPI */
void spi_transmit_receive(char * buffer, int size);

#endif /* SPI_H_ */