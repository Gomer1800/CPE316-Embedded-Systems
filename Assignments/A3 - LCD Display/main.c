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
    __delay_cycles(DELAY50MS);

    // construct lcd
    void *lcd;
    lcd = constructor_LCD(BYTE);

    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 1, LCD_5x8DOTS);

    clear_LCD(lcd);

    setup_RED_LED();

    // test string for output
    char* my_string = "1234567890#*";

    write_string_LCD(lcd, my_string);

    uint8_t i;
    for(i = 0x00; i<0x0D; i++)
    {
        // P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        __delay_cycles(DELAY40MS);

        // write(lcd , get_char(my_string[i]) );

        // P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        __delay_cycles(DELAY40MS);
    }

    __delay_cycles(DELAY10S);

    home_LCD(lcd);

    destroy_LCD(lcd);
}
