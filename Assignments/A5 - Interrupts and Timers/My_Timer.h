/*
 * My_Timer.h
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */

#ifndef MY_TIMER_H_
#define MY_TIMER_H_

#include "msp.h"

/*duty_cycle = 720 and Period = 960 will give you a 0.25 PWM*/
#define PERIOD 960
#define DUTY_CYCLE 480

//for 2-bit counter
#define MY_CCR0 1500
#define MY_CCR1 750

void setup_TIMER_A0(void);
void TA0_0_IRQHandler(void);
void TA0_N_IRQHandler(void);

#endif /* MY_TIMER_H_ */
