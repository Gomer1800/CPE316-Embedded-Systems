/*
 * My_Oscilloscope.h
 *
 *  Created on: May 16, 2020
 *      Author: Hayden
 */

#ifndef MY_FUNCGEN_H_
#define MY_FUNCGEN_H_

#include "msp.h"
#include <string.h>

// FSM STATES
enum STATE {INIT, DISPLAY_MENU, SQUARE, SAW, SINE};

// FUNCTION DEFINITIONS
void change_wave(char keypad_char);
char*get_wave_string(STATE);
char*get_freq_string(uint16_t period);

// STRUCTS
typedef struct wave_struct {
    uint32_t period;
    uint8_t dutycycle;
} wave_data_t;

// FREQUENCIES
#define P_100HZ 10000
#define P_200HZ 5000
#define P_300HZ 3333
#define P_400HZ 2500
#define P_500HZ 2000

#endif /* MY_FUNCGEN_H_ */
