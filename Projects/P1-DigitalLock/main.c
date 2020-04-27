#include "msp.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"
#include "keypad.h"
#include "lock.h"

//#include "STDINT.H"         // definition of uint32_t

#define LCD_COLUMNS     ((uint8_t)0x10) // 16   columns
#define LCD_ROWS        ((uint8_t)0x02) // 2    rows
/**
 * main.c
 *
 */
void *lcd;
char savedPin[ARR_LENGTH];
uint32_t count;
enum BOOL isLocked;

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
    isLocked = true;
    setup_keypad();
    while(1){
        __sleep();
    }
}

void lock_input(uint8_t output) {
    uint32_t index = count % 4;

    if(output == '*'){
        display_menu_LCD();
        isLocked = true;
        count = 0;
    }
    else{
        savedPin[index] = output;
        if((output != '?') && ((count/PIN_LENGTH) != 1) && isLocked){ //if the count == 3 then call compare function
            write_char_LCD(lcd, output);
            count++;
        }
        if(count == 4){
            savedPin[count] = '\0';
            if(strcmp(savedPin, CORRECT_PIN) == 0){
                count = 0;
                clear_LCD(lcd);
                delay_us(DELAY2MS);
                write_string_LCD(lcd, UNLOCKED);
                moveTextLeft(13);
                moveTextRight(29);
                moveTextLeft(16);

               //end of scroll
                isLocked = false;
            }
            else{
                display_menu_LCD();
                count = 0;
            }
        }
    }
}
