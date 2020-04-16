/*
 * My_Delays.h
 *
 *  Created on: Apr 15, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_DELAYS_H_
#define MY_DELAYS_H_

#include "msp.h"
#include  "STDINT.H" // definition of uint32_t

// Delay constants
#define DELAY40ms               ((uint32_t)40000) // 40 milliseconds
#define DELAY40us               ((uint32_t)40)    // 40 microseconds
#define FLOATING_POINT_DIVISOR  ((uint32_t)10000) // used to avoid floating point conversion errors

// Slopes from our data trend-lines on excel
#define SLOPE_1_5MHZ            ((uint32_t)1374)
#define SLOPE_3MHZ              ((uint32_t)2745)
#define SLOPE_6MHZ              ((uint32_t)4955)
#define SLOPE_12MHZ             ((uint32_t)9862)
#define SLOPE_24MHZ             ((uint32_t)19798)

void delay_us(uint32_t);

#endif /* MY_DELAYS_H_ */
