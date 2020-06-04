#include "msp.h"
#include "My_UART.h"
#include "My_DCO.h"
#include "My_LCD.h"

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//***** DCO INITIALIZATION *****
    setup_DCO(FREQ_3MHZ);

    //***** LCD INITIALIZATION *****
    //__delay_cycles(DELAY50MS); // Wait for LCD to power up to at least 4.5 V
    //lcd = constructor_LCD(BYTE, P6);
    //begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);

    //***** UART INITIALIZATION *****
	init_UART();

	while(1){
	    UART_TX(0x61); // print 'a'
	    __delay_cycles(DELAY500MS);
	    __delay_cycles(DELAY500MS);
	}
}
