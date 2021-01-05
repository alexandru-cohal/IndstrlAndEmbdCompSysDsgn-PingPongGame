/*
 * ext.c
 *
 * Created: 25.09.2016 14:37:14
 * Authors: Group 39 (Alexandru, Jens, Vitor)
 */ 

#include "ext.h"

volatile char * const ext_oled_c = (char * const) 0x4000;
volatile char * const ext_oled_d = (char * const) 0x4200;