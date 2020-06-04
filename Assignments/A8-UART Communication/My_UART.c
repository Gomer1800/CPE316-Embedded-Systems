#include "My_UART.h"
/*
 * My_UART.c
 *
 *  Created on: Jun 2, 2020
 *      Author: Wulfkine
 *
 *  Important Pins
 *
 *  P1.2 - RX
 *  P1.3 - TX
 */

void init_UART(void){
    /*
     * Initializing eUSCI_A module
     * 1. Set UCSWRST
     * 2. Init all eUSCI_A registers w/ UCSWRST = 1
     * 3. Configure Ports
     * 4. Clear UCSWRST
     * 5. (optional) Enable interrupts with UCRXIE or UCTXIE
     */

    // 1.
    EUSCI_A0->CTLW0 |=  EUSCI_A_CTLW0_SWRST;    // software reset enable
    // 2.
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SYNC;     // UART mode selected
    EUSCI_A0->CTLW0 |=  EUSCI_A_CTLW0_MODE_3;   // UART mode w/ automatic baud rate detection
    //EUSCI_A0->CTLW0 |=  EUSCI_A_CTLW0_MST;      // master mode select
    //EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_CKPL;     // clock is low when inactive
    //EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_MSB;      // LSB first
    //EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT; // 8-bit mode select
    // 3.

    // 4.
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // software reset disable
}

uint8_t UART_TX(uint8_t data) {
    // Check if the TX Buffer is ready for data
    if( EUSCI_A0->CTLW0 & EUSCI_A_IFG_TXIFG == EUSCI_A_IFG_TXIFG){
        EUSCI_A0->TXBUF = data; // Transmit the 8 bits
        return 1;   // successful transmissions
    }
    else return 0;  // failed transmission
}
