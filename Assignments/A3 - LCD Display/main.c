#include "msp.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"

#include "STDINT.H"         // definition of uint32_t

#define LCD_COLUMNS     ((uint8_t)0x10) // 16   columns
#define LCD_ROWS        ((uint8_t)0x02) // 2    rows
/**
 * main.c
 *
 */

void main(void)
{
    // Wait for LCD to power up to at least 4.5 V
    __delay_cycles(DELAY50MS);

    /***** LCD INITIALIZATION *****/
    void *lcd;
    lcd = constructor_LCD(BYTE, P4);
    begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);
    clear_LCD(lcd);

    /***** WRITING TO LCD *****/
    char* my_string1 = "Hello World!";
    char* my_string2 = "Assignment 3";
    __delay_cycles(DELAY2MS);           // needed while string is loaded

    write_string_LCD(lcd, my_string1);

    set_cursor_LCD(lcd, 0, 1);
    write_string_LCD(lcd, my_string2);

    /***** SCROLL *****/
    int positionCounter;
    for(positionCounter = 0; positionCounter < 13; positionCounter++) {
      // scroll one position left:
      scrollDisplayLeft(lcd);
      // wait a bit:
      __delay_cycles(DELAY500MS);
    }

    // scroll 29 positions (string length + display length) to the right
    // to move it offscreen right:
    for (positionCounter = 0; positionCounter < 29; positionCounter++) {
      // scroll one position right:
      scrollDisplayRight(lcd);
      // wait a bit:
      __delay_cycles(DELAY500MS);
    }

    // scroll 16 positions (display length + string length) to the left
    // to move it back to center:
    for (positionCounter = 0; positionCounter < 16; positionCounter++) {
      // scroll one position left:
      scrollDisplayLeft(lcd);
      // wait a bit:
      __delay_cycles(DELAY500MS);
    }

    destroy_LCD(lcd);

    /***** LED BLINK *****/
    setup_RED_LED();

    P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
    __delay_cycles(DELAY100MS);
}
