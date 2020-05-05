#include "msp.h"
#include "My_LEDS.h"
/**
 * main.c
 */
#define DELAY100MS 100000 * 1// DELAY * 1 mS assuming 3 MHz clock

int main(void)
{
   WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // watchdog

   // Blink RED LED to show code is running
   setup_RED_LED();

   // Set up Pin 2.5 as IO
   P2->SEL1 &= ~BIT5;
   P2->SEL0 &= ~BIT5;

   // set P2.5 as input with pulldown resistor
   P2->DIR &= ~BIT5;
   P2->REN |=  BIT5;
   P2->OUT |=  BIT5;

   P2->IES |=  BIT5;    // Interrupt on high-to-low transition
   P2->IFG &= ~BIT5;    // Clear P2.5 interrupt flag
   P2->IE  |=  BIT5;    // Enable interrupt for P2.5

   P1->OUT |= (BIT0);   /* turn on  P1.0 red LED */

   // Enable Port 2 on the NVIC
   NVIC->ISER[1] = 1 << ((PORT2_IRQn) & 31); // 31

   __enable_irq();      // Enable global interrupt

   while(1){
       __sleep();           // go to sleep interrupts
   }
}

void PORT2_IRQHandler(void)
{
 if(P2->IFG & BIT5){
      P1->OUT ^= BIT0;  // Toggle Bit0
   }
   P2->IFG &= ~BIT5;    // Clear the interrupt flag
}
