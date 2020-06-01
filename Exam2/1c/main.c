#include <My_Temp.h>
#include "msp.h"
#include "My_Delays.h"
#include "My_LEDS.h"
#include "My_DCO.h"
#include "My_Timer.h"

uint8_t count = 0;

void main(void) {
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    setup_DCO(FREQ_1_5MHZ);
    setup_MCLK_and_SMCLK_to_DCO();

    setup_RED_LED();    // sets up P1.0 for output

	init_spi();         // initializes eUSCI_B0 SPI for Temp Sensor

	delay_us(DELAY500MS);   // Wait 500ms for Temp Sensor to initialize

    setup_TIMER_A0();       // 10 second timer

    while(1){
        __sleep();
        __no_operation();
    }
}
