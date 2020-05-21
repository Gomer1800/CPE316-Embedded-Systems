/*
 * My_Timer.h
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */

#ifndef MY_TIMER_H_
#define MY_TIMER_H_

#include "msp.h"
#include "My_DCO.h"

// Calculated based on formula
// PERIOD_COUNTER = Tdesired * Fclock
#define PERIOD_COUNTER 1500

void setup_TIMER_A0(void);
void TA0_0_IRQHandler(void);

#endif /* MY_TIMER_H_ */
