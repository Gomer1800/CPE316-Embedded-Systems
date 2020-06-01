#include <My_Temp.h>

/*
 * dac.c
 *
 *  Created on: May 9, 2020
 *      Author: Luis Gomez
 */

void init_spi(void) {
    // P1.6 = USB0SIMO -> SDI
    // P1.5 = UCB0CLK -> SCLK
    // P5.1 -> CS

    P1->SEL0 |= BIT5 | BIT6; // Set P1.5 and P1.6 as SPI pins

    // Setup P4.1 (CS) as GPIO output
    P5->SEL0 &= ~BIT1;
    P5->SEL1 &= ~BIT1;
    P5->DIR |= BIT1;
    P5->OUT |= BIT1; // CS is high when idle

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

uint8_t temp_read(void) {
    uint16_t data       = 0;
    uint16_t signbit    = 0;
    P5->OUT &= ~BIT1;                           // Sets CS as low, signaling a transmission to the Temp Sensor
    if (EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG) {    // If the RX Buffer is full
        data = EUSCI_B0->RXBUF;          // Receive data
        signbit = data >> 15;

        if( signbit == 1){      // detect negative numbers
            P5->OUT |= BIT1;    // Sets CS back to high, signaling end of transmission
            return 0;
        }
        data = data >> 3;       // correct data for tristate bits
        if( data > CELCIUS_40){
            P5->OUT |= BIT1;    // Sets CS back to high, signaling end of transmission
            return 1;
        }
    }
    P5->OUT |= BIT1;            // Sets CS back to high, signaling end of transmission
    return 0;
}
