/*
 * My_ADC.h
 *
 *  Created on: May 21, 2020
 *      Author: kattia
 */

#include "msp.h"

#ifndef MY_ADC_H_
#define MY_ADC_H_

#define NUM_CYCLES_4        ADC14_CTL0_SHT0__4
#define NUM_CYCLES_8        ADC14_CTL0_SHT0__8
#define NUM_CYCLES_16       ADC14_CTL0_SHT0__16
#define NUM_CYCLES_32       ADC14_CTL0_SHT0__32
#define NUM_CYCLES_64       ADC14_CTL0_SHT0__64
#define NUM_CYCLES_96       ADC14_CTL0_SHT0__96
#define NUM_CYCLES_128      ADC14_CTL0_SHT0__128
#define NUM_CYCLES_192      ADC14_CTL0_SHT0__192

#define MY_MODCLK           ADC14_CTL0_SSEL__MODCLK
#define MY_SYSCLK           ADC14_CTL0_SSEL__SYSCLK
#define MY_ACLK             ADC14_CTL0_SSEL__ACLK
#define MY_MCLK             ADC14_CTL0_SSEL__MCLK
#define MY_SMCLK            ADC14_CTL0_SSEL__SMCLK
#define MY_HSMCLK           ADC14_CTL0_SSEL__HSMCLK

uint16_t digitalVal;

void select_sample_time_cycles(uint32_t numCycles);
void select_clk_source(uint32_t clkSource);
void setup_ADC14(void);
uint32_t calibrate_digital_to_analog(uint16_t digitalValue);
void ADC14_IRQHandler(void);

#endif /* MY_ADC_H_ */
