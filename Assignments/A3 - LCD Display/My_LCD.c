#include "My_LCD.h"
#include "My_Delays.h"
/*
 * My_LCD.c
 *
 *  Created on: Apr 21, 2020
 *
 *      Adapted from the Arduino Liquid Crystal library
 *      & ported to C by Luis Gomez
 */

/******** constructor & destructor ********/

void* constructor_LCD(uint8_t fourbitmode)
{
    LCD *lcd;
    lcd = malloc(sizeof(LCD));

    // control bits
    lcd->_rs_pin        = &(P6->OUT);   // 6.7
    lcd->_rw_pin        = &(P1->OUT);   // 1.6
    lcd->_enable_pin    = &(P6->OUT);   // 6.6

    // Bits[0:3]
    lcd->_data_pins[0]  = &(P3->OUT);    // 3.7
    lcd->_data_pins[1]  = &(P3->OUT);    // 3.5
    lcd->_data_pins[2]  = &(P5->OUT);    // 5.1
    lcd->_data_pins[3]  = &(P2->OUT);    // 2.3

    // Bits[4:7]
    lcd->_data_pins[4]  = &(P3->OUT);    // 3.6
    lcd->_data_pins[5]  = &(P5->OUT);    // 5.2
    lcd->_data_pins[6]  = &(P5->OUT);    // 5.0
    lcd->_data_pins[7]  = &(P1->OUT);    // 1.7

    if (fourbitmode)
        lcd->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        lcd->_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    begin_LCD(lcd, 16, 1, LCD_5x8DOTS);

    return lcd;
}

void destroy_LCD(void *lcd)
{
    free(lcd);
}

/******** setup ********/

void begin_LCD(void *lcd, uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) {
      (((LCD*)lcd)->_displayfunction) |= LCD_2LINE;
    }

    ((LCD*)lcd)->_numlines = lines;

    setRowOffsets(lcd, 0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // Do these once, instead of every time a character is drawn for speed reasons.
    setup_LCD_Pins();

    // Change according to data sheet
    __delay_cycles(DELAY50MS);

    // Now we pull both RS and R/W low to begin commands
    *(((LCD*)lcd)->_rs_pin)     &= ~BIT7;
    *(((LCD*)lcd)->_rw_pin)     &= ~BIT6;
    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;

    // Wake up Procedure
    // put the LCD into 4 bit or 8 bit mode
    if(!( (((LCD*)lcd)->_displayfunction) & LCD_8BITMODE ))
    {

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(lcd, 0x03);
        __delay_cycles(DELAY100MS);

        // second try
        write4bits(lcd, 0x03);
        __delay_cycles(DELAY100MS);

        // third go!
        write4bits(lcd, 0x03);
        __delay_cycles(DELAY100MS);

        // finally, set to 4-bit interface
        write4bits(lcd, 0x03);
    }
    else
    {
        // Send function set command sequence
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
        __delay_cycles(DELAY100MS);

        // second try
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
        __delay_cycles(DELAY100MS);

        // third go
        command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );
    }

    // finally, set # lines, font size, etc.
    command(lcd, LCD_FUNCTIONSET | (((LCD*)lcd)->_displayfunction) );

    // turn the display on with no cursor or blinking default
    ((LCD*)lcd)->_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
    display(lcd);

    // clear it off
    clear(lcd);

    // Initialize to default text direction (for romance languages)
    (((LCD*)lcd)->_displaymode) = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    command(lcd, LCD_ENTRYMODESET | (((LCD*)lcd)->_displaymode) );
}

void setRowOffsets(void *lcd, uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3)
{
  ((LCD*)lcd)->_row_offsets[0] = row0;
  ((LCD*)lcd)->_row_offsets[1] = row1;
  ((LCD*)lcd)->_row_offsets[2] = row2;
  ((LCD*)lcd)->_row_offsets[3] = row3;
}

