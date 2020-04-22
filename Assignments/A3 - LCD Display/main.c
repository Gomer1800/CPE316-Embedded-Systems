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
	// WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // construct lcd
    void *lcd;
    lcd = constructor_LCD(0);

    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 1, LCD_5x8DOTS);

    clear(lcd);
    // noDisplay(lcd);

    write(lcd , 0x33);

    setup_RED_LED();

    int i;
    for(i = 0; i<200; i++)
    {

        // P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY40ms);

        // P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY40ms);
    }

    destroy_LCD(lcd);
}
