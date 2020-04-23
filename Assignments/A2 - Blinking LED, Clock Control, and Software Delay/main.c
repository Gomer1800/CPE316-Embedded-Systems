#include "msp.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"

/**
 * main.c
 */

void main(void)
{
    setup_DCO(FREQ_3MHZ);

    setup_MCLK_to_DCO();

    // Configure Pin 4.3 to output MCLK, see MDS pin function tables for details

    /* configure P4.3 as simple I/O
    P4->SEL1 &= ~BIT3;
    P4->SEL0 &= ~BIT3;
    */

    /* configure P4.3 to output MCLK */
    P4->SEL1 &= ~BIT3;
    P4->SEL0 |= BIT3;


    P4->DIR |= BIT3;           /* P4.3 set as output pin */

    // Blink RED LED to show code is running
    setup_RED_LED();

    while(1) {

        // P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY40ms);

        // P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY40ms);
    }
}