void setup_LCD_Pins(void)
{
    uint8_t DATA_BIT = 0;

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

uint8_t get_data_pin_bit(uint8_t bit_index)
{
    uint8_t DATA_BIT = 0;
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

uint8_t get_shift_amount(uint8_t bit_index)
{
    uint8_t shift = 0;
    switch(bit_index)
    {
        case BIT0:
            shift = 0;
            break;
        case BIT1:
            shift = 1;
            break;
        case BIT2:
            shift = 2;
            break;
        case BIT3:
            shift = 3;
            break;
        case BIT4:
            shift = 4;
            break;
        case BIT5:
            shift = 5;
            break;
        case BIT6:
            shift = 6;
            break;
        case BIT7:
            shift = 7;
            break;
        default:
            shift = 0;
            break;
    }
    return shift;
}

/********** high level commands, for the user! */

void clear(void *lcd)
{
    command(lcd, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    __delay_cycles(DELAY2MS);    // this command takes a long time!
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

void command(void *lcd, uint8_t value)
{
    send(lcd, value, COMMAND);
}

size_t write(void *lcd, uint8_t value)
{
    send(lcd, value, REGISTER);
    return 1; // assume success
}

uint8_t get_char(char my_char)
{
    //TODO(Luis): move this to a data structure

    uint8_t byte;
    switch(my_char)
    {
        case '0':
            byte = 0x30;
            break;
        case '1':
            byte = 0x31;
            break;
        case '2':
            byte = 0x32;
            break;
        case '3':
            byte = 0x33;
            break;
        case '4':
            byte = 0x34;
            break;
        case '5':
            byte = 0x35;
            break;
        case '6':
            byte = 0x36;
            break;
        case '7':
            byte = 0x37;
            break;
        case '8':
            byte = 0x38;
            break;
        case '9':
            byte = 0x39;
            break;
        case '#':
            byte = 0x23;
            break;
        case '*':
            byte = 0x2A;
            break;
        default: // '?' , notifies user with a ? that unknown char given
            byte = 0x3F;
            break;
    }
    return byte;
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void send(void *lcd, uint8_t value, uint8_t mode) {

    // write to digital pins
    switch(mode)
    {
        case COMMAND:
            *(((LCD*)lcd)->_rs_pin) &= ~BIT7;
            break;
        default:
            // REGISTER
            *(((LCD*)lcd)->_rs_pin) |= BIT7;
            break;
    }

    *(((LCD*)lcd)->_rw_pin) &= ~BIT6;

    if( (((LCD*)lcd)->_displayfunction) & LCD_8BITMODE )
    {
        write8bits(lcd, value);
    }
    else
    {
        // write4bits(lcd, value >> 4);
        write4bits(lcd, value);
    }
}

void pulseEnable(void *lcd)
{
    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;
    __delay_cycles(1);

    *(((LCD*)lcd)->_enable_pin) |= BIT6;
    __delay_cycles(2);      // enable pulse must be >450ns

    *(((LCD*)lcd)->_enable_pin) &= ~BIT6;
    __delay_cycles(1);    // commands need > 37us to settle
}

// TODO(Luis): This doesn't work, has a bug somewhere
void write4bits(void *lcd, uint8_t value)
{
    uint8_t have;
    uint8_t have_pin;
    uint8_t want;

    uint8_t i;

    // Feeds Least Significant Nibble from Byte
    for(i = 4; i < 8; i++)
    {
        // Retrieve the relevant bit
        have_pin = get_data_pin_bit(i);
        have = ((*(((LCD*)lcd)->_data_pins[i])) & have_pin) >> get_shift_amount(have_pin);
        want = (value >> (i-4)) & 0x01;

        if(have != want) // check, are relevant bits different?
        {
            if (have > 0)
            {   // Toggling from high to low
                *(((LCD*)lcd)->_data_pins[i]) &= ~have_pin;
            }
            else
            {   // Toggling from low to high
                *(((LCD*)lcd)->_data_pins[i]) |= have_pin;
            }
        }
    }

    pulseEnable(lcd);

    // Feeds Most Significant Nibble from Byte
    for(i = 4; i < 8; i++)
    {
        // Retrieve the relevant bit
        have_pin = get_data_pin_bit(i);
        have = ((*(((LCD*)lcd)->_data_pins[i])) & have_pin) >> get_shift_amount(have_pin);
        want = (value >> i) & 0x01;

        if(have != want) // check, are relevant bits different?
        {
            if (have > 0)
            {   // Toggling from high to low
                *(((LCD*)lcd)->_data_pins[i]) &= ~have_pin;
            }
            else
            {   // Toggling from low to high
                *(((LCD*)lcd)->_data_pins[i]) |= have_pin;
            }
        }
    }

    pulseEnable(lcd);
}

void write8bits(void *lcd, uint8_t value)
{
    uint8_t have;
    uint8_t have_pin;
    uint8_t want;

    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        // Retrieve the relevant bit
        have_pin = get_data_pin_bit(i);
        have = ((*(((LCD*)lcd)->_data_pins[i])) & have_pin) >> get_shift_amount(have_pin);
        want = (value >> i) & 0x01;

        if(have != want) // check, are relevant bits different?
        {
            if (have > 0)
            {   // Toggling from high to low
                *(((LCD*)lcd)->_data_pins[i]) &= ~have_pin;
            }
            else
            {   // Toggling from low to high
                *(((LCD*)lcd)->_data_pins[i]) |= have_pin;
            }
        }
    }

    pulseEnable(lcd);
}
