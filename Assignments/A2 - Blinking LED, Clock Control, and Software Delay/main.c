#include "msp.h"
#include "My_DCO.h"
#include "My_LEDS.h"

/**
 * main.c
 */

void delay_test_find_N(void);

//void delay_test_find_Min(void);

void main(void)
{
    // SET FREQUENCY HERE, SEE My_DCO.h FOR MACROS
    setup_DCO(FREQ_3MHZ);

    setup_MCLK_to_DCO();

    ////////// DELAY TEST SETUP START

    P4->SEL1 &= ~BIT3;         /* configure P4.3 as simple I/O */

    P4->SEL0 &= ~BIT3;         /* configure P4.3 as simple I/O*/

    P4->DIR |= BIT3;           /* P4.3 set as output pin */

    ////////// DELAY TEST SETUP END

    delay_test_find_N();
}

void delay_test_find_N(void) {

    // Test function for delay, iterates through
    uint32_t max_loops = 160000;
    uint32_t current_loops;
    uint32_t increment = 10000;

    uint32_t i;
    uint32_t num_loops;

    // Delay loop
    for (current_loops = 10000; current_loops <= max_loops; current_loops = current_loops + increment)
    {
        num_loops = current_loops;
        P4->OUT = ~P4->OUT; // TOGGLE P4.3
        for (i=0; i<=num_loops; i++)
        {
            asm ("NOP");
        }
    }
}
