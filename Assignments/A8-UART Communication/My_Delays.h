/*
 * My_Delays.h
 *
 *  Created on: Apr 15, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_DELAYS_H_
#define MY_DELAYS_H_

#include "msp.h"
#include "My_DCO.h"
//#include  "STDINT.H" // definition of uint32_t

// Delay constants

// Seconds
#define DELAY10S                 ((uint32_t)10000000)   // 10s
#define DELAY1S                 ((uint32_t)1000000)     // 1s

// Milliseconds
#define DELAY500MS              ((uint32_t)500000)  // 500ms
#define DELAY100MS              ((uint32_t)100000)  // 100ms
#define DELAY50MS               ((uint32_t)50000)   // 50ms
#define DELAY40MS               ((uint32_t)40000)   // 40ms
#define DELAY25MS               ((uint32_t)25000)   //25ms
#define DELAY5MS                ((uint32_t)5000)    // 5ms
#define DELAY4_1MS              ((uint32_t)4500)    // 4.1ms
#define DELAY2MS                ((uint32_t)2000)    // 2ms

// Microseconds
#define DELAY40us               ((uint32_t)40)    // 40 microseconds

// Math Constants
#define FLOATING_POINT_DIVISOR  ((uint32_t)10000) // used to avoid floating point conversion errors

// Slopes from our data trend-lines on excel
#define SLOPE_1_5MHZ            ((uint32_t)1374)
#define SLOPE_3MHZ              ((uint32_t)2745)
#define SLOPE_6MHZ              ((uint32_t)4955)
#define SLOPE_12MHZ             ((uint32_t)9862)
#define SLOPE_24MHZ             ((uint32_t)19798)

void delay_us(uint32_t);

#endif /* MY_DELAYS_H_ */
