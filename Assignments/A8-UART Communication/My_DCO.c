#include "My_DCO.h"
/*
 * My_DCO.c
 *
 *  Created on: Apr 12, 2020
 *      Author: Luis Gomez
 */

// Input:  Frequency desired by user for the DCO on the dev launch board
// Output: None
void setup_DCO(uint32_t FREQ) {

    CS->KEY = CS_KEY_VAL;           // unlock CS registers
    CS->CTL0 = 0;                   // clear register CTL0

    switch(FREQ)
    {
        case FREQ_3MHZ:
            CS->CTL0 = FREQ_3MHZ;   // set DCO = 3 MHz
            break;
        case FREQ_6MHZ:
            CS->CTL0 = FREQ_6MHZ;   // set DCO = 6 MHz
            break;
        case FREQ_12MHZ:
            CS->CTL0 = FREQ_12MHZ;   // set DCO = 12 MHz
            break;
        case FREQ_24MHZ:
            CS->CTL0 = FREQ_24MHZ;   // set DCO = 24 MHz
            break;
        default:
            CS->CTL0 = FREQ_1_5MHZ;   // set DCO = 1.5 MHZ
            break;
    }

    CS->KEY = 0; // lock CS
}

void setup_MCLK_to_DCO(void) {

    CS->KEY = CS_KEY_VAL;   // unlocks CS

    // select DCO clock source for MCLK
    CS->CTL1 = CS_CTL1_SELM_3;

    CS->KEY = 0;            // locks CS
}

void setup_SMCLK_to_DCO(void){
    CS->KEY = CS_KEY_VAL;   // unlocks CS
    /*select DCO Clock source for SMCLK
     *this additional masking is done to get accurate frequencies
     *might affect the DAC
     */
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK) | CS_CTL1_SELS_3;
    CS->KEY = 0;    // locks CS
}

uint32_t get_DCO_Frequency() {

    uint32_t DCO_FREQUENCY = CS->CTL0;

    return DCO_FREQUENCY;
}


