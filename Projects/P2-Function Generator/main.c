#include "msp.h"
#include "My_Keypad.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"
#include <My_DAC.h>
/**
 * main.c
 */

#define GRAN 50

// FSM STATES
enum STATE {INIT, IDLE, SQUARE, SAW, SINE};

/*
3.The function generator shall be capable of producing
    a. A square wave with variable duty cycle
    b. A sinusoidal waveform
    c. A sawtooth waveform
4. All waveforms shall have a Vpp of 3.0 V and be DC-biased at 1.5 V.
5. All waveforms shall have adjustable frequencies:
    a. 100 Hz, 200 Hz, 300 Hz, 400 Hz, 500 Hz
    b. The frequency of the waveforms should be within 2.5 Hz of these frequencies for full-credit.
    c. The function generator shall have an output rate (points / sec) of at least 75% of the maximum determined from (1) above.
    d. The output rate shall not change with waveform frequency. The time between outputs from the DAC should be the same for all frequencies.
       This means the 100 Hz waveform will output 5x the number of points in one period as the 500 Hz waveform.

6. Upon power-up, the function generator shall display a 100 Hz square wave with 50% duty cycle.
7. The keypad buttons 1-5 shall set the waveform frequency in 100 Hz increments (1 for 100 Hz, 2 for 200 Hz, etc).
8. The keypad buttons 7, 8, and 9 shall set the output waveform to Square, Sine, and Sawtooth.
9. The keypad buttons *, 0, and # shall change the duty cycle of the square wave.
    a. The * shall decrease the duty cycle by 10% down to a minimum of 10%.
    b. The # shall increase the duty cycle by 10% up to a maximum of 90%
    c. The 0 key shall reset the duty cycle to 50%
    d. The keys *, 0, and # shall not affect the sin or sawtooth waveforms.
*/
void *lcd;
enum STATE CURRENT_WAVE     = SQUARE;

void gen_square_wave(double vp, uint32_t period);
void gen_triangle_wave(double vp, uint32_t period);

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    enum STATE PRESENT_STATE    = INIT;
    enum STATE NEXT_STATE       = SQUARE;

    while(1){
        switch(PRESENT_STATE){

        case INIT:

            //***** DCO INITIALIZATION *****
            setup_DCO(FREQ_3MHZ);
            setup_MCLK_to_DCO();

            //***** LCD INITIALIZATION *****
            __delay_cycles(DELAY50MS); // Wait for LCD to power up to at least 4.5 V
            lcd = constructor_LCD(BYTE, P6);
            begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);

            //***** DAC INITIALIZATION *****
            init_spi(); // initializes eUSCI_B0 SPI for DAC

            /***** KEYPAD INITIALIZATION *****/
            setup_keypad();

            NEXT_STATE = SQUARE;
            break;

        case IDLE:
            __sleep();
            NEXT_STATE = IDLE;
            break;

        case SQUARE:
            gen_square_wave(3, 20000);
            display_menu_LCD(lcd, "SQUARE WAVE");
            NEXT_STATE = IDLE;
            break;

        case SAW:
            gen_triangle_wave(3, 20000);
            display_menu_LCD(lcd, "TRIANGLE WAVE");
            NEXT_STATE = IDLE;
            break;

        case SINE:
            // gen_square_wave(3, 20000);
            display_menu_LCD(lcd, "SINE WAVE");
            NEXT_STATE = IDLE;
            break;
        default:
            NEXT_STATE = IDLE;
            break;
        }

        PRESENT_STATE = NEXT_STATE;
    }
}

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
