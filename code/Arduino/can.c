 /*
 * can.c
 *
 * Created: 13.10.2016 15:26:00
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <util/delay.h>
#include <stdio.h>

#include "mcp.h"

#include "can.h"

#define TXB0SIDH 0x31
#define TXB0SIDL 0x32
#define EXIDE 3
#define TXB0DLC 0x35
#define TXB0D0 0x36

#define RXB0SIDH 0x61
#define RXB0SIDL 0x62
#define RXB0DLC	0x65
#define RXB0DM 0x66

void can_init() {
	/* CAN Initialization */
	
	mcp_init();
}

void can_message_send(can_message* msg) 
{
	/* Send a CAN message (msg) */
	
	char SIDL_value = 0;
	char SIDH_value = 0;
	char DLC_value = 0;

	//Selection standard identifier (EXIDE = 0, pg. 20 MCP2515 data sheet)
	//Writing the 3 LSB of the identifier
	SIDL_value = (((msg->id)&7)<<5); 	
	mcp_write(TXB0SIDL, SIDL_value);

	//Writing the 8 MSB of the identifier
    SIDH_value =(msg->id)>>3;  
	mcp_write(TXB0SIDH,  SIDH_value);

	//Writing the length of data
	DLC_value = msg->length;
	mcp_write(TXB0DLC,DLC_value);

	//Writing the data into the TXB0 buffer
	for (int i = 0 ; i < msg->length; i++)
	{
		mcp_write(TXB0D0+i, msg->data[i]);
	}

	//Sending the packet package 
	mcp_request_to_send(1);
}


void can_message_receive(can_message *msg)
{
	/* Receive a CAN message (msg) */
	
	char idl, idh;

	_delay_us(100);
	//Getting the high part of the ID
	idh = mcp_read(RXB0SIDH);

	//Getting the low  part of the ID 
	idl = mcp_read(RXB0SIDL);

	//Building the 11-bit ID
	msg->id = (idh<<3)|(idl>>5);

	//Getting the length of the message
	msg->length = (mcp_read(RXB0DLC)) & 15;

	//Getting all the bytes of the message
	for (int i = 0 ; i < msg->length ; i++)
	{
		msg->data[i] = mcp_read(RXB0DM+i);
	}
}

void can_check_message_receive(can_message *msg)
{
	/* Check if a new CAN message (msg) is available and receive it */
	
	char CANINTF_value = mcp_read(CANINTF);
	
	if((CANINTF_value)&(1<<RX0IF))
	{
		can_message_receive(msg);
		mcp_write(CANINTF,CANINTF_value&(~(1<<RX0IF)));
		
		printf("Node2: ID = %i    Length = %i    Data = %s\n",msg->id, msg->length, msg->data);
	}
	else
	{
		printf("Node2: No message received...\n");
	}
}

void can_wait_message_receive(can_message *msg)
{
	/* Wait until a new CAN message (msg) arrives and receive it */
	
	char CANINTF_value = mcp_read(CANINTF);
	
	while(!((CANINTF_value)&(1<<RX0IF)))
	{
		//printf("ERROR -> EFLG: %i\n",mcp_read(0x2D));
		CANINTF_value = mcp_read(CANINTF);
	}
	
	can_message_receive(msg);
	mcp_write(CANINTF,CANINTF_value&(~(1<<RX0IF)));
}