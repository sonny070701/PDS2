/**
 * @file    Lab2.c
 * @brief   Application entry point.
 */
/*
* Programa básico para la digitalización de una señal manteniendo una frecuencia
* constante de muestreo.
*
* Se utiliza el timer PIT (Periodic Interrupt Timer) configurado con el perioodo
* de muestreo deseado.
* Durante un interrupt se toma una muestra del ADC y se pasa al DAC.
*
*/
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_pit.h"
#include "gpio.h"
#include "math.h"
#include "nvic.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_PIT_BASEADDR PIT
#define DEMO_PIT_CHANNEL  kPIT_Chnl_0
#define PIT_LED_HANDLER   PIT0_IRQHandler
#define PIT_IRQ_ID        PIT0_IRQn
/* Get source clock for PIT driver */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)
uint16_t ADC0_value(void);
volatile bool pitIsrFlag = false;
volatile bool ToggleFlag = false;

#define N_ELEMENTS 		(7)
#define N_DIM			(2)

#define MAX_ANALOG_VAL  (3.3F)
#define MAX_DIGITAL_VAL (4095U)

// Variable to modify amplitude of the signal
float g_amplitude = 1.0f;

// Increment the amplitude
float g_increment = 0.0f;

// System 1
float h1[N_ELEMENTS] = {-0.1050, -0.1448, -0.1721, 0.8182, -0.1721, -0.1448,
        -0.1050};

// System 2
float h2[N_ELEMENTS] = {0.0802, 0.0860, 0.0897, 0.0909, 0.0897, 0.0860, 0.0802};

// Array to store 1
float arr1[N_ELEMENTS] = {0};

// Array to store 2
float arr2[N_ELEMENTS] = {0};

static uint32_t count = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/
void PIT_LED_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, kPIT_TimerFlag);
    pitIsrFlag = true;
    __DSB();
}

// Callback function to test button
void
incrementOnPush (uint32_t useless);

// Callback function to test button
void
decrementOnPush (uint32_t useless);

// Push value to array
void
push2Input (float val);

// Calculate the convolution of the signal with the given system
uint32_t
calculate_convolution (float val, float sys[]);

/*******************************************************************************
 * main
 ******************************************************************************/
int main(void) {
   /* Init board hardware. */
    volatile uint16_t adc_value;
    pit_config_t pitConfig;

    // Variable to store the real value
    float real_analog_value = 0.0f;

    // Dip-switch 2
    uint32_t dip_sw_2 = 0;

    // Dip-switch 1
    uint32_t dip_sw_1 = 0;

    uint32_t output_val = 0;

    GPIO_init();

    NVIC_INIT_interrupts();

    GPIO_callback_init(GPIO_A, incrementOnPush);
    GPIO_callback_init(GPIO_C, decrementOnPush);
/*******************************************************************************
     * PIT Configuration */
        PIT_GetDefaultConfig(&pitConfig);
        /* Init pit module */
        PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);
        /* Set timer period for channel 0 */
        PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL, USEC_TO_COUNT(43U,
PIT_SOURCE_CLOCK));

/*******************************************************************************
     * Enable Timer Interrupts */
        /* Enable timer interrupts for channel 0 */
        PIT_EnableInterrupts(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL,
kPIT_TimerInterruptEnable);
        /* Enable at the NVIC */
        EnableIRQ(PIT_IRQ_ID);
    /* Start timer channel 0 */
        PIT_StartTimer(DEMO_PIT_BASEADDR, DEMO_PIT_CHANNEL);

/*******************************************************************************
     * ADC Configuration */
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    ADC0->CFG1 = ADC_CFG1_ADIV(0)|ADC_CFG1_ADLSMP_MASK |ADC_CFG1_MODE(1)|
ADC_CFG1_ADICLK(0);
    ADC0->CFG2 = 0;
    ADC0->SC2 = 0;

/*******************************************************************************
     * DAC Configuration */
    SIM->SCGC2 = 0x1000;
    DAC0->C0 = 0xC0;
    DAC0->DAT[0].DATL = 0;
    DAC0->DAT[0].DATH = 0;

/*******************************************************************************
     * Inf while */
     while (true)
         {
             /* Check whether occur interrupt and toggle LED */
             if (true == pitIsrFlag)
             {
                 // Lectura del ADC
                 ADC0->SC1[0] = ADC_SC1_ADCH(12);
                 while( (ADC0->SC1[0] & ADC_SC1_COCO_MASK) == 0);
                 adc_value = ADC0->R[0];

                 real_analog_value = ((float) (MAX_ANALOG_VAL * adc_value))
                                     / ((float) (MAX_DIGITAL_VAL));

                 dip_sw_2 = GPIO_PinRead(GPIOD, 0);
                 dip_sw_1 = GPIO_PinRead(GPIOD, 2);

                 if ((dip_sw_1 == 1) && (dip_sw_2 == 0))
				 {
					 output_val = calculate_convolution(real_analog_value, h1);
				 }
				 else if ((dip_sw_1 == 0) && (dip_sw_2 == 1))
				 {
					 output_val = calculate_convolution(real_analog_value, h2);
				 }
				 else
				 {
					 output_val = calculate_convolution(real_analog_value, NULL);
				 }

                 output_val = output_val*g_amplitude;

                 // Valor del ADC se proporciona al DAC
                 DAC0->DAT[0].DATL = (output_val) & 0xFF;
                 DAC0->DAT[0].DATH = (output_val >> 8) & 0x0F;
                 // PIT Flag down
                 pitIsrFlag = false;
             }
         }
}

// Callback function to test button
void
incrementOnPush (uint32_t useless)
{
    g_increment += 0.1f;
    g_amplitude = 1 + g_increment;
}

// Callback function to test button
void
decrementOnPush (uint32_t useless)
{
    g_increment -= 0.1f;
    g_amplitude = 1 + g_increment;
}

// Push value to array
void
push2Input (float val)
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
uint32_t
calculate_convolution (float val, float sys[])
{
    float test = 0;
    float y = 0.0f;
    float temp_y = 0.0f;

    // If there's no system option, return the digital value
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
