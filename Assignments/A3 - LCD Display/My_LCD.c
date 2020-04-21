#include "My_LCD.h"
/*
 * My_LCD.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Luis Gomez
 */
#define NIBBLE_ON   ((uint8_t)1)
#define NIBBLE_OFF  ((uint8_t)0)

void* constructor_LCD(
        uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
        uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
        uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    LCD *lcd;
    lcd = malloc(sizeof(LCD));

    lcd->_rs_pin = rs;
    lcd->_rw_pin = rw;
    lcd->_enable_pin = enable;

    lcd->_data_pins[0] = d0;
    lcd->_data_pins[1] = d1;
    lcd->_data_pins[2] = d2;
    lcd->_data_pins[3] = d3;
    lcd->_data_pins[4] = d4;
    lcd->_data_pins[5] = d5;
    lcd->_data_pins[6] = d6;
    lcd->_data_pins[7] = d7;

    if (fourbitmode)
        lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        lcd->_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    return lcd;
}

/*
void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize)
{
}
*/
