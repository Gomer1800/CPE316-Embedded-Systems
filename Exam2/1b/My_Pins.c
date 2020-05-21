#include "My_Pins.h"
/*
 * My_Pins.c
 *
 *  Created on: Apr 24, 2020
 *      Author: Luis Gomez
 */

void pinMode(DIO_PORT_Even_Interruptable_Type *PORT, uint8_t PIN, uint8_t MODE)
{
    uint16_t DATA_BIT = 0x01 << PIN;
    switch(MODE)
    {
        case IO:
            PORT->DIR  |=  DATA_BIT;
            PORT->SEL1 &= ~DATA_BIT;
            PORT->SEL0 &= ~DATA_BIT;
            break;
        default:
            PORT->DIR  |=  DATA_BIT;
            PORT->SEL1 &= ~DATA_BIT;
            PORT->SEL0 &= ~DATA_BIT;
            break;
    }
}
