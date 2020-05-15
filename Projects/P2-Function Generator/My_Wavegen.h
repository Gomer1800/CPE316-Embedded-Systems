/*
 * My_Wavegen.h
 *
 *  Created on: May 15, 2020
 *      Author: Hayden
 */

#ifndef MY_WAVEGEN_H_
#define MY_WAVEGEN_H_

#include "msp.h"

// MACROS
#define DAC_3V 930
#define GRAN 50

// SINE LOOKUP TABLE
static uint16_t sine_lookup_q1[32] = {
465, 489, 512, 535, 559, 582, 604, 626,
648, 670, 691, 711, 731, 750, 768, 785,
802, 818, 833, 847, 860, 872, 882, 892,
901, 909, 915, 920, 925, 928, 929, 930
};

// FUNCTION DEFINITIONS
void gen_square_wave(uint8_t dutycycle, uint32_t period);
void gen_triangle_wave(uint32_t period);
void gen_sine_wave(uint32_t period);

#endif /* MY_WAVEGEN_H_ */
