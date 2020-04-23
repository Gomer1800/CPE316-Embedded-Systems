#include "My_LEDS.h"
/*
 * My_LEDS.c
 *
 *  Created on: Apr 12, 2020
 *      Author: Luis Gomez
 */

void setup_RED_LED(void) {
    P1->SEL1 &= ~BIT0;          /* configure P1.0 as simple I/O */

    P1->SEL0 &= ~BIT0;          /* configure P1.0 as simple I/O*/

    P1->DIR |= BIT0;            /* P1.0 set as output pin */
    // P1->DIR |= (1<<0);
}

#define DELAY100MS 300000 * 1   // DELAY * 1 mS, multiply

void blink_RED_LED(void) {
    // User must provide correct delay cycles

    P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
    __delay_cycles(DELAY100MS);

    P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
    __delay_cycles(DELAY100MS);
}
