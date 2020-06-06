#include "msp.h"
#include "My_UART.h"
#include "My_LCD.h"
#include "My_Delays.h"
/**
 * main.c
 */

extern uint16_t RX_DATA;
void *lcd;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //***** LCD INITIALIZATION *****
	delay_us(DELAY50MS);
    lcd = constructor_LCD(BYTE, P6);
    begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);

    clear_LCD(lcd);

    //***** UART INITIALIZATION *****
    RX_DATA = 'C';
    init_UART();

    while (1) {
        //UART_TX('Y');
        //UART_TX('e');
        //UART_TX('s');
        //UART_TX(0x1B);
        //UART_TX('E');
        //clear_LCD(lcd);
        UART_TX(RX_DATA);
        set_cursor_LCD(lcd, 0, 0);
        write_char_LCD(lcd, RX_DATA);
        delay_us(DELAY50MS);
    }
}
