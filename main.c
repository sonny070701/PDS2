// HEADERS //
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"

#include "gpio.h"
#include "math.h"
#include "adc.h"
#include "dac.h"
#include "pit.h"
#include "config.h"

uint32_t g_pressedCounter = 1000;

// Callback function to test button
void incrementOnPush (uint32_t useless);
void decrementOnPush (uint32_t useless);

// Variable to modify amplitude of the signal
float g_amplitude = 1.0f;
// Increment the amplitude
float g_increment = 0.0f;

// System 1
float h1[N_ELEMENTS] = {-0.1050, -0.1448, -0.1721, 0.8182, -0.1721, -0.1448, 57 -0.1050};
// System 2
float h2[N_ELEMENTS] = {0.0802, 0.0860, 0.0897, 0.0909, 0.0897, 0.0860, 0.0802};

// Array to store 1
float arr1[N_ELEMENTS] = {0};
// Array to store 2
float arr2[N_ELEMENTS] = {0};

// Counter to the next element
static uint32_t count = 0;

// Push value to array
void push2Input (float val);

// Calculate the convolution of the signal with the given system
uint32_t calculate_convolution (float val, float sys[]);

int main ()
{
	GPIO_init();
	dac_init();
	PIT_init(kPIT_Chnl_0);
	ADC_config();
	ADC_start();
	dac_buffer_init();

	uint32_t ADC_convertion_status = true;
	// Variable to store ADC Reading
	uint32_t ADC_value;
	// Variable to store the result of the convolution
	uint32_t output_val = 0;
	// Variable to store the real value
	float real_analog_value = 0.0f;

	// switch 2
	uint32_t input_sw_2 = 0;
	// Dip-switch 3
	uint32_t input_sw_3 = 0;

	/* Init board hardware. */
	/*
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();
	*/

	GPIO_callback_init(GPIO_A, incrementOnPush);
	GPIO_callback_init(GPIO_C, decrementOnPush);

	for (;;)
	{
		ADC_convertion_status = ADC_get_conversion_status();
		if (TRUE == ADC_convertion_status)
		{
			ADC_value = ADC_get_simple_conversion_value();
			real_analog_value = ((float) (MAX_ANALOG_VAL * ADC_value)) / ((float) (MAX_DIGITAL_VAL));
			input_sw_3 = GPIO_Pin_Read_PORTA(GPIO_A, sw3_p);
			input_sw_2 = GPIO_Pin_Read_PORTC(GPIO_C, sw2_p);
			if ((input_sw_2 == 1) && (input_sw_3 == 0))
			{
				output_val = calculate_convolution(real_analog_value, h1);
			}
			if ((input_sw_2 == 0) && (input_sw_3 == 3))
			{
				output_val = calculate_convolution(real_analog_value, h2);
			}
			else
			{
				output_val = calculate_convolution(real_analog_value, NULL);
			}
			DAC_SetBufferValue(DAC0, 0U, (uint16_t) (output_val * g_amplitude));
		}
	}
	return 0;
}

// Callback function to test button
void incrementOnPush (uint32_t useless)
{
	g_increment += 0.1f;
	g_amplitude = 1 + g_increment;
}

// Callback function to test button
void decrementOnPush (uint32_t useless)
{
	g_increment -= 0.1f;
	g_amplitude = 1 + g_increment;
}

// Push value to array
void push2Input (float val)
{
	// If counter is even
	if (!(count & 1))
	{
		// from arr1 to arr2, current = arr2
		for (uint8_t i = 0; i < N_ELEMENTS - 1; i++)
		{
			arr2[i] = arr1[i + 1];
		}
		arr2[N_ELEMENTS - 1] = val;
	}
	else
	{
		// from arr2 to arr1, current = arr1
		for (uint8_t i = 0; i < N_ELEMENTS - 1; i++)
		{
			arr1[i] = arr2[i + 1];
		}
		arr1[N_ELEMENTS - 1] = val;
	}
}

// Calculate the convolution of the signal with the given system
uint32_t calculate_convolution (float val, float sys[])
{
	float test = 0;
	float y = 0.0f;
	float temp_y = 0.0f;
	// If there’s no system option, return the digital value

	if (sys == NULL)
	{
		// Return output value without system processing
		temp_y = ((float) MAX_DIGITAL_VAL / MAX_ANALOG_VAL) * val;
		return (uint32_t) temp_y;
	}
	push2Input(val);
	// If count is even
	if (!(count & 1))
	{
		// Use arr2 to calculate convolution
		for (uint8_t i = 0; i < N_ELEMENTS; i++)
		{
			y += sys[i] * arr2[(N_ELEMENTS - 1) - i];
		}
	}
	else
	{
		// Use arr1 to calculate convolution
		for (uint8_t i = 0; i < N_ELEMENTS; i++)
		{
			y += sys[i] * arr1[(N_ELEMENTS - 1) - i];
		}
	}
	count++;
	// transform the y analog value to digital
	temp_y = ((float) MAX_DIGITAL_VAL / MAX_ANALOG_VAL) * y;
	test = fabsf(temp_y);
	return (uint32_t) test;
}
