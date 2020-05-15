/*
 * keypad.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Hayden Rinn
 */

#ifndef MY_KEYPAD_H_
#define MY_KEYPAD_H_

#include "msp.h"
#include <stdio.h>
#include "STDINT.H" // definition of uint32_t

#define COLBITS ((uint8_t)0x70)
#define ROWBITS ((uint8_t)0x0F)

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4

#define NCOL 3
#define NROW 4
#define CORRECT_PIN "1234"
#define PIN_LENGTH  4
#define ARR_LENGTH (PIN_LENGTH+1)

//global variables to save the pin and the digit count
extern char savedPin[5];
extern uint32_t count;

enum BOOL{false, true};

void setup_keypad(void);

void determine_key(uint8_t row);

uint8_t check_row(uint8_t row);

char get_key(uint8_t col, uint8_t row);

#endif /* MY_KEYPAD_H_ */
