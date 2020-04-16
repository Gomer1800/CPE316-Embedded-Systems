#include "msp.h"

int main(void) {
   WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // watchdog

   P1->DIR |= BIT0;  // Set BIT0 as output

   P1->DIR &= ~BIT1; // Set BIT1 as input
   P1->IES |= BIT1;  // Interrupt on high-to-low transition
   P1->IFG &= ~BIT1; // Clear P1.1 interrupt flag
   P1->IE |= BIT1;   // Enable interrupt for P1.1

   // Enable Port 1 on the NVIC
   NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

   __enable_irq();   // Enable global interrupt
   __sleep();        // go to sleep interrupts
}

void PORT1_IRQHandler(void)
{
 if(P1->IFG & BIT1){
      P1->OUT ^= BIT0;   // Toggle Bit0
   }
   P1->IFG &= ~BIT1;     // Clear the interrupt flag

}
