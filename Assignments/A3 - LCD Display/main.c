#include "msp.h"
#include "My_LCD.h"

/**
 * main.c
 *
 */

void main(void)
{
	// WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // construct lcd
    LCD *lcd = constructor_LCD(0);

    // set up the LCD's number of columns and rows:
    begin_LCD(lcd, 16, 2, LCD_5x8DOTS);

    clear(lcd);

    write(lcd , 0x33);

    __delay_cycles(10000000);

    destroy_LCD(lcd);
}
