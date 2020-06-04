#include "My_Delays.h"
/*
 * My_Delays.c
 *
 *  Created on: Apr 15, 2020
 *      Author: Luis Gomez
 */

void delay_us(uint32_t microseconds) {

    // Determine current DCO configuration
    uint32_t FREQ =  get_DCO_Frequency();

    // Calculate number of iterations for desired delay
    uint32_t num_loops;

    // Note: we divide by 10000 to get rid of floating point errors
    // Scalars we multiply by are from our trendlines on excel
    switch(FREQ)
    {
        case FREQ_3MHZ:
            num_loops = ((microseconds) * SLOPE_3MHZ)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_6MHZ:
            num_loops = ((microseconds) * SLOPE_6MHZ)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_12MHZ:
            num_loops = ((microseconds) * SLOPE_12MHZ)/FLOATING_POINT_DIVISOR;
            break;
        case FREQ_24MHZ:
            num_loops = ((microseconds) * SLOPE_24MHZ)/FLOATING_POINT_DIVISOR;
            break;
        default:
            num_loops = ((microseconds) * SLOPE_1_5MHZ)/FLOATING_POINT_DIVISOR;
            break;
    }

    // Delay loop
    uint32_t i;
    for (i=0; i<num_loops; i++)
    {
        asm ("NOP");
    }
}
