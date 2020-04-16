#include "msp.h"
#include "My_DCO.h"
#include "My_LEDS.h"

/**
 * main.c
 */
void delay_us(uint32_t);

void main(void)
{
    setup_DCO(FREQ_3MHZ);

    setup_MCLK_to_DCO();

    // Configure Pin 4.3 to output MCLK, see MDS pin function tables for details
    P4->SEL1 &= ~BIT3;         /* configure P4.3 as simple I/O */

    P4->SEL0 |= BIT3;         /* configure P4.3 as simple I/O*/

    P4->DIR |= BIT3;           /* P4.3 set as output pin */

    // Blink RED LED to show code is running
    setup_RED_LED();

    while(1) {
        uint32_t DELAY10S = 10000000; // 10 seconds

        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY10S);

        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY10S);
    }
}

void delay_us(uint32_t microseconds) {

    // Determine current DCO configuration
    uint32_t FREQ =  get_DCO_Frequency();

    // Calculate number of iterations for desired delay
    uint32_t num_loops;

    // TODO Luis: Figure out how an exact expression for number of us per loop, current one overshoots
    switch(FREQ)
    {
        case FREQ_3MHZ:
            num_loops = ((microseconds*10000) * 2745)/10000;
            break;
        case FREQ_6MHZ:
            num_loops = ((microseconds*10000) * 4955)/10000;
            break;
        case FREQ_12MHZ:
            num_loops = ((microseconds*10000) * 9862)/10000;
            break;
        case FREQ_24MHZ:
            num_loops = ((microseconds*10000) * 19798)/10000;
            break;
        default:
            num_loops = ((microseconds*10000) * 1374)/10000;
            break;
    }

    // Delay loop
    uint32_t i;
    for (i=0; i<num_loops; i++)
    {
        asm ("NOP");
    }
}
