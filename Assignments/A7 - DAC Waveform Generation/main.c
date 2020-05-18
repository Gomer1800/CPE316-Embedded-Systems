#include <My_DAC.h>
#include "msp.h"
#include "My_Delays.h"

#define GRAN 50

/**
 * main.c
 */

void gen_square_wave(double vp, uint32_t period) {
    uint16_t voltage = volt_to_int(vp);
    while(1) {
        dac_write(voltage);
        delay_us(period/2);
        dac_write(0);
        delay_us(period/2);
    }
}

void gen_triangle_wave(double vp, uint32_t period) {
    uint16_t voltage = volt_to_int(vp);
    uint16_t step = voltage/GRAN;
    uint16_t output = 0;
    while(1) {
        while(output < voltage) {
            dac_write(output);
            output += step;
            delay_us((period/2)/GRAN);
        }
        while(output > 0) {
            dac_write(output);
            output -= step;
            delay_us((period/2)/GRAN);
        }
    }

}

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	init_spi(); // initializes eUSCI_B0 SPI for DAC
	// gen_square_wave(2.5, 20000);
	gen_triangle_wave(2.5, 20000);
}
