/*
 * keypad.c
 *
 *  Created on: Apr 21, 2020
 *      Author: Hayden Rinn
 */

#include <My_FuncGen.h>
#include <My_Keypad.h>
#include "My_Wavegen.h"

void setup_keypad(void) {
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // watchdog

    // set columns as GPIO (P4.4, P4.5, P4.6)
    P4->SEL1 &= ~COLBITS;
    P4->SEL0 &= ~COLBITS;

    //set columns as outputs
    P4->DIR |= COLBITS;

    //set all columns to output high
    P4->OUT |= COLBITS;

    // set rows as inputs with pulldown resistors (P4.0, P4.1, P4.2, P4.3)
    P4->DIR &= ~ROWBITS;
    P4->REN |= ROWBITS;
    P4->OUT &= ~ROWBITS;

    // setup and enable interrupts for input pins on low-to-high transition
    P4->IES &= ~ROWBITS;
    P4->IFG &= ~ROWBITS;
    P4->IE  |= ROWBITS;

    // enable port 4 on the NVIC
    NVIC->ISER[1] = 1 << ((PORT4_IRQn) & 0x1F);

    __enable_irq(); // enable global interrupts
}

void determine_key(uint8_t row) {
    char keypad_input;
    uint8_t col;
    uint8_t c = 1;
    for (col = BIT4; col < BIT7; col = col << 1) {
        P4->OUT &= ~COLBITS;
        P4->OUT |= col; // turn on one column
        delay_us(DELAY25MS); // button debounce
        if (check_row(row)) { // check if the row is still high
            keypad_input = get_key(c, row);  // the correct key is at (col, row)

            // Business Logic
            change_wave(keypad_input);
            set_cursor_LCD(lcd, 0, 1);
            write_char_LCD(lcd, keypad_input);
            set_cursor_LCD(lcd, 0, 1);
            break;
        }
        c++;

    }
    P4->OUT |= COLBITS; // Reset all column bits to output high
}

uint8_t check_row(uint8_t row) {
    return P4->IN & (1 << (row-1)); //checks to see if the bit row-1 is high in Port4
}

char get_key(uint8_t col, uint8_t row) {
    char keys[NCOL*NROW] = "123456789*0#";
    return keys[(row-1)*NCOL+(col-1)];
}

void PORT4_IRQHandler(void){
    if(P4->IFG & BIT0){
       determine_key(ROW1);
       P4->IFG &= ~BIT0;
    }
    if(P4->IFG & BIT1){
        determine_key(ROW2);
        P4->IFG &= ~BIT1;
    }
    if(P4->IFG & BIT2){
        determine_key(ROW3);
        P4->IFG &= ~BIT2;
    }
    if(P4->IFG & BIT3){
        determine_key(ROW4);
        P4->IFG &= ~BIT3;
    }
}
