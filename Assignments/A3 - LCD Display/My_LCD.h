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

// flags for constructor_LCD
#define NIBBLE_ON   ((uint8_t)0x01)
#define NIBBLE_OFF  ((uint8_t)0x00)

// flags for function set
#define LCD_8BITMODE    ((uint8_t)0x10)
#define LCD_4BITMODE    ((uint8_t)0x00)
#define LCD_2LINE       ((uint8_t)0x08)
#define LCD_1LINE       ((uint8_t)0x00)
#define LCD_5x10DOTS    ((uint8_t)0x04)
#define LCD_5x8DOTS     ((uint8_t)0x00)

typedef struct{
    volatile uint8_t* _rs_pin;        // LOW: command.  HIGH: character.
    volatile uint8_t* _rw_pin;        // LOW: write to LCD.  HIGH: read from LCD.
    // uint8_t* _enable_pin;    // activated by a HIGH pulse.
    volatile uint8_t* _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];

} LCD;

void* constructor_LCD(uint8_t fourbitmode);

void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);

void setRowOffsets(void *lcd, int row1, int row2, int row3, int row4);

// void destroy_LCD(void *lcd);

#endif /* MY_LCD_H_ */
