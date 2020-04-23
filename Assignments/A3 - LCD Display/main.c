#include "msp.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"

/**
 * main.c
 *
 */

void main(void)
{
    setup_DCO(FREQ_3MHZ);
    setup_MCLK_to_DCO();

    // Wait for LCD to power up to at least 4.5 V
    __delay_cycles(DELAY50MS);

    // construct lcd
    void *lcd;
    lcd = constructor_LCD(BYTE);
    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 2, LCD_5x8DOTS);
    clear_LCD(lcd);

    setup_RED_LED();

    // test string for output
    char* my_string1 = "Hello World!";
    char* my_string2 = "Assignment 3";

    __delay_cycles(DELAY2MS);

    write_string_LCD(lcd, my_string1);

    set_cursor_LCD(lcd, 0, 1);

    write_string_LCD(lcd, my_string2);

    uint8_t i;
    for(i = 0x00; i<0x0D; i++)
    {
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        __delay_cycles(DELAY40MS);

        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        __delay_cycles(DELAY40MS);
    }

    destroy_LCD(lcd);
}
