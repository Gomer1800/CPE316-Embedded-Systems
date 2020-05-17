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

//Define macros for menu
#define STRING_SQUARE     ((char *)("SQUARE"))
#define STRING_SINE       ((char *)("SINE"))
#define STRING_SAW        ((char *)("SAW TOOTH"))

// FUNCTION DEFINITIONS
void gen_square_wave(double dutycycle, uint32_t period);
void gen_sawtooth_wave(uint32_t period);
void gen_sine_wave(uint32_t period);

#endif /* MY_WAVEGEN_H_ */
