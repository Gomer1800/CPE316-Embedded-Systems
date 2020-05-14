#include <My_DAC.h>

/*
 * dac.c
 *
 *  Created on: May 9, 2020
 *      Author: Hayden
 */

void init_spi(void) {
    // P1.6 = USB0SIMO -> SDI
    // P1.5 = UCB0CLK -> SCLK
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

dac_data_t *format_dac_data(uint16_t data) {
    dac_data_t *dac_data = 0;                         // these are the 16 bits that will be transmitted to the DAC
    dac_data->upper = (DAC_CONFIG << 4);              // Sets the config bits for the DAC
    dac_data->upper = ((data & DAC_DATA_UPPER) >> 6); // Sets the data bits in the first half
    dac_data->lower = ((data & DAC_DATA_LOWER) << 2); // Sets the remaining data bits in the second half
    dac_data->lower &= ~DAC_DC_BITS;                  // Sets the don't care bits to 0
    return dac_data;
}

void dac_write(uint16_t data) {
    dac_data_t *dac_data = format_dac_data(data);
    P4->OUT &= ~BIT1;                               // Sets CS as low, signaling a transmission to the DAC
    EUSCI_B0->TXBUF = dac_data->upper;              // transmit the first half of the data
    while (!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG))    // wait for the TXBUF to be ready for more data
        ;
    EUSCI_B0->TXBUF = dac_data->lower;              // transmit the second half of the data
}

uint16_t volt_to_int(double voltage) {
    return (uint16_t)((voltage/DAC_VREF)*(DAC_INT_MAX));
}
