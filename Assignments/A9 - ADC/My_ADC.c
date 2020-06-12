/*
 * My_ADC.c
 *
 *  Created on: May 21, 2020
 *      Author: kattia
 */

#include "My_ADC.h"

extern uint8_t flag;

void select_sample_time_cycles(uint32_t numCycles){
    switch(numCycles){
        case NUM_CYCLES_8:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__8;
            break;
        case NUM_CYCLES_16:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__16;
            break;
        case NUM_CYCLES_32:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__32;
            break;
        case NUM_CYCLES_64:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__64;
            break;
        case NUM_CYCLES_96:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__96;
            break;
        case NUM_CYCLES_128:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__128;
            break;
        case NUM_CYCLES_192:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__192;
            break;
        default:
            ADC14->CTL0 |= ADC14_CTL0_SHT0__4;
            break;
    }
}

void select_clk_source(uint32_t clkSource){
    switch(clkSource){
    case MY_MODCLK:
        ADC14->CTL0 |= ADC14_CTL0_SSEL__MODCLK;
        break;
    case MY_SYSCLK:
        ADC14->CTL0 |= ADC14_CTL0_SSEL__SYSCLK;
        break;
    case MY_ACLK:
        ADC14->CTL0 |= ADC14_CTL0_SSEL__ACLK;
        break;
    case MY_SMCLK:
        ADC14->CTL0 |= ADC14_CTL0_SSEL__SMCLK;
        break;
    case MY_HSMCLK:
        ADC14->CTL0 |=  ADC14_CTL0_SSEL__HSMCLK;
        break;
    default:
        ADC14->CTL0 |= ADC14_CTL0_SSEL__MCLK;
        break;
    }
}

void setup_ADC14(void){
    ADC14->CTL0 |= ADC14_CTL0_ON;               //ADC 14 ON
    ADC14->CTL0 |= ADC14_CTL0_SHP;              //Sample and hold pulse select mode
    select_sample_time_cycles(NUM_CYCLES_128);  //sample time # of cycles
    select_clk_source(MY_MCLK);
    ADC14->CTL0 |= ADC14_CTL1_RES__14BIT;       //Use sampling timer, 14-bit conversion results
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_6;        //Use Channel 6 Avcc is by default 3.3v
    ADC14->IER0 |= ADC14_IER0_IE0;              // Enable ADC conv complete interrupt
    ADC14->CTL0 |= ADC14_CTL0_ENC;              // Enable conversions
}

uint32_t calibrate_digital_to_analog(uint16_t digitalValue){
    uint32_t result;
    result = 202*digitalValue + 4492;           //from trendline
    return result;
}


void ADC14_IRQHandler(void){
    digitalVal = 0;
    digitalVal = ADC14->MEM[0];                 // Read conversion result
    flag = 1;
}
