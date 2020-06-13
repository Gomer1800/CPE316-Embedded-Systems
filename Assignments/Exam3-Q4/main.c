#include "msp.h"

#define _250MS              ((uint8_t)0xFA)  // 250MS

void setup_ADC14(void);
void __delay_ms(uint8_t delay);

uint16_t digitalVal;
uint8_t  flag = 1; // ADC ready flag

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    // Configure GPIO
    P4->SEL1 |= BIT7; //Set to alternate mode| BIT6 | BIT7;
    P4->SEL0 |= BIT7; //P4.7 is A/D channel A6 according to quick guide

    //enable global interrupt
    __enable_irq();
    NVIC->ISER[0] = 1 << ((ADC14_IRQn) & 31);
    setup_ADC14();

    while(1)
    {
        // 1 second delay, have to call delay 4 times because of 8bit limit
        __delay_ms(_250MS);
        __delay_ms(_250MS);
        __delay_ms(_250MS);
        __delay_ms(_250MS);
        ADC14->CTL0 |= ADC14_CTL0_SC;               // Start conversion-software trigger
        while(flag == 0){}

        flag = 0;
    }
}

void setup_ADC14(void){
    ADC14->CTL0 |= ADC14_CTL0_ON;               //ADC 14 ON
    ADC14->CTL0 |= ADC14_CTL0_SHP;              //Sample and hold pulse select mode
    ADC14->CTL0 |= ADC14_CTL0_SHT0__128;        //sample time # of cycles
    ADC14->CTL0 |= ADC14_CTL0_SSEL__MCLK;
    ADC14->CTL0 |= ADC14_CTL1_RES__14BIT;       //Use sampling timer, 14-bit conversion results
    ADC14->MCTL[0] = ADC14_MCTLN_INCH_6;        //Use Channel 6 Avcc is by default 3.3v
    ADC14->IER0 |= ADC14_IER0_IE0;              // Enable ADC conv complete interrupt
    ADC14->CTL0 |= ADC14_CTL0_ENC;              // Enable conversions
}

void ADC14_IRQHandler(void){
    digitalVal = 0;
    digitalVal = ADC14->MEM[0];                 // Read conversion result
    flag = 1;
}

void __delay_ms(uint8_t delay){}
