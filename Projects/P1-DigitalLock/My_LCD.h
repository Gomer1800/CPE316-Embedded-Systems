/*
 * My_LCD.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Luis Gomez
 */

#ifndef MY_LCD_H_
#define MY_LCD_H_

#include "msp.h"
//#include "STDINT.H"         // definition of uint32_t
#include <stdlib.h>         // definition of size_t

#include "My_Pins.h"

// flags for constructor_digital I/0
#define NIBBLE      ((uint8_t)0xFF)
#define BYTE        ((uint8_t)0x00)
#define REGISTER    ((uint8_t)0xFF)
#define COMMAND     ((uint8_t)0x00)

// flags for constructor_LCD
#define NIBBLE_ON   ((uint8_t)0x01)
#define NIBBLE_OFF  ((uint8_t)0x00)

// commands
#define LCD_CLEARDISPLAY    0x01
#define LCD_RETURNHOME      0x02
#define LCD_ENTRYMODESET    0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON   0x04
#define LCD_DISPLAYOFF  0x00
#define LCD_CURSORON    0x02
#define LCD_CURSOROFF   0x00
#define LCD_BLINKON     0x01
#define LCD_BLINKOFF    0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
//#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE    ((uint8_t)0x10)
#define LCD_4BITMODE    ((uint8_t)0x00)
#define LCD_2LINE       ((uint8_t)0x08)
#define LCD_1LINE       ((uint8_t)0x00)
#define LCD_5x11DOTS    ((uint8_t)0x04)
#define LCD_5x8DOTS     ((uint8_t)0x00)

//Define macros for menu
#define LOCKED          ((char *)("LOCKED"))
#define ENTER_KEY       ((char *)("ENTER KEY"))
#define UNLOCKED        ((char *)("UNLOCKED"))

//Global pointer for the LCD used in main and keypad
extern void *lcd;

/******** LCD Struct Declaration ********/

typedef struct {
    DIO_PORT_Even_Interruptable_Type *_data_port;

    volatile uint8_t* _rs_pin;        // LOW: command.  HIGH: character.
    volatile uint8_t* _rw_pin;        // LOW: write to LCD.  HIGH: read from LCD.
    volatile uint8_t* _enable_pin;    // activated by a HIGH pulse.

    volatile uint8_t* _data_pins[8];

    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _numlines;
    uint8_t _row_offsets[4];

} LCD;

/******** constructor & destructor ********/

void*   constructor_LCD (uint8_t fourbitmode, DIO_PORT_Even_Interruptable_Type *port);
void    destroy_LCD     (void *lcd);

/******** setup ********/

void begin_LCD          (void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize);
void setRowOffsets      (void *lcd, uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4);

uint8_t get_data_pin_bit(uint8_t);
uint8_t get_shift_amount(uint8_t);

/********** high level commands, for the user! */

void clear_LCD          (void *lcd);
void home_LCD           (void *lcd);
void set_cursor_LCD(void *, uint8_t, uint8_t);

void noDisplay          (void *lcd);
void display            (void *lcd);

void scrollDisplayLeft  (void *lcd);
void scrollDisplayRight (void *lcd);
void leftToRight        (void *lcd);
void rightToLeft        (void *lcd);
void autoscroll         (void *lcd);
void noAutoscroll       (void *lcd);


void write_char_LCD     (void *lcd, char);
void write_string_LCD   (void *lcd, char*);
void display_menu_LCD   (void);
void moveTextLeft       (int maxCount);
void moveTextRight       (int maxCount);

/*********** mid level commands, for sending data/cmds */

inline void command (void *lcd, uint8_t value);
inline size_t write (void *lcd, uint8_t value);

uint8_t get_char    (char);

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send       (void *lcd, uint8_t value, uint8_t mode);
// TODO(Luis): nibble mode is bugged, fix this function or find the root cause of the bug
void write4bits (void *lcd, uint8_t);
void write8bits (void *lcd, uint8_t);
void pulseEnable(void *lcd);

#endif /* MY_LCD_H_ */
