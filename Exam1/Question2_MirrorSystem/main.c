#include "msp.h"
#include <sys/stdint.h>
#include "My_DCO.h"
#include "My_Delays.h"
/**
 * main.c
 */
// CUSTOM MIRROR DELAYS
#define TSW         ((uint32_t)15)  // 15us
#define TH          ((uint32_t)20)  // 20us

// FSM STATES
enum STATE {INIT, WAIT, POLL, ACTUATE};

// GPIO MACRO
#define BITS0_THRU_5    (uint16_t)(0x001F);

void init_GPIO(void) {
    // Set up Pins 2.0 - 2.5 as IO
    P2->SEL1 &= ~BITS0_THRU_5;
    P2->SEL0 &= ~BITS0_THRU_5;
}

void mirror_logic(uint8_t data) {

    if ((data > 3) & (data < 12)) {
        // activate Mirror 1
        P2->OUT |= BIT5;
        // wait Tsw , then activate Mirror 2
        delay_us(TSW);
        P2->OUT |= BIT4;
        // Leave Mirrors ON for TH, then deactivate both Mirrors
        delay_us(TH);
        P2->OUT &= ~(BIT5 | BIT4);
    }
}

void main(void) {

    // Setup
    enum STATE PRESENT_STATE = INIT;
    enum STATE NEXT_STATE    = WAIT;
    setup_DCO(FREQ_6MHZ);           // setup DCO at 6MHZ
	init_GPIO();                    // setup GPIO 2.0-2.5
	uint8_t sensor_reading = 0;

	while(1){ // FSM LOGIC
	    switch(PRESENT_STATE){

	    case INIT:
            NEXT_STATE      = WAIT;
	        sensor_reading  = 0;
	        // Set up Sensor pins 2.0-2.3 as input
	        P2->DIR &= ~(BIT3 | BIT2 | BIT1| BIT0);
	        // Set up Mirrors as Outputs & set to ground state
	        P2->DIR |=   BIT5 | BIT4;
	        P2->OUT &= ~(BIT5 | BIT4);
	        break;

	    case WAIT:
            NEXT_STATE = POLL;
	        sensor_reading = 0;
	        delay_us(DELAY2MS);
	        break;

	    case POLL:
	        NEXT_STATE = ACTUATE;
	        sensor_reading |=  (BIT3|BIT2|BIT1|BIT0);
	        break;

	    case ACTUATE:
	        NEXT_STATE = WAIT;
	        mirror_logic(sensor_reading);
	        break;

	    default:
	        PRESENT_STATE   = INIT;
	        NEXT_STATE      = WAIT;
	    }
	    // Transition to Next state
	    PRESENT_STATE = NEXT_STATE;
	}
}
