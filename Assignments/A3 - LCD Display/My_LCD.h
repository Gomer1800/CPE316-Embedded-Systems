/*
 * My_LCD.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_LCD_H_
#define MY_LCD_H_

#include "msp.h"
#include  "STDINT.H"        // definition of uint32_t
#include <stdlib.h>

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct{
    uint8_t _rs_pin;        // LOW: command.  HIGH: character.
    uint8_t _rw_pin;        // LOW: write to LCD.  HIGH: read from LCD.
    uint8_t _enable_pin;    // activated by a HIGH pulse.
    uint8_t _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];

} LCD;

void* constructor_LCD(
        uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
        uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
        uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);

// void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);

// void destroy_LCD(void *lcd);

#endif /* MY_LCD_H_ */
