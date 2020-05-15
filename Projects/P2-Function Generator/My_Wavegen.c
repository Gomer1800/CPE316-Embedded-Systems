#include "My_Wavegen.h"
#include "My_DAC.h"
#include "My_Delays.h"

/*
 * My_Wavegen.c
 *
 *  Created on: May 15, 2020
 *      Author: Hayden
 */

void gen_square_wave(uint8_t dutycycle, uint32_t period) {
    uint16_t voltage = DAC_3V;
    uint32_t on_period = (dutycycle/100) * period;
    uint32_t off_period = period - on_period;
    while(1) {
        dac_write(voltage);
        delay_us(on_period);
        dac_write(0);
        delay_us(off_period);
    }
}

void gen_triangle_wave(uint32_t period) {
    uint16_t voltage = DAC_3V;
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

void gen_sine_wave(uint32_t period) {
    uint8_t i;
    uint32_t step_delay = (period/4)/32;
    while(1) {
        // 0 to pi/2
        for (i = 0; i < 32; i++) {
            dac_write(sine_lookup_q1[i]);
            delay_us(step_delay);
        }
        // pi/2 to pi
        for (i = 31; i-- > 0; ) {
            dac_write(sine_lookup_q1[i]);
            delay_us(step_delay);
        }
        // pi to 3pi/2
        for (i = 1; i < 32; i++) {
            dac_write(DAC_3V - sine_lookup_q1[i]);
            delay_us(step_delay);
        }
        // 3pi/2 to 2pi
        for (i = 31; i-- > 1; ) {
            dac_write(DAC_3V - sine_lookup_q1[i]);
            delay_us(step_delay);
        }
    }
}
