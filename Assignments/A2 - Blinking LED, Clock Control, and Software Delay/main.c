#include "msp.h"
#include "My_DCO.h"
#include "My_LEDS.h"

/**
 * main.c
 */

// Delay constants
#define DELAY40ms               ((uint32_t)40000) // 40 milliseconds
#define DELAY40us               ((uint32_t)40)    // 40 microseconds
#define FLOATING_POINT_DIVISOR  ((uint32_t)10000) // used to avoid floating point conversion errors

// Slopes from our data trend-lines on excel
#define 1_5MHZ_SLOPE            ((uint32_t)1374)
#define 3MHZ_SLOPE              ((uint32_t)2745)
#define 5MHZ_SLOPE              ((uint32_t)4955)
#define 12MHZ_SLOPE             ((uint32_t)9862)
#define 24MHZ_SLOPE             ((uint32_t)19798)

void delay_us(uint32_t);

void main(void)
{
    setup_DCO(FREQ_24MHZ);

    setup_MCLK_to_DCO();

    // Configure Pin 4.3 to output MCLK, see MDS pin function tables for details
    P4->SEL1 &= ~BIT3;         /* configure P4.3 as simple I/O */

    P4->SEL0 &= ~BIT3;         /* configure P4.3 as simple I/O*/

    P4->DIR |= BIT3;           /* P4.3 set as output pin */

    // Blink RED LED to show code is running
    setup_RED_LED();

    while(1) {

        P4->OUT = ~P4->OUT;
        P1->OUT |= (BIT0);          /* turn on  P1.0 red LED */
        delay_us(DELAY40ms);

        P4->OUT = ~P4->OUT;
        P1->OUT &= ~(BIT0);         /* turn off P1.0 red LED */
        delay_us(DELAY40ms);
    }
}

void delay_us(uint32_t microseconds) {

    // Determine current DCO configuration
    uint32_t FREQ =  get_DCO_Frequency();

    // Calculate number of iterations for desired delay
    uint32_t num_loops;

    // Note: we divide by 10000 to get rid of floating point errors
    // Scalars we multiply by are from our trendlines on excel
    switch(FREQ)
    {
        case FREQ_3MHZ:
            num_loops = ((microseconds) * 3MHZ_SLOPE)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_6MHZ:
            num_loops = ((microseconds) * 6MHZ_SLOPE)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_12MHZ:
            num_loops = ((microseconds) * 12MHZ_SLOPE)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_24MHZ:
            num_loops = ((microseconds) * 24MHZ_SLOPE)/FLOATING_POINT_DIVISOR;
            break;
        default:
            num_loops = ((microseconds) * 1_5MHZ_SLOPE)/FLOATING_POINT_DIVISOR;
            break;
    }

    // Delay loop
    uint32_t i;
    for (i=0; i<num_loops; i++)
    {
        asm ("NOP");
    }
}
