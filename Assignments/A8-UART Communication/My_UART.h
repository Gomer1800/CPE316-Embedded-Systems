/*
 * My_UART.h
 *
 *  Created on: Jun 2, 2020
 *      Author: Wulfkine
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#include "msp.h"
#include "My_Delays.h"

// Initializes UART according to Ch 24 in the TRM
void init_UART(void);

void UART_TX(uint8_t data);

#endif /* MY_UART_H_ */
