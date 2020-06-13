#include "msp.h"

void convert(void);

uint8_t ADC_read(void);
void    DAC_write(uint8_t input);

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
}

void convert(void)
{
    // get   ADC input & convert
    uint8_t digitalValue = ADC_read();
    // check ADC input, and output to DAC
    if(digitalValue < 64)           // less than 1.5V
        DAC_write(0);   // output 0V
    else if (digitalValue < 170)  // less than 4.5 V
        DAC_write( (digitalValue-64) << 2 ); // scaled using factor
    else
        DAC_write(255); // output 3V
}

uint8_t ADC_read(void)
{
    return 0;
}

void    DAC_write(uint8_t input)
{
}
