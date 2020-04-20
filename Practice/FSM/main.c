#include "msp.h"

/**
 * main.c
 */

struct State {
        uint8_t OutMealy[2];  //mealy outputs[in=1, in=0] - RGB LED on P2
        uint8_t outMoore;     //moore output - red LED on P1.0
        uint32_t Delay;       //delay in mS
        const struct State *Next[2];
};

typedef const struct State State_t;
#define STOP   &FSM[0]
#define GO     &FSM[1]
#define LED_OFF 0x0
#define LED_RED 0x1
#define LED_GR  0x2
#define LED_BL  0x4

State_t FSM[2] = {
                  {{LED_BL,LED_RED}, 1, 1000, {STOP, GO}},  //STOP
                  {{LED_OFF,LED_GR}, 0, 5000, {STOP, GO}}   //GO
                 };

void delay_ms(uint32_t delay)
{
    uint32_t i;
    for(i=0; i<(delay<<6); i++);
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    State_t *Pt;    //state pointer
    uint8_t Input;

    // setup LEDs and input button
    P2->SEL0 &= ~0x7;
    P2->SEL1 &= ~0x7;
    P2->DIR |= 0x7;     //green - blue - red
    P1->SEL0 &= 0x03;
    P1->SEL1 &= 0x03;
    P1->DIR |= 0x01;    //P1.0 LED enabled
    P1->DIR &= ~0x02;   // P1.1 button enable
    P1->REN |= 0x02;
    P1->OUT |= 0x02;    // P1.1 PU on

    Pt = STOP;       //init state

    while(1) {
        Input = (P1->IN & 0x02)>>1;                             //get input
        P2->OUT = (Pt->OutMealy[Input] );                       //get Mealy output
        P1->OUT = (P1->OUT & ~0x1) | Pt->outMoore;              //get Moore output
        delay_ms(Pt->Delay);                                    //wait
        Pt = Pt->Next[Input];                                   //next state

    } // end while
} // end main
