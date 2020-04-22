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
    lcd = constructor_LCD(0);

    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 1, LCD_5x8DOTS);

    clear(lcd);
    // noDisplay(lcd);

    // send(lcd , 0x55, REGISTER);

    setup_RED_LED();

    uint8_t i;
    for(i = 0x00; i<0xFF; i++)
    //while(1)
    {
        // P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY40ms);

        write(lcd , i);
        // noDisplay(lcd);

        // P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY40ms);
    }

    clear(lcd);

    destroy_LCD(lcd);
}
