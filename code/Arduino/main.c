/*
 * main.c
 *
 * Created: 19.10.2016 10:18:45
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "config.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <util/delay.h>

#include "mcp.h"
#include "can.h"
#include "pwm.h"
#include "serial.h"
#include "system.h"
#include "adc.h"
#include "ac_motor.h"
#include "TWI_Master.h"

// IDs of the CAN messages
#define MID_PWM_SET       1048
#define MID_AC_MOTOR_SET  512
#define MID_SOLENOID_TRIG 444
#define MID_GOAL 11

#define FILTER_LENGTH 4
#define THRESHOLD_IR_GOAL 100
#define THRESHOLD_IGNORE_AVERAGES_IR 20

#define KP 0.5
#define KI 0.1

unsigned int ir_values[FILTER_LENGTH]; // Array which stores the last 4 reads of the ADC (ir_values[0] = oldest value, ir_values[4] = newest value)
unsigned int old_average_ir_value;
int goal_flag = 0; // goal_flag = 1 when a valid goal was detected
int ignore_average_ir_value = 0; // counter for the ignored average values after a goal was scored (avoid spurious goals)
int goal_counter = 0;
volatile int8_t joy_x_pos = 0;

int error_old = 0, controller_output_old = 0, controller_output_current;

void print_can_message(can_message *msg) {
	/* Print a CAN message (received as a CAN message structure) */
	
	printf("Node2 -> ID = %i    Length = %i    Data : ", msg->id, msg->length);
	for (int i = 0 ; i < msg->length ; i++ )
	{
		printf("%c", msg->data[i]);
	}
	printf("\n");
}

void detect_goal()
{
	/* Detect if a goal was scored (IR beam) */
	
	int average_ir_value;
	can_message goal_msg;
	
	// Filter the values obtained after AD conversion (average of the last FILTER_LENGTH values)
	average_ir_value = 0;
	for (int i = 0; i < FILTER_LENGTH - 1; ++i)
	average_ir_value += ir_values[i];
	
	
	average_ir_value /= FILTER_LENGTH;
			
	// Compare the current and the previous filtered value with a threshold value
	if (average_ir_value < THRESHOLD_IR_GOAL && old_average_ir_value >= THRESHOLD_IR_GOAL)
	{
		goal_counter++;
		printf("GOAL :( -> Goal counter: %i \n", goal_counter);
		goal_flag = 1;
		
		goal_msg.id = MID_GOAL;
		strcpy(goal_msg.data,"Goal!");
		goal_msg.length = strlen(goal_msg.data);
		
		can_message_send(&goal_msg);	
	}
			
	// Ignore THRESHOLD_IGNORE_AVERAGES_IR reads after a goal (avoid spurious goals)
	if (goal_flag == 1)
	{
		ignore_average_ir_value++;
		if (ignore_average_ir_value == THRESHOLD_IGNORE_AVERAGES_IR)
		{
			goal_flag = 0;
			ignore_average_ir_value = 0;
		}
	}
			
	// Store the current filtered AD value
	old_average_ir_value = average_ir_value;
}

ISR(INT4_vect)
{
	// Interrupt routine called when a CAN message is received
	
	can_message msgRX0;
	unsigned char CANINTF_value = mcp_read(CANINTF);
	
	if((CANINTF_value)&(1<<RX0IF))
	{
		mcp_write(CANINTF,CANINTF_value&(~(1<<RX0IF)));
		can_message_receive(&msgRX0);
		//print_can_message(&msgRX0);

		if (msgRX0.id == MID_PWM_SET) {
			int8_t p = *(msgRX0.data);
			//printf("%hhi\n", p);
			pwm_set(p);
		}
		
		else if (msgRX0.id == MID_AC_MOTOR_SET) {
			int8_t p = *(msgRX0.data);
			//printf("AC_MOTOR_INT = %hhi\n", p);
			//ac_motor_set_pos(p);
			joy_x_pos = p;
		}
		
		else if(msgRX0.id == MID_SOLENOID_TRIG ) {
			
			printf("ST\n");
			//Trigger the solenoid (active low)
			PORTJ &= ~(1<<PJ1);
			_delay_ms(150);
			PORTJ |= (1<<PJ1);
		}
		
	}
}

ISR(ADC_vect)
{
	// Interrupt routine called when AD conversion is done
	
	// Read the high part and the low part of the AD conversion from the register
	unsigned int low_part = ADCL, high_part = ADCH;
	
	// Update (shift) the old values
	for (int i = 0; i < FILTER_LENGTH - 1; ++i)
		ir_values[i] = ir_values[i + 1];
	
	// Compute and store the newest result of the AD conversion
	// If ADLAR bit is set to 1 => ADCH = [ADC9 ... ADC2], ADCL = [ADC1 ADC0 ...] (page 286 ATMega2560 datasheet)
	ir_values[FILTER_LENGTH - 1] = (high_part << 2) + (low_part >> 6);
}

ISR(TIMER3_COMPA_vect)
{
	/* PI controller */
	//INTERRUPT of TIMER 3 with a period = SAMPLING_TIME, defined in ac_motor.h
	
	int error_current;
	int position_current = ac_motor_encoder(); // takes values between 0 and 9000
	int position_current_mapped;
		
	position_current_mapped = -(position_current * (255.0 / 9000.0)); // map to interval [-128, 127] if the motor is placed initially in the middle; multiply with -1 to have negative values on the left side
	
	error_current = joy_x_pos - position_current_mapped; // takes vales between [-255, 255]
	
	//printf("Joy: %i Encoder %i \n", joy_x_pos, position_current_mapped);
	//printf("Error: %i \n", error_current);
	
	controller_output_current = KP * (error_current - error_old) + SAMPLING_TIME * KI * error_current + controller_output_old;
	
	//printf("Controller output: %i \n", controller_output_current);
		
	error_old = error_current;
	controller_output_old = controller_output_current;
}

int main() 
{
	int speed = 0;
	
	/* Initialize subsystems */
	cli();
	
	system_init();
	serial_init();
	system_setup();
	can_init();
	pwm_setup();	
	adc_request();
	ac_motor_init();
	ac_motor_timer_setup();

	sei();
	
	_delay_ms(1000);
	
	// Main loop
	while(true)
	{
		/* Check if a goal was scored */
		detect_goal();

		// PI controller (with a deadzone in the middle of the joystick movement on X axis)
		if (controller_output_current < -5)
		{
			// Move to the left
	
			speed = ac_motor_speed_saturation(0, 255, -controller_output_current);
			
			//printf("LEFT - Speed: %i \n", ac_motor_speed_saturation(0, 255, -controller_output_current));
			ac_motor_move_left(speed);
		}
		else
			if (controller_output_current > 5)
			{
				// Move to the right
			
				speed = ac_motor_speed_saturation(0, 255, controller_output_current);
			
				//printf("RIGHT - Speed: %i \n", ac_motor_speed_saturation(0, 255, controller_output_current));
				ac_motor_move_right(speed);
			}
			else
			{
				// Halt !
				
				//printf("HALT! \n");
				ac_motor_stop();
			}
    }
}