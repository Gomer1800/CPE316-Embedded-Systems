#include "msp.h"
#include "My_DCO.h"
#include "My_Timer.h"

/**
 * main.c
 */

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	setup_DCO(FREQ_1_5MHZ);
	setup_MCLK_and_SMCLK_to_DCO();

	P2->OUT &= ~BIT3; //CCR0
	P2->DIR |= BIT3; //set as output

	P3->OUT &= ~BIT0; //CCR1
	P3->DIR |= BIT0; //set as output

	setup_TIMER_A0();

	while(1){
	    __sleep();
	}
}


