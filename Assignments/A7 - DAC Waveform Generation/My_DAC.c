#include <My_DAC.h>

/*
 * dac.c
 *
 *  Created on: May 9, 2020
 *      Author: Hayden
 */

void init_spi(void) {
    // P1.7 = USB0SOMI -> SDI
    // P1.5 = UCB0CLK  -> SCLK
    // P4.1 -> CS

    P1->SEL0 |= BIT5 | BIT6; // Set P1.5 and P1.6 as SPI pins

    // Setup P4.1 (CS) as GPIO output
    P4->SEL0 &= ~BIT1;
    P4->SEL1 &= ~BIT1;
    P4->DIR |= BIT1;
    P4->OUT |= BIT1; // CS is high when idle

    // Initialize eUSCI
    // set UCSWRST
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST;         // This puts the SPI into reset mode and allows config
    // Configure parameters
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MST;           // master mode
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_CKPL;         // clock is low when inactive
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_CKPH;          // Data is changed on the falling edge of the clock
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_MSB;           // MSB is transmitted first
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SEVENBIT;     // 8 bit data length mode
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SYNC;          // Synchronous mode
    EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SSEL__SMCLK;   // Select SMCLK as clock source
    // Clear UCSWRST
    EUSCI_B0->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;        // SPI is released for normal operation
}

uint8_t get_upper(uint16_t data) {
    uint8_t upper = DAC_CONFIG | ((data & DAC_DATA_UPPER) >> DAC_UPPER_SHIFT);
    return upper;
}

uint8_t get_lower(uint16_t data) {
    uint8_t lower = (data & DAC_DATA_LOWER) << DAC_LOWER_SHIFT;
    return lower;
}

void dac_write(uint16_t data) {
    uint8_t transmit_count = 0;
    P4->OUT &= ~BIT1;                               // Sets CS as low, signaling a transmission to the DAC
    while (transmit_count < 2) {
        if (EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG) {    // If the TX Buffer is ready for more data
            if (transmit_count == 0) {
                EUSCI_B0->TXBUF = get_upper(data);  // Transmit the first 8 bits
            } else {
                EUSCI_B0->TXBUF = get_lower(data);  // Transmit the last 8 bits
            }
            transmit_count++;
        }
    }
    P4->OUT |= BIT1;                                // Sets CS back to high, signaling end of transmission
}

uint16_t volt_to_int(double voltage) {
    return (uint16_t)((voltage/DAC_VREF)*(DAC_INT_MAX));
}
