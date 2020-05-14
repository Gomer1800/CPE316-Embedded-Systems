/*
 * dac.h
 *
 *  Created on: May 11, 2020
 *      Author: Hayden
 */

#ifndef MY_DAC_H_
#define MY_DAC_H_

/* includes */
#include "msp.h"

/* macros */
#define DAC_CONFIG 0x3
#define DAC_DATA_UPPER 0x3C0
#define DAC_DATA_LOWER 0x03F
#define DAC_DC_BITS 0x003
#define DAC_VREF 3.3
#define DAC_INT_MAX 0x3FF

/* structs */
typedef struct {
    uint8_t upper;
    uint8_t lower;
} dac_data_t;

/* function definitions */
void init_spi(void);
dac_data_t *format_dac_data(uint16_t data);
void dac_write(uint16_t data);
uint16_t volt_to_int(double voltage);

#endif /* DAC_H_ */
