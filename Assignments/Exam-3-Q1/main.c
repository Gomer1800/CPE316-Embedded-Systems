#include "msp.h"

// Delay constants
#define DELAY20MS               ((uint32_t)20000)   //20ms

/**
 * main.c
 */

// Delay Dummy Function
void __delay_ms(uint8_t delay);

// DCO;
void set_DCO_24MHZ(void);

// SPI
void init_SPI(void);
void send_data_spi(uint8_t data);

void main(void)
{
    uint16_t myarray[5] = {0x3333, 0x4444, 0x5555, 0x6666, 0x7777};

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// setup DCO 24MHZ
	set_DCO_24MHZ();

	// Setup GPIO for P1.1
	P1->DIR  &= ~BIT1;
	P1->SEL1 &= ~BIT1;
	P1->SEL0 &= ~BIT1;

	// Setup SPI
	init_SPI();

	// P1.1 polled every 20ms
	while(1)
	{
	    __delay_ms(DELAY20MS);

	    if(P1->OUT & BIT1)
	    {
	        uint8_t count = 0;
	        for(count=0; count<5; count++)
	        {
	            send_data_spi(myarray[count]);
	        }
	    }
	}
}

// SPI code
void send_data_spi (uint8_t data)
{
    EUSCI_B0_SPI -> TXBUF = data;
    while (!(EUSCI_B0-> STATW & EUSCI_B_STATW_SPI_BUSY));
}

void init_SPI (void)
{
    P1-> SEL1 &= ~(BIT5|BIT6);
    P1-> SEL0 |=  (BIT5|BIT6); //SCLK, SIMO
    EUSCI_B0 -> CTLW0 =
            EUSCI_B_CTLW0_SWRST |
            EUSCI_B_CTLW0_SYNC  |
            EUSCI_B_CTLW0_MST   |
            EUSCI_B_CTLW0_MSB   |
            EUSCI_B_CTLW0_SSEL__SMCLK |
            EUSCI_B_CTLW0_CKPL;
    EUSCI_B0 -> CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
}

void set_DCO_24MHZ(void)
{
    // change DC0 from default of 3MHz to 24MHz.
    CS-> KEY = CS_KEY_VAL;
    CS-> CTL0 = CS_CTL0_DCORSEL_4;
    CS-> CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS-> KEY = 0;
}

void __delay_ms(uint8_t delay)
{
    //__delay_cycles(DELAY50MS);
}
