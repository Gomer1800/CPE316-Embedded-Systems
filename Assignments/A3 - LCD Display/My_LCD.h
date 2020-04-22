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

// flags for constructor_digital I/0
#define HIGH        ((uint8_t)0xFF)
#define LOW         ((uint8_t)0x00)
#define REGISTER    ((uint8_t)0xFF)
#define COMMAND     ((uint8_t)0x00)

// Delay Constant
#define DELAY50MS   50000   // 50ms
#define DELAY4_1MS  4500    // 4.1ms
#define DELAY2MS    2000    // 2ms

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

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
    volatile uint8_t* _enable_pin;    // activated by a HIGH pulse.

    volatile uint8_t* _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    // uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];

} LCD;

/******** constructor & destructor ********/

void* constructor_LCD(uint8_t fourbitmode);

void destroy_LCD(void *lcd);

/******** setup ********/

void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);

void setRowOffsets(void *lcd, int row1, int row2, int row3, int row4);

void setup_LCD_Pins(void);

uint16_t get_data_pin_bit(uint8_t);

/********** high level commands, for the user! */

void clear(void *lcd);

void noDisplay(void *lcd);

void display(void *lcd);

/*********** mid level commands, for sending data/cmds */

inline void command(void *lcd, uint8_t value);

inline size_t write(void *lcd, uint8_t value);

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection

void send(void *lcd, uint8_t value, uint8_t mode);

void pulseEnable(void *lcd);

void write4bits(void *lcd, uint8_t);

void write8bits(void *lcd, uint8_t);

void destroy_LCD(void *lcd);

#endif /* MY_LCD_H_ */
