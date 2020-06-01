#include "My_Timer.h"
#include "My_DCO.h"
#include "My_Temp.h"

/*
 * My_Timer.c
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */
#define TEN_SECONDS_ELAPSED 20000

extern uint8_t count;

void setup_TIMER_A0(void){
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // period TCCR0 interrupt enabled

    // SMCLK as source in continuous mode
    TIMER_A0->CTL       = TIMER_A_CTL_CLR | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;
    TIMER_A0->CCR[0]    = PERIOD_COUNTER;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    //enable sleep on exit from ISR
    __enable_irq();                         //enable global interrupts

    NVIC->ISER[0] = (1 << ((TA0_0_IRQn) & 0x1F));   //Enable CCR0 Interrupt
}

void TA0_0_IRQHandler(void){
    if(count == TEN_SECONDS_ELAPSED){ // checks if meta counter has reached 10 seconds
        count = 0;
        if(temp_read() == 1)
            P1->OUT |=  (BIT0);         // P1.0 HIGH
        else
            P1->OUT &= ~(BIT0);         // P1.0 LOW
    }
    TIMER_A0->CCR[0] += PERIOD_COUNTER;

    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG);    // clear the interrupt flag
}
