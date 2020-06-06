#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "My_ADC.h"
#include "My_DCO.h"
#include "My_UART.h"

uint8_t flag = 0;

void setup_ADC14(void);

extern uint16_t digitalVal;

void UART_TX_STRING(uint16_t data){
    char buffer[33];
    sprintf(buffer, "%d", data);
    int i = 0;
    while(buffer[i] != '\0'){
        UART_TX(buffer[i]);
        i++;
    }
    UART_TX(0x1B);
    UART_TX('E');
}

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // Stop WDT

    // setup_DCO(FREQ_3MHZ);

    // setup_MCLK_to_DCO();

    // setup UART
    init_UART();

    // Configure GPIO
    P4->SEL1 |= BIT7; //Set to alternate mode| BIT6 | BIT7;
    P4->SEL0 |= BIT7; //P4.7 is A/D channel A6 according to quick guide

    //enable global interrupt
    __enable_irq();
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    setup_ADC14();

    SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;   // Wake up on exit from ISR
    __DSB();    // Ensures SLEEPONEXIT occurs immediately

    while (1){
        int i;
        if(flag == 1){
            flag = 0;
        }
        for (i = 20000; i > 0; i--);
        ADC14->CTL0 |= ADC14_CTL0_SC;       // Start conversion-software trigger

        UART_TX_STRING(digitalVal);         // EXAMPLE USE OF THE UART TX FUNCTION CALL

        __sleep();
    }
}
