/*
 * adc.h
 *
 *  Created on: 4 oct 2022
 *      Author: sonny
 */
#ifndef ADC_H_
#define ADC_H_

#include "fsl_adc16.h"
#include "fsl_pit.h"
#include "pit.h"
#include "dac.h"
#include "config.h"

#define ADC_BASE ADC1 // Base ADC used
#define ADC_CHANNEL_GROUP (0U) // Channel group used for ADC
#define ADC_USER_CHANNEL (16U) // ADC channel used

#define ADC_IRQn ADC1_IRQn // ADC Interrupt Request Enable
#define N_ADC_READS (10U) // Number of ADC reads to calculate average

#define ADC_PIT_BASEADDR PIT // PIT used for ADC
#define ADC_PIT_CHANNEL kPIT_Chnl_0 // Pit channel 0

#define ADC_PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
#define PIT_DELAY (45U) // Delay in Microseconds

void init_PIT_config(void);
void ADC_config(void);
void ADC_start_conversion(void);

boolean_t ADC_get_conversion_status(void);
uint32_t ADC_get_average_conversion_value(void);
uint32_t ADC_get_simple_conversion_value(void);
void ADC_stop(void);
void ADC_start(void);

#endif /* ADC_H_ */
