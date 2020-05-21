#include "msp.h"
#include "My_Pins.h"
#include "My_Timer.h"

/**
 * Example main.c for Q1B
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    pinMode(P2, 3, IO);

    setup_TIMER_A0();

    while(1){
        __sleep();
        __no_operation();
    }
}
