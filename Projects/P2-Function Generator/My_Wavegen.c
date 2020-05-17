#include "My_Wavegen.h"
#include "My_DAC.h"
#include "My_Delays.h"

/*
 * My_Wavegen.c
 *
 *  Created on: May 15, 2020
 *      Author: Hayden
 */

// SINE LOOKUP TABLE
static uint16_t sine_lookup_q1[32] = {
465, 489, 512, 535, 559, 582, 604, 626,
648, 670, 691, 711, 731, 750, 768, 785,
802, 818, 833, 847, 860, 872, 882, 892,
901, 909, 915, 920, 925, 928, 929, 930
};

/*void* constructor_waveform(void){
    Wave* wave;
    wave = malloc(sizeof(Wave));
    wave->CURRENT_WAVE = SAW;
    wave->DUTY_CYCLE   = 50;
    wave->PERIOD       = 20000;
    return wave;
}*/


void gen_square_wave(double dutycycle, uint32_t period) {
    uint16_t voltage = DAC_3V;
    uint32_t on_period = period * dutycycle;
    uint32_t off_period = period - on_period;
    dac_write(voltage);
    delay_us(on_period);
    dac_write(0);
    delay_us(off_period);
}

void gen_sawtooth_wave(uint32_t period) {
    uint16_t voltage = DAC_3V;
    uint16_t step = voltage/GRAN;
    uint32_t step_delay = period/GRAN;
    uint16_t output = 0;
    while(output < voltage) {
        dac_write(output);
        output += step;
        delay_us(step_delay);
    }
}

void gen_sine_wave(uint32_t period) {
    uint8_t i;
    uint32_t step_delay = (period/4)/32;
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

/*char* get_wave_string(void) {
    switch(((Wave*)waveform)->CURRENT_WAVE){
        case SQUARE:
            return STRING_SQUARE;

        case SINE:
            return STRING_SINE;

        case SAW:
            return STRING_SAW;

        default:
            return "?????";
    }
}*/
