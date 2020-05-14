#include "My_Timer.h"
/*
 * My_Timer.c
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */
void setup_TIMER_A0(void){
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // period TCCR0 interrupt enabled
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;//duty cycle TCCR0 interrupt enabled

    /*SMCLK as source in continuous mode*/
    TIMER_A0->CTL = TIMER_A_CTL_CLR | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;
    TIMER_A0->CCR[0] = MY_CCR0;
    TIMER_A0->CCR[1] = MY_CCR1;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; //enable sleep on exit from ISR
    __enable_irq(); //enable global interrupts

    NVIC->ISER[0] = (1 << ((TA0_0_IRQn) & 0x1F)); //Enable CCR0 Interrupt
    NVIC->ISER[0] = (1 << ((TA0_N_IRQn) & 0x1F)); //Enable CCR1-4 interrupts
}

void TA0_0_IRQHandler(void){
    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); // clear the interrupt flag
    P2->OUT ^= BIT3;
    TIMER_A0->CCR[0] += MY_CCR0;
}

void TA0_N_IRQHandler(void){
    TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG); // clear the interrupt flag
    P3->OUT ^= BIT0;
    TIMER_A0->CCR[1] += MY_CCR1;
}

