/*
 * mcp.c
 *
 * Created: 11.10.2016 13:20:59
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "spi.h"

#include "mcp.h"

/* define CAN modes set of MCP2515 */
#define MCP_MODE_NORMAL   0b00000000
#define MCP_MODE_SLEEP    0b00100000
#define MCP_MODE_LOOPBACK 0b01000000
#define MCP_MODE_LISTEN   0b01100000
#define MCP_MODE_CONFIG   0b10000000

/* define instructions set of MCP2515 */
#define MCP_INSTRUCTION_RESET           0b11000000
#define MCP_INSTRUCTION_READ            0b00000011
#define MCP_INSTRUCTION_WRITE           0b00000010
#define MCP_INSTRUCTION_READ_RX_BUFFER  0b10010000
#define MCP_INSTRUCTION_LOAD_TX_BUFFER  0b01000000
#define MCP_INSTRUCTION_REQUEST_TO_SEND 0b10000000
#define MCP_INSTRUCTION_READ_STATUS     0b10100000
#define MCP_INSTRUCTION_RX_STATUS       0b10110000
#define MCP_INSTRUCTION_BIT_MODIFY      0b00000101

/* define other things */
#define CANCTRL       0x0F
#define CANSTAT       0x0E
#define MODE_MASK     0xE0
#define CANINTF       0x2C
#define CANINTE		  0x2B
#define RX0IF		  0
#define RX0IE		  0

int mcp_init()
{
	/* Initialization of the CAN controller */
	
	char canstat;

	spi_init();
	printf ("DDR_SPI stat: %d \n", SPI_DDR);
	
	mcp_reset();

	_delay_ms(20);
	
	//Reading the current status of the CAN controller
	canstat = mcp_read(CANSTAT);
	printf ("CANstat: %d \n", canstat);

	//Checking if the CAN controller has entered in configuration mode
	if((canstat & MODE_MASK) != MCP_MODE_CONFIG )
	{
		printf("\n MCP2515 is not in configuration mode after reset :( \n");
		return 1;
	}
	//Disabling filters
	mcp_write(0x60, 0b01100000);
	//Enabling Interrupt RX0IE
	mcp_write(CANINTE, 1<<RX0IE);
	// Setting NORMAL MODE
	mcp_select_mode(MCP_MODE_NORMAL);
	//mcp_select_mode(MCP_MODE_LOOPBACK);

	return 0;
}

void mcp_select_mode(char mode) {
	/* Set mode (mode) - CAN controller */
	
	mcp_bit_modify(CANCTRL, MODE_MASK, mode);
}

void mcp_reset() {
	/* Reset internal registers & Enter in Configuration Mode - CAN controller */
	
	char buffer[] = {MCP_INSTRUCTION_RESET};
	spi_transmit_receive(buffer, sizeof(buffer));
}

char mcp_read(char address) {
	/* Read and return the data from a register (from the address address) - CAN controller */
	
	char buffer[] = {MCP_INSTRUCTION_READ, address, 0};
	spi_transmit_receive(buffer, sizeof(buffer));
	return buffer[2];
}

void mcp_write(char address, char data) {
	/* Write data (data) to a register (from the address address) - CAN controller */
	
	char buffer[] = {MCP_INSTRUCTION_WRITE, address, data};
	spi_transmit_receive(buffer, sizeof(buffer));
}

char mcp_read_rx_buffer(char reg) {
	/* Read and return the data from a receive buffer (defined by the LSB 2 bits from reg) - CAN controller */
	/* Datasheet MCP2515 - page 66 */
	
	char instruction = MCP_INSTRUCTION_READ_RX_BUFFER | (reg << 1);
	char buffer[] = {instruction, 0};
	spi_transmit_receive(buffer, sizeof(buffer));
	return buffer[1];
}

void mcp_load_tx_buffer(char reg, char data) {
	/* Write data (data) to a transmit buffer (defined by the LSB 3 bits from reg) - CAN controller */
	/* Datasheet MCP2515 - page 66 */
	
	char instruction = MCP_INSTRUCTION_LOAD_TX_BUFFER | reg;
	char buffer[] = {instruction, data};
	spi_transmit_receive(buffer, sizeof(buffer));
}

void mcp_request_to_send(char reg) {
	/* Send a Request-to-Send message */
	
	char buffer[] = {MCP_INSTRUCTION_REQUEST_TO_SEND | reg};
	spi_transmit_receive(buffer, sizeof(buffer));
}

char mcp_read_status() {
	/* Return status bits */
	
	char buffer[] = {MCP_INSTRUCTION_READ_STATUS, 0, 0};
	spi_transmit_receive(buffer, sizeof(buffer));
	return buffer[1];
}

char mcp_rx_status() {
	/* Return RX status */
	
	char buffer[] = {MCP_INSTRUCTION_RX_STATUS, 0, 0};
	spi_transmit_receive(buffer, sizeof(buffer));
	return buffer[1];
}

void mcp_bit_modify(char address, char mask, char data) {
	/* Modify the bits determined by a mask (mask) from a register (having the address address) with specfic values (defined in data)  */
	
	char buffer[] = {MCP_INSTRUCTION_BIT_MODIFY, address, mask, data};
	spi_transmit_receive(buffer, sizeof(buffer));
}