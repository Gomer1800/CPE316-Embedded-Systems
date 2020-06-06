#include "My_UART.h"
/*
 * My_UART.c
 *
 *  Created on: Jun 2, 2020
 *      Author: Luis Gomez
 *
 *  Important Pins
 *
 *  P1.2 - RX
 *  P1.3 - TX
 */

uint16_t RX_DATA;

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
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_PEN;      // Parity Disabled
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_MSB;      // LSB first
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SEVENBIT; // 8-bit data
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SPB;      // One stop bit
    EUSCI_A0->CTLW0 |=  EUSCI_A_CTLW0_MODE_0;   // UART mode
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SYNC;     // Asynchronous mode
    EUSCI_A0->CTLW0 |=  EUSCI_A_CTLW0_UCSSEL_2; // eUSCI_A clock source: SMCLK

    //clock prescaler setting of the baud-rate generator
    EUSCI_A0->BRW = 26;         // 3,000,000 / 115200 = 26
    //EUSCI_A0->BRW = 312;            // 3,000,000 / 9600 = 312.5

    EUSCI_A0->MCTLW = 0;            // disable oversampling

    // 3.
    P1->SEL0 |=  0x0C;              // P1.3, P1.2 for UART: UCA0RXD & UCA0TXD
    P1->SEL1 &= ~0x0C;

    // 4.
    EUSCI_A0->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // software reset disable
}

void UART_TX(uint8_t data) {
    // Wait until TX Buffer is ready for data
    while(!(EUSCI_A0->IFG & EUSCI_A__TXIE)) { asm ("NOP"); }
    EUSCI_A0->TXBUF = data; // Transmit the 8 bits
    delay_us(2);
}

