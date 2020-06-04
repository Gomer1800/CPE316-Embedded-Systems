/*
 * My_UART.h
 *
 *  Created on: Jun 2, 2020
 *      Author: Wulfkine
 */

#ifndef MY_UART_H_
#define MY_UART_H_

#include "msp.h"

// Initializes UART according to Ch 24 in the TRM
void init_UART(void);

uint8_t UART_TX(uint8_t data);

#endif /* MY_UART_H_ */