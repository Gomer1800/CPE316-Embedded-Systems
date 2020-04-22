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
    lcd->_rs_pin        = &(P6->OUT);         // 6.7
    lcd->_rw_pin        = &(P1->OUT);         // 1.6
    lcd->_enable_pin    = &(P6->OUT);         // 6.6

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

void destroy_LCD(void *lcd)
{
    free(lcd);
}

void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) {
      (((LCD*)lcd)->_displayfunction) |= LCD_2LINE;
    }

    ((LCD*)lcd)->_numlines = lines;

    setRowOffsets(lcd, 0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // Do these once, instead of every time a character is drawn for speed reasons.
    enable_LCD_Output_Pins();

    // Change according to data sheet
    __delay_cycles(DELAY50us);

    // Now we pull both RS and R/W low to begin commands
    *(((LCD*)lcd)->_rs_pin) &= ~BIT7;
    *(((LCD*)lcd)->_rw_pin) &= ~BIT6;
    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;

    //put the LCD into 4 bit or 8 bit mode
    if(!( (((LCD*)lcd)->_displayfunction) & LCD_8BITMODE ))
    {

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(lcd, 0x03);
        __delay_cycles(4500);   // wait min 4.1ms

        // second try
        write4bits(lcd, 0x03);
        __delay_cycles(4500);   // wait min 4.1ms

        // third go!
        write4bits(lcd, 0x03);
        __delay_cycles(150);

        // finally, set to 4-bit interface
        write4bits(lcd, 0x02);
    }
    else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
        __delay_cycles(4500);  // wait more than 4.1ms

        // second try
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
        __delay_cycles(150);

        // third go
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
    }

    // finally, set # lines, font size, etc.
    command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );

    // turn the display on with no cursor or blinking default
    (((LCD*)lcd)->_displaycontrol) = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display(lcd);

    // clear it off
    clear(lcd);

    // Initialize to default text direction (for romance languages)
    (((LCD*)lcd)->_displaymode) = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    command(lcd, LCD_ENTRYMODESET | (((LCD*)lcd)->_displaymode) );
}

void setRowOffsets(void *lcd, int row0, int row1, int row2, int row3)
{
  ((LCD*)lcd)->_row_offsets[0] = row0;
  ((LCD*)lcd)->_row_offsets[1] = row1;
  ((LCD*)lcd)->_row_offsets[2] = row2;
  ((LCD*)lcd)->_row_offsets[3] = row3;
}

void enable_LCD_Output_Pins(void)
{
    uint16_t DATA_BIT = 0;

    /* control bits                 */
    P6->DIR  |=  BIT7;  // RS 6.7
    P6->SEL1 &= ~BIT7;
    P6->SEL0 &= ~BIT7;

    P1->DIR  |=  BIT6;  // RW 1.6
    P1->SEL1 &= ~BIT6;
    P1->SEL0 &= ~BIT6;

    P6->DIR  |=  BIT6;  // RE 6.6
    P6->SEL1 &= ~BIT6;
    P6->SEL0 &= ~BIT6;

    /* Bits[0:3]                    */
    DATA_BIT = get_data_pin_bit(0);
    P3->DIR  |=  DATA_BIT;  // bit0 3.7
    P3->SEL1 &= ~DATA_BIT;
    P3->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(1);
    P3->DIR  |=  DATA_BIT;  // bit1 3.5
    P3->SEL1 &= ~DATA_BIT;
    P3->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(2);
    P5->DIR  |=  DATA_BIT;  // bit2 5.1
    P5->SEL1 &= ~DATA_BIT;
    P5->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(3);
    P2->DIR  |=  DATA_BIT;  // bit3 2.3
    P2->SEL1 &= ~DATA_BIT;
    P2->SEL0 &= ~DATA_BIT;

    // Bits[4:7]
    DATA_BIT = get_data_pin_bit(4);
    P3->DIR  |=  DATA_BIT;  // bit4 3.6
    P3->SEL1 &= ~DATA_BIT;
    P3->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(5);
    P5->DIR  |=  DATA_BIT;  // bit5 5.2
    P5->SEL1 &= ~DATA_BIT;
    P5->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(6);
    P5->DIR  |=  DATA_BIT;  // bit6 5.0
    P5->SEL1 &= ~DATA_BIT;
    P5->SEL0 &= ~DATA_BIT;

    DATA_BIT = get_data_pin_bit(7);
    P1->DIR  |=  DATA_BIT;  // bit7 1.7
    P1->SEL1 &= ~DATA_BIT;
    P1->SEL0 &= ~DATA_BIT;
}

uint16_t get_data_pin_bit(uint8_t bit_index)
{
    uint16_t DATA_BIT = 0;
    switch(bit_index)
    {
        case 0:
            DATA_BIT = BIT7;
            break;
        case 1:
            DATA_BIT = BIT5;
            break;
        case 2:
            DATA_BIT = BIT1;
            break;
        case 3:
            DATA_BIT = BIT3;
            break;
        case 4:
            DATA_BIT = BIT6;
            break;
        case 5:
            DATA_BIT = BIT2;
            break;
        case 6:
            DATA_BIT = BIT0;
            break;
        case 7:
            DATA_BIT = BIT7;
            break;
        default:
            DATA_BIT = 0;
            break;
    }
    return DATA_BIT;
}

/********** high level commands, for the user! */

void clear(void *lcd)
{
    command(lcd, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    __delay_cycles(2000);    // this command takes a long time!
}

// Turn the display on/off (quickly)
void noDisplay(void *lcd)
{
    (((LCD*)lcd)->_displaycontrol) &= ~LCD_DISPLAYON;
    command( lcd, LCD_DISPLAYCONTROL | (((LCD*)lcd)->_displaycontrol) );
}
void display(void *lcd)
{
    (((LCD*)lcd)->_displaycontrol) |= LCD_DISPLAYON;
    command( lcd, LCD_DISPLAYCONTROL | (((LCD*)lcd)->_displaycontrol) );
}

/*********** mid level commands, for sending data/cmds */

inline void command(void *lcd, uint8_t value)
{
    send(lcd, value, LOW);
}

inline size_t write(void *lcd, uint8_t value)
{
    send(lcd, value, HIGH);
    return 1; // assume success
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send(void *lcd, uint8_t value, uint8_t mode) {

    // write to digital pins
    *(((LCD*)lcd)->_rs_pin) &= (BIT7 & mode);
    *(((LCD*)lcd)->_rw_pin) &= (BIT6 & mode);

    if( (((LCD*)lcd)->_displayfunction) & LCD_8BITMODE )
    {
        write8bits(lcd, value);
    }
    else
    {
        write4bits(lcd, value>>4);
        write4bits(lcd, value);
  }
}

void pulseEnable(void *lcd)
{
    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;
    __delay_cycles(1);

    *(((LCD*)lcd)->_enable_pin) |= BIT6;
    __delay_cycles(1);      // enable pulse must be >450ns

    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;
    __delay_cycles(100);    // commands need > 37us to settle
}

void write4bits(void *lcd, uint8_t value)
{
    uint8_t i;
    for(i = 0; i < 4; i++)
    {
        *(((LCD*)lcd)->_data_pins[i]) |= ((value >> i) & 0x01);
    }

    pulseEnable(lcd);
}

void write8bits(void *lcd, uint8_t value)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        *(((LCD*)lcd)->_data_pins[i]) |= ((value >> i) & 0x01);
    }

    pulseEnable(lcd);
}
