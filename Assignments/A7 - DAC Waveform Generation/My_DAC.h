/*
 * dac.h
 *
 *  Created on: May 11, 2020
 *      Author: Hayden
 */

#ifndef MY_DAC_H_
#define MY_DAC_H_

/* includes */
#include "msp.h"

/* macros */
#define DAC_CONFIG 0x30
#define DAC_DATA_UPPER 0x3C0
#define DAC_DATA_LOWER 0x03F
#define DAC_UPPER_SHIFT 6
#define DAC_LOWER_SHIFT 2
#define DAC_VREF 3.3
#define DAC_INT_MAX 1023

/* function definitions */
void init_spi(void);
void dac_write(uint16_t data);
uint16_t volt_to_int(double voltage);

#endif /* MY_DAC_H_ */
