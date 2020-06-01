/*
 * dac.h
 *
 *  Created on: May 11, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_TEMP_H_
#define MY_TEMP_H_

/* includes */
#include "msp.h"

/* macros */
#define CELCIUS_40     0x0280 // .0625 * 40_CELSIUS is 40 celsius in decimal

void    init_spi(void);

uint8_t temp_read(void);

double  int_to_temp(uint16_t);

#endif /* MY_TEMP_H_ */
