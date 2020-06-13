#include "msp.h"

// PERIOD_COUNTER = Tdesired * (Fclock/IDPRESCALE)
#define PERIOD_COUNTER 7600 // this value generates a 1Khz period when used with 24MHZ clock

void init_timerA0(void);
void set_DCO_24MHZ(void);

uint8_t DATA      = 0;
uint8_t OFFSET    = 0;
uint8_t DATA_FLG  = 0;
uint8_t START_FLG = 0;
uint8_t STOP_FLG  = 0;

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	set_DCO_24MHZ();

    // Setup GPIO for P2.1 output
    P2->DIR  |=  BIT1;
    P2->SEL1 &= ~BIT1;
    P2->SEL0 &= ~BIT1;

	init_timerA0();

    while(1)
    {
        // Reset Data, Offset, and Status Flags
        DATA      = 0;
        OFFSET    = 0;
        DATA_FLG  = 0;
        START_FLG = 0;
        STOP_FLG  = 0;

        while(!(STOP_FLG & 1))
        {
            __sleep();
        }
    }
}

void init_timerA0(void)
{
    TIMER_A0->CCTL[0] = TIMER_A_CCTLN_CCIE; // period TCCR0 interrupt enabled

    // SMCLK as source in continuous mode w/ ID prescake 1/2
    TIMER_A0->CTL       = TIMER_A_CTL_CLR | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS | TIMER_A_CTL_ID_1 ;
    TIMER_A0->CCR[0]    = PERIOD_COUNTER;

    SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;    //enable sleep on exit from ISR
    __enable_irq(); //enable global interrupt

    NVIC->ISER[0] = (1 << ((TA0_0_IRQn) & 0x1F));   //Enable CCR0 Interrupt
}

void set_DCO_24MHZ(void)
{
    // change DC0 from default of 3MHz to 24MHz.
    CS-> KEY = CS_KEY_VAL;
    // select DCO clock source for MCLK
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK) | CS_CTL1_SELM_3;
    //select DCO Clock source for SMCLK
    CS->CTL1 = CS->CTL1 & ~(CS_CTL1_SELS_MASK | CS_CTL1_DIVS_MASK) | CS_CTL1_SELS_3;
    CS-> CTL0 = CS_CTL0_DCORSEL_4;
    CS-> CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS-> KEY = 0;
}

void TA0_0_IRQHandler(void)
{
    if(!(START_FLG & 1))
    {
        START_FLG  = 1;
        P2->OUT   |= BIT1;
    }
    else if(!(DATA_FLG & 1))
    {
        if(OFFSET == 8)
            DATA_FLG = 1;
        else
        {
            P2->OUT = ((DATA >> OFFSET) << 1) & BIT1;
            OFFSET++;
        }
    }
    else if(!(STOP_FLG & 1))
    {
        STOP_FLG  = 1;
        P2->OUT   |= BIT1;
    }

    // reset period counter
    TIMER_A0->CCR[0]  += PERIOD_COUNTER;
    TIMER_A0->CCTL[0] &= ~(TIMER_A_CCTLN_CCIFG); // clear the interrupt flag
}
