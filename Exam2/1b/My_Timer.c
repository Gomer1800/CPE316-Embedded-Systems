#include "My_Timer.h"
#include "My_DCO.h"
/*
 * My_Timer.c
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */

void setup_TIMER_A0(void){
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // period TCCR0 interrupt enabled

    CS->KEY     = CS_KEY_VAL;   // unlock CS registers
    CS->CTL0    = 0;            // clear register CTL0
    CS->CTL0    = FREQ_1_5MHZ;  // set DCO = 1.5 MHZ
    CS->KEY     = 0;            // lock CS
    setup_MCLK_and_SMCLK_to_DCO();

    // SMCLK as source in continuous mode
    TIMER_A0->CTL       = TIMER_A_CTL_CLR | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;
    TIMER_A0->CCR[0]    = PERIOD_COUNTER;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    //enable sleep on exit from ISR
    __enable_irq();                         //enable global interrupts

    NVIC->ISER[0] = (1 << ((TA0_0_IRQn) & 0x1F));   //Enable CCR0 Interrupt
}

void TA0_0_IRQHandler(void){
    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);    // clear the interrupt flag
    P2->OUT ^= BIT3;
    TIMER_A0->CCR[0] += PERIOD_COUNTER;
}
