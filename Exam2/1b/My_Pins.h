/*
 * My_Pins.h
 *
 *  Created on: Apr 24, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_PINS_H_
#define MY_PINS_H_

#include "msp.h"
#include "STDINT.H"         // definition of uint32_t

// flags for pinMode
#define IO      ((uint8_t)0x01)

void pinMode    (DIO_PORT_Even_Interruptable_Type *PORT, uint8_t PIN, uint8_t MODE);

#endif /* MY_PINS_H_ */
