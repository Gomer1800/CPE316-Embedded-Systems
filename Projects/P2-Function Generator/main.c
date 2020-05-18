#include "msp.h"
#include "My_Keypad.h"
#include "My_LCD.h"
#include "My_DCO.h"
#include "My_LEDS.h"
#include "My_Delays.h"
#include "My_Wavegen.h"
#include <My_DAC.h>
#include <My_FuncGen.h>
/**
 * main.c
 */

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
enum STATE PRESENT_STATE;
enum STATE NEXT_STATE;
enum STATE CURRENT_WAVE;
uint32_t wave_period;
double wave_dutycycle;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	PRESENT_STATE = INIT;

    while(1){
        switch(PRESENT_STATE){

        case INIT:

            //***** DCO INITIALIZATION *****
            setup_DCO(FREQ_3MHZ);

            //***** LCD INITIALIZATION *****
            __delay_cycles(DELAY50MS); // Wait for LCD to power up to at least 4.5 V
            lcd = constructor_LCD(BYTE, P6);
            begin_LCD(lcd, LCD_COLUMNS, LCD_ROWS, LCD_5x8DOTS);

            //***** DAC INITIALIZATION *****
            init_spi(); // initializes eUSCI_B0 SPI for DAC

            //***** KEYPAD INITIALIZATION ****
            display_menu_LCD(lcd, "");

            //***** WAVE INITIALIZATION *****
            wave_period = P_100HZ;
            wave_dutycycle = 0.5;
            CURRENT_WAVE = SQUARE;
            NEXT_STATE = DISPLAY_MENU;
            setup_keypad();
            break;

        case DISPLAY_MENU:
            display_menu_LCD(lcd, get_wave_string(CURRENT_WAVE));
            NEXT_STATE = CURRENT_WAVE;
            break;

        case SQUARE:
            CURRENT_WAVE = SQUARE;
            gen_square_wave(wave_dutycycle, wave_period);
            //display_menu_LCD(lcd, STRING_SQUARE);
            if (CURRENT_WAVE != PRESENT_STATE) {
                NEXT_STATE = DISPLAY_MENU;
            } else {
                NEXT_STATE = CURRENT_WAVE;
            }
            break;

        case SAW:
            CURRENT_WAVE = SAW;
            gen_sawtooth_wave(wave_period);
            //display_menu_LCD(lcd, STRING_SAW);
            if (CURRENT_WAVE != PRESENT_STATE) {
                NEXT_STATE = DISPLAY_MENU;
            } else {
                NEXT_STATE = CURRENT_WAVE;
            }
            break;

        case SINE:
            CURRENT_WAVE = SINE;
            gen_sine_wave(wave_period);
            //display_menu_LCD(lcd, STRING_SINE);
            if (CURRENT_WAVE != PRESENT_STATE) {
                NEXT_STATE = DISPLAY_MENU;
            } else {
                NEXT_STATE = CURRENT_WAVE;
            }
            break;

        default:
            NEXT_STATE = SQUARE;
            break;
        }

        PRESENT_STATE = NEXT_STATE;
    }
}

void change_wave(char keypad_char){
    switch(keypad_char){

    case '1':
        wave_period = P_100HZ;
        break;

    case '2':
        wave_period = P_200HZ;
        break;

    case '3':
        wave_period = P_300HZ;
        break;

    case '4':
        wave_period = P_400HZ;
        break;

    case '5':
        wave_period = P_500HZ;
        break;

    case '6':
        break;

    case '7':
        CURRENT_WAVE = SQUARE;
        break;

    case '8':
        CURRENT_WAVE = SINE;
        break;

    case '9':
        CURRENT_WAVE = SAW;
        break;

    case '*':
        if (wave_dutycycle >= 0.2) {
            wave_dutycycle -= 0.1;
        }
        break;

    case '#':
        if (wave_dutycycle <= 0.8) {
            wave_dutycycle += 0.1;
        }
        break;

    case '0':
        wave_dutycycle = 0.5;
        break;

    default:
        //((Wave*)waveform)->CURRENT_WAVE = SQUARE;
        break;
    }
    write_char_LCD(lcd, keypad_char);
}

char* get_wave_string(STATE){
    switch(STATE){
    case SQUARE:
        return STRING_SQUARE;

    case SINE:
        return STRING_SINE;

    case SAW:
        return STRING_SAW;

    default:
        return "STATE ERROR";

    }
}
