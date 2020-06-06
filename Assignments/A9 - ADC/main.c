#include "msp.h"
#include <stdint.h>
#include<stdio.h>
#include "My_ADC.h"
#include "My_DCO.h"
#include "My_UART.h"

uint8_t flag = 0;

void setup_ADC14(void);

extern uint16_t digitalVal;

void UART_TX_STRING(uint16_t data){
    char result[50];
    snprintf(result, 50, "%d", data);
    int i;
    for(i=0;i<50;i++){
        if(result[i] == '\0'){
            break;
        }
        else{
            UART_TX(result[i]);
        }
    }
}

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // Stop WDT

    setup_DCO(FREQ_3MHZ);

    setup_MCLK_to_DCO();

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
        UART_TX_STRING(165);
        __sleep();
    }
}
