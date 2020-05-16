/*
 * My_Wavegen.h
 *
 *  Created on: May 15, 2020
 *      Author: Hayden
 */

#ifndef MY_WAVEGEN_H_
#define MY_WAVEGEN_H_

#include "msp.h"
#include <stdlib.h>         // definition of malloc

// MACROS
#define DAC_3V 930
#define GRAN 50

// FSM STATES
enum STATE {INIT, SQUARE, SAW, SINE};

//Global pointer for the waveform struct
extern void *waveform;

typedef struct {
    enum STATE CURRENT_WAVE;
    uint8_t  DUTY_CYCLE;
    uint32_t PERIOD;
} Wave;

// Constructor
void* constructor_waveform(void);

// FUNCTION DEFINITIONS
void gen_square_wave(uint8_t dutycycle, uint32_t period);
void gen_triangle_wave(uint32_t period);
void gen_sine_wave(uint32_t period);

#endif /* MY_WAVEGEN_H_ */
