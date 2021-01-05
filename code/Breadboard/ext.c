/*
 * ext.c
 *
 * Created: 25.09.2016 14:37:14
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "ext.h"

/* Address partitioning */
volatile char * const ext_ram    = (char * const) 0x1800;
volatile char * const ext_adc    = (char * const) 0x1400;
volatile char * const ext_oled_c = (char * const) 0x1000;
volatile char * const ext_oled_d = (char * const) 0x1200;