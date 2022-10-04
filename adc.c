/*
 * adc.c
 *
 *  Created on: 4 oct 2022
 *      Author: sonny
 */

#include "ADC.h"
// Configuration structures for ADC control
static adc16_config_t adc_config_struct; // Configuration structure
static adc16_channel_config_t adc_channel_config_struct; // Channel configuration structure

static volatile boolean_t g_AdcConversionDoneFlag = TRUE; // ADC done converting
static volatile boolean_t g_AdcAverageConversionDoneFlag = FALSE; // ADC done converting average
static volatile uint32_t g_AdcConversionValueArr[N_ADC_READS] = {0}; // Conversion array values from

static volatile uint32_t g_AdcConversionValue = 0; // Conversion value from ADC
static volatile uint8_t g_pit_interrupt_count = 0; // Count PIT interrupts
static volatile uint32_t temp_conversion_value = 0; // store temporal sum of values

void ADC1_IRQHandler (void)
{
	g_AdcConversionDoneFlag = TRUE; // Done converting
	/* Read conversion result to clear the conversion completed flag. */
	g_AdcConversionValue = ADC_get_simple_conversion_value();
	SDK_ISR_EXIT_BARRIER;
}

// PIT channel 0 ISR handler
void PIT0_IRQHandler_ADC (void)
{
	/* Clear interrupt flag.*/
	PIT_ClearStatusFlags(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL, kPIT_TimerFlag);
	// Begin the ADC read
	if (TRUE == g_AdcConversionDoneFlag)
	{
		ADC_start_conversion();
	}
}
// Init PIT config

void init_PIT_config (void)
{
	/* Structure of initialize PIT */
	pit_config_t pitConfig;

	//pitConfig.enableRunInDebug = false;
	PIT_GetDefaultConfig(&pitConfig);

	/* Init pit module */
	PIT_Init(ADC_PIT_BASEADDR, &pitConfig);

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL,
	USEC_TO_COUNT(PIT_DELAY, ADC_PIT_SOURCE_CLOCK));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT0_IRQn);
}

// Configure ADC
void ADC_config (void)
{
	// Initialize PIT
	init_PIT_config();

	// Enable ADC Interrupt on conversion
	EnableIRQ(ADC_IRQn);

	// Configure ADC with the default settings
	ADC16_GetDefaultConfig(&adc_config_struct);

	// Initialize ADC device
	ADC16_Init(ADC_BASE, &adc_config_struct);

	// Disable hardware trigger
	ADC16_EnableHardwareTrigger(ADC_BASE, FALSE);

	// Calibrate ADC
	ADC16_DoAutoCalibration(ADC_BASE);

	// Configure ADC channel
	adc_channel_config_struct.channelNumber = ADC_USER_CHANNEL; // Set channel number
	adc_channel_config_struct.enableInterruptOnConversionCompleted = TRUE; // Enable interrupt
	adc_channel_config_struct.enableDifferentialConversion = FALSE; // Disable differential conversion

	/* Set timer period for channel 0 */
	PIT_SetTimerPeriod(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL,
	USEC_TO_COUNT(PIT_DELAY, ADC_PIT_SOURCE_CLOCK));

	/* Enable timer interrupts for channel 0 */
	PIT_EnableInterrupts(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL, kPIT_TimerInterruptEnable);

	/* Enable at the NVIC */
	EnableIRQ(PIT0_IRQn);

	// Start Timer
	//PIT_StartTimer(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL);
}

// Start ADC conversion
void ADC_start_conversion (void)
{
	g_AdcConversionDoneFlag = FALSE; // Set conversion flag to false (conversion in progress)
	ADC16_SetChannelConfig(ADC_BASE, ADC_CHANNEL_GROUP,
	&adc_channel_config_struct);
}

// Get ADC Conversion status
boolean_t ADC_get_conversion_status (void)
{
	return (boolean_t) g_AdcConversionDoneFlag;
}

// Get ADC Conversion average value
uint32_t ADC_get_average_conversion_value (void)
{
	return (uint32_t) g_AdcConversionValue;
}

// Get ADC Conversion average value
uint32_t ADC_get_simple_conversion_value (void)
{
	return (uint32_t) ADC_BASE->R[ADC_CHANNEL_GROUP];
}

// Stop ADC
void ADC_stop (void)
{
	PIT_StopTimer(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL);
}

// Start ADC
void ADC_start (void)
{
	PIT_StartTimer(ADC_PIT_BASEADDR, ADC_PIT_CHANNEL);
}
