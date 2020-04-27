#include "msp.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"
#include "keypad.h"

//#include "STDINT.H"         // definition of uint32_t

#define LCD_COLUMNS     ((uint8_t)0x10) // 16   columns
#define LCD_ROWS        ((uint8_t)0x02) // 2    rows
/**
 * main.c
 *
 */
void *lcd;
void main(void)
{
    setup_DCO(FREQ_3MHZ);
    setup_MCLK_to_DCO();

    // Wait for LCD to power up to at least 4.5 V
    __delay_cycles(DELAY50MS);

    /***** LCD INITIALIZATION *****/

    lcd = constructor_LCD(BYTE, P6);
    begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);
    display_menu_LCD();
    setup_RED_LED();
    setup_keypad();
    while(1){
        __sleep();
    }
}
