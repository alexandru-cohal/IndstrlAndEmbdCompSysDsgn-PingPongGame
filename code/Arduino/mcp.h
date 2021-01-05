/*
 * mcp.h
 *
 * Created: 11.10.2016 13:20:48
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#ifndef MCP_H_
#define MCP_H_

/* CAN controller */

/* Initialization of the CAN controller */
int mcp_init();

/* Set mode (mode) - CAN controller */
void mcp_select_mode(char mode);

/* Reset internal registers & Enter in Configuration Mode - CAN controller */
void mcp_reset();

/* Read and return the data from a register (from the address address) - CAN controller */
char mcp_read(char address);

/* Write data (data) to a register (from the address address) - CAN controller */
void mcp_write(char address, char data);

/* Read and return the data from a receive buffer (defined by the LSB 2 bits from reg) - CAN controller */
char mcp_read_rx_buffer(char reg);

/* Write data (data) to a transmit buffer (defined by the LSB 3 bits from reg) - CAN controller */
void mcp_load_tx_buffer(char reg, char data);

/* Send a Request-to-Send message */
void mcp_request_to_send(char reg);

/* Return status bits */
char mcp_read_status();

/* Return RX status */
char mcp_rx_status();

/* Modify the bits determined by a mask (mask) from a register (having the address address) with specfic values (defined in data)  */
void mcp_bit_modify(char address, char mask, char data);

#endif /* MCP_H_ */