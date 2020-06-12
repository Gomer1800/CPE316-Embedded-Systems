#include "msp.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "My_ADC.h"
#include "My_DCO.h"
#include "My_UART.h"

volatile uint8_t flag = 0;
extern uint16_t digitalVal;

void setup_ADC14(void);

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;             // Stop WDT

    // setup UART
    init_UART();

    // Configure GPIO
    P4->SEL1 |= BIT7; //Set to alternate mode| BIT6 | BIT7;
    P4->SEL0 |= BIT7; //P4.7 is A/D channel A6 according to quick guide

    //enable global interrupt
    __enable_irq();
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    setup_ADC14();

    ADC14->CTL0 |= ADC14_CTL0_SC;               // Start conversion-software trigger

    while (1){
        int i;
        uint32_t calibrated;
        if(flag == 1){
            calibrated = calibrate_digital_to_analog(digitalVal);
            UART_TX_STRING(calibrated);
            UART_TX('\n');                      //get a newline
            UART_TX('\r');                      //put the cursor at the beginning of new line
            for (i = 20000; i > 0; i--);
            flag = 0;
            ADC14->CTL0 |= ADC14_CTL0_SC;       // Start conversion-software trigger
        }
    }
}
