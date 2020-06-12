#include "msp.h"
#include "My_DAC.h"
#include "My_UART.h"
#include "My_Delays.h"
/**
 * main.c
 */

extern uint16_t RX_DATA;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

    //***** DCO INITIALIZATION *****
    setup_DCO(FREQ_3MHZ);

    //***** UART INITIALIZATION *****
    RX_DATA = 'C';
    init_UART();

    UART_TX(RX_DATA);
    UART_TX(0x1B);
    UART_TX('E');

    //***** DAC INITIALIZATION *****
    init_spi(); // initializes eUSCI_B0 SPI for DAC

    while (1) {
        __sleep();
        if(RX_DATA <= 0xFFF){
            UART_TX_STRING(RX_DATA);
            dac_write(RX_DATA);
        }
    }
}
