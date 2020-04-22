#include "My_LCD.h"
/*
 * My_LCD.c
 *
 *  Created on: Apr 21, 2020
 *
 *      Adapted from the Arduino Liquid Crystal library
 *      & ported to C by Luis Gomez
 */

void* constructor_LCD(uint8_t fourbitmode)
{
    LCD *lcd;
    lcd = malloc(sizeof(LCD));

    // control bits
    lcd->_rs_pin = &(P6->OUT);         // 6.7
    lcd->_rw_pin = &(P1->OUT);         // 1.6
    // lcd->_enable_pin = ???

    // Bits[0:3]
    lcd->_data_pins[0] = &(P3->OUT);   // 3.7
    lcd->_data_pins[1] = &(P3->OUT);   // 3.5
    lcd->_data_pins[2] = &(P5->OUT);   // 5.1
    lcd->_data_pins[3] = &(P2->OUT);   // 2.3

    // Bits[4:7]
    lcd->_data_pins[4] = &(P3->OUT);   // 3.6
    lcd->_data_pins[5] = &(P5->OUT);   // 5.2
    lcd->_data_pins[6] = &(P5->OUT);   // 5.0
    lcd->_data_pins[7] = &(P1->OUT);   // 1.7

    if (fourbitmode)
        lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        lcd->_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    return lcd;
}

void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) {
      ((LCD*)lcd)->_displayfunction |= LCD_2LINE;
    }

    ((LCD*)lcd)->_numlines = lines;

    setRowOffsets(lcd, 0x00, 0x40, 0x00 + cols, 0x40 + cols);
}

void setRowOffsets(void *lcd, int row0, int row1, int row2, int row3)
{
  ((LCD*)lcd)->_row_offsets[0] = row0;
  ((LCD*)lcd)->_row_offsets[1] = row1;
  ((LCD*)lcd)->_row_offsets[2] = row2;
  ((LCD*)lcd)->_row_offsets[3] = row3;
}
