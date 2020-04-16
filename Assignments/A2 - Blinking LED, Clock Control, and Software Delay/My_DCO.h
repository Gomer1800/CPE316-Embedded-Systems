/*
 * My_DCO.h
 *
 *  Created on: Apr 12, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_DCO_H_
#define MY_DCO_H_

#include "msp.h"
#include  "STDINT.H" // definition of uint32_t

// Macros for human readable access to clock control settings
#define FREQ_1_5MHZ CS_CTL0_DCORSEL_0
#define FREQ_3MHZ   CS_CTL0_DCORSEL_1
#define FREQ_6MHZ   CS_CTL0_DCORSEL_2
#define FREQ_12MHZ  CS_CTL0_DCORSEL_3
#define FREQ_24MHZ  CS_CTL0_DCORSEL_4

// Functions

void setup_DCO(uint32_t);

void setup_MCLK_to_DCO(void);

uint32_t get_DCO_Frequency();

#endif /* MY_DCO_H_ */
