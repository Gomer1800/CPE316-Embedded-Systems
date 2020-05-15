#include "My_LCD.h"
/*
 * My_LCD.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Luis Gomez
 */

/******** constructor & destructor ********/

void* constructor_LCD(uint8_t fourbitmode, DIO_PORT_Even_Interruptable_Type *data_port)
{
    LCD *lcd;
    lcd = malloc(sizeof(LCD));

    lcd->_data_port     = data_port;

    lcd->_rs_pin        = &(P2->OUT);   // 2.4
    lcd->_rw_pin        = &(P2->OUT);   // 2.6
    lcd->_enable_pin    = &(P2->OUT);   // 2.7

    // assign control bits
    uint8_t i;

    if(fourbitmode){
        for(i=0; i<4; i++)
            lcd->_data_pins[i] = &(lcd->_data_port->OUT);
    }
    else{
        for(i=0; i<8; i++)
            lcd->_data_pins[i] = &(lcd->_data_port->OUT);
    }

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

    // Initialize pins once, instead of every time a character is drawn for speed reasons.

    // control bits
    pinMode(P2, 4, IO); // RS 2.4
    pinMode(P2, 6, IO); // RW 2.6
    pinMode(P2, 7, IO); // RE 2.7

    //  Data Bits
    uint8_t DATA_BIT;
    if(( ((LCD*)lcd)->_displayfunction & LCD_8BITMODE) == LCD_8BITMODE){
        for(DATA_BIT=0; DATA_BIT<8; DATA_BIT++){
            pinMode(((LCD*)lcd)->_data_port, DATA_BIT, IO);
        }
    }
    else{
        for(DATA_BIT=0; DATA_BIT<4; DATA_BIT++){
            pinMode(((LCD*)lcd)->_data_port, DATA_BIT, IO);
        }
    }

    // Change according to data sheet
    __delay_cycles(DELAY50MS);

    // Now we pull both RS and R/W low to begin commands
    *(((LCD*)lcd)->_rs_pin)     &= ~BIT4;
    *(((LCD*)lcd)->_rw_pin)     &= ~BIT6;
    *(((LCD*)lcd)->_enable_pin) &= ~BIT7;

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

    // turn the display on with cursor & blinking default
    ((LCD*)lcd)->_displaycontrol = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
    display(lcd);

    // clear it off
    clear_LCD(lcd);

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



uint8_t get_data_pin_bit(uint8_t bit_index)
{
    uint8_t DATA_BIT = 0;
    DATA_BIT = 0x01 << bit_index;
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

void clear_LCD(void *lcd)
{
    command(lcd, LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    __delay_cycles(DELAY2MS);    // this command takes a long time!
}

void home_LCD(void *lcd)
{
  command(lcd, LCD_RETURNHOME);  // set cursor position to zero
  __delay_cycles(DELAY2MS);  // this command takes a long time!
}

void set_cursor_LCD(void *lcd, uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(((LCD*)lcd)->_row_offsets) / sizeof( *(((LCD*)lcd)->_row_offsets) );
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= ((LCD*)lcd)->_numlines ) {
    row = ((LCD*)lcd)->_numlines - 1;    // we count rows starting w/0
  }

  command(lcd, LCD_SETDDRAMADDR | (col + (((LCD*)lcd)->_row_offsets)[row]));
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

void scrollDisplayLeft  (void *lcd)
{
    command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight (void *lcd)
{
    command(lcd, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void leftToRight        (void *lcd)
{
    ((LCD*)lcd)->_displaymode |= LCD_ENTRYLEFT;
    command(lcd, LCD_ENTRYMODESET | ((LCD*)lcd)->_displaymode);
}

void rightToLeft        (void *lcd)
{
    ((LCD*)lcd)->_displaymode &= ~LCD_ENTRYLEFT;
    command(lcd, LCD_ENTRYMODESET | ((LCD*)lcd)->_displaymode);
}

void autoscroll         (void *lcd)
{
    ((LCD*)lcd)->_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(lcd, LCD_ENTRYMODESET | ((LCD*)lcd)->_displaymode);
}

void noAutoscroll       (void *lcd)
{
    ((LCD*)lcd)->_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(lcd, LCD_ENTRYMODESET | ((LCD*)lcd)->_displaymode);
}

void write_char_LCD(void *lcd, char my_char)
{
    write(lcd , get_char(my_char) );
}

void write_string_LCD(void *lcd, char *my_string)
{
    uint32_t i = 0;

    while(my_string[i] != '\0')
    {
        write_char_LCD(lcd, my_string[i]);
        i = i + 1;
    }
}

void display_menu_LCD(void *lcd){
    clear_LCD(lcd);
    delay_us(DELAY2MS); // initial delay to display
    write_string_LCD(lcd, LOCKED);
    set_cursor_LCD(lcd, 0, 1);
    write_string_LCD(lcd, ENTER_KEY);
    delay_us(DELAY2MS);
}

void moveTextLeft(int maxCount){
    int positionCounter;
    for(positionCounter = 0; positionCounter < maxCount; positionCounter++) {
        // scroll one position left:
        scrollDisplayLeft(lcd);
        // wait a bit:
        __delay_cycles(DELAY500MS);
    }
}

void moveTextRight(int maxCount){
    int positionCounter;
    for(positionCounter = 0; positionCounter < maxCount; positionCounter++) {
        // scroll one position right:
        scrollDisplayRight(lcd);
        // wait a bit:
        __delay_cycles(DELAY500MS);
    }
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
        case ' ':
            byte = 0x20;
            break;
        case '!':
            byte = 0x21;
            break;
        case '0':               // Decimal Numbers
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
        case 'A':               // alphabet
        case 'a':
            byte = 0x41;
            break;
        case 'B':
        case 'b':
            byte = 0x42;
            break;
        case 'C':
        case 'c':
            byte = 0x43;
            break;
        case 'D':
        case 'd':
            byte = 0x44;
            break;
        case 'E':
        case 'e':
            byte = 0x45;
            break;
        case 'F':
        case 'f':
            byte = 0x46;
            break;
        case 'G':
        case 'g':
            byte = 0x47;
            break;
        case 'H':
        case 'h':
            byte = 0x48;
            break;
        case 'I':
        case 'i':
            byte = 0x49;
            break;
        case 'J':
        case 'j':
            byte = 0x4A;
            break;
        case 'K':
        case 'k':
            byte = 0x4B;
            break;
        case 'L':
        case 'l':
            byte = 0x4C;
            break;
        case 'M':
        case 'm':
            byte = 0x4D;
            break;
        case 'N':
        case 'n':
            byte = 0x4E;
            break;
        case 'O':
        case 'o':
            byte = 0x4F;
            break;
        case 'P':
        case 'p':
            byte = 0x50;
            break;
        case 'Q':
        case 'q':
            byte = 0x51;
            break;
        case 'R':
        case 'r':
            byte = 0x52;
            break;
        case 'S':
        case 's':
            byte = 0x53;
            break;
        case 'T':
        case 't':
            byte = 0x54;
            break;
        case 'U':
        case 'u':
            byte = 0x55;
            break;
        case 'V':
        case 'v':
            byte = 0x56;
            break;
        case 'W':
        case 'w':
            byte = 0x57;
            break;
        case 'X':
        case 'x':
            byte = 0x58;
            break;
        case 'Y':
        case 'y':
            byte = 0x59;
            break;
        case 'Z':
        case 'z':
            byte = 0x5A;
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
            *(((LCD*)lcd)->_rs_pin) &= ~BIT4;
            break;
        default:
            // REGISTER
            *(((LCD*)lcd)->_rs_pin) |= BIT4;
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
    *(((LCD*)lcd)->_enable_pin) &= ~BIT7;
    __delay_cycles(1);

    *(((LCD*)lcd)->_enable_pin) |= BIT7;
    __delay_cycles(2);      // enable pulse must be >450ns

    *(((LCD*)lcd)->_enable_pin) &= ~BIT7;
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
