#include "My_Timer.h"
/*
 * My_Timer.c
 *
 *  Created on: May 8, 2020
 *      Author: kattia
 */
void setup_TIMER_A0(void){
    /* Enable interrupts at CCR0 and CCR1
     * These will hold the value of the period and duty cycle
     * of the square wave respectively
     * */
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE;
    TIMER_A0->CCTL[1] = TIMER_A_CCTLN_CCIE;

    /*SMCLK as source in continuous mode*/
    TIMER_A0->CTL = TIMER_A_CTL_CLR | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk; //enable sleep on exit from ISR
    __enable_irq(); //enable global interrupts

    NVIC->ISER[0] = (1 << ((TA0_0_IRQn) & 0x1F)); //Enable CCR0 Interrupt
    NVIC->ISER[0] = (1 << ((TA0_N_IRQn) & 0x1F)); //Enable CCR1-4 interrupts
}

/*start counting until period and then reset the count
 * toggle on and count
 */
void TA0_0_IRQHandler(void){
    uint16_t voltage = DAC_3V;
    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); // clear the interrupt flag
    TIMER_A0->CCR[0] += global_square_period;
    //write to DAC ON period
    dac_write(voltage);
}

/*CCR1 < CRR0 reach CCR1 first (duty cycle)
 * toggle off!
 * the count will keep going until CCR0 and we toggle ON again!
 */
void TA0_N_IRQHandler(void){
    TIMER_A0->CCTL[1] &= ~(TIMER_A_CCTLN_CCIFG); // clear the interrupt flag
    TIMER_A0->CCR[1] += global_square_period;
    //Write to DAC OFF period
    dac_write(0);
}

