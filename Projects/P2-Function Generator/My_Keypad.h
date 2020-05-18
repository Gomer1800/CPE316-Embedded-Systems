/*
 * keypad.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Hayden Rinn
 */

#ifndef MY_KEYPAD_H_
#define MY_KEYPAD_H_

// IMPORTS
#include "msp.h"
#include <stdio.h>
#include "My_LCD.h"
#include "My_Delays.h"
#include <string.h>

// MACROS
#define COLBITS ((uint8_t)0x70)
#define ROWBITS ((uint8_t)0x0F)
#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4
#define NCOL 3
#define NROW 4

// FUNCTION DEFINTIONS
void setup_keypad(void);
void determine_key(uint8_t row);
uint8_t check_row(uint8_t row);
char get_key(uint8_t col, uint8_t row);

#endif /* KEYPAD_H_ */
