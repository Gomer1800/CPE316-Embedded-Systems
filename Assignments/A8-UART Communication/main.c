#include "msp.h"
#include "My_UART.h"
#include "My_DCO.h"
#include "My_LCD.h"

/**
 * main.c
 */

/* delay milliseconds when system clock is at 3 MHz */
void delayMs(int n) {
    int i, j;
    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //***** LCD INITIALIZATION *****
    //__delay_cycles(DELAY50MS); // Wait for LCD to power up to at least 4.5 V
    //lcd = constructor_LCD(BYTE, P6);
    //begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);

    //***** UART INITIALIZATION *****
	init_UART();

    while (1) {
        UART_TX('Y');
        UART_TX('e');
        UART_TX('s');
        UART_TX(0x1B);
        UART_TX('E');
    }
}
