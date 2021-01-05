/*
 * can.h
 *
 * Created: 13.10.2016 15:26:53
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include <stdint.h>

#ifndef CAN_H_
#define CAN_H_

#define CANINTF       0x2C
#define RX0IF		  0

/* The structure of a CAN message */
typedef struct can_message
{
	uint16_t id;
	uint8_t length;
	char data[8];
} can_message;

/* CAN Initialization */
void can_init();

/* Send a CAN message (msg) */
void can_message_send(can_message* msg);

/* Receive a CAN message (msg) */
void can_message_receive(can_message *msg);

/* Check if a new CAN message (msg) is available and receive it */
void can_check_message_receive(can_message *msg);

/* Wait until a new CAN message (msg) arrives and receive it */
void can_wait_message_receive(can_message *msg);

#endif /* CAN_H_ */