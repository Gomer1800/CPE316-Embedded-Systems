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

	__delay_cycles(DELAY50MS);

    // construct lcd
    void *lcd;
    lcd = constructor_LCD(BYTE);

    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 1, LCD_5x8DOTS);

    clear(lcd);

    setup_RED_LED();

    // test string for output
    char* string = "1234567890#*";

    uint8_t i;
    for(i = 0x00; i<0x0D; i++)
    {
        // P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY40MS);

        write(lcd , get_char(string[i]));

        // P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY40MS);
    }

    delay_us(15*DELAY1S);

    clear(lcd);

    destroy_LCD(lcd);
}
