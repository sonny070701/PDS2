/*
 * pit.c
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#include "pit.h"
#include "fsl_pit.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "config.h"
#include "nvic.h"

#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)

static void (*pit_0_callback)(uint32_t flags) = FALSE;
static void (*pit_2_callback)(uint32_t flags) = FALSE;

volatile static pit_interrupt_flags_t g_intr_status_flag = { FALSE };

/**Clear the interrupts flags*/
void PIT_clear_irq_status(pit_chnl_t chnl) {
	switch (chnl) {
		case (kPIT_Chnl_0):
			g_intr_status_flag.flag_pit_0 = FALSE;
		break;
		case (kPIT_Chnl_1):
			g_intr_status_flag.flag_pit_1 = FALSE;
		break;
		case (kPIT_Chnl_2):
			g_intr_status_flag.flag_pit_2 = FALSE;
		break;
		default:
			g_intr_status_flag.flag_pit_3 = FALSE;
		break;
	}
}

/**Get the interrupt status of the pits.*/
uint8_t PIT_get_irq_status(pit_chnl_t chnl) {

	uint8_t status = false;

	switch (chnl) {
		case (kPIT_Chnl_0):
			status = g_intr_status_flag.flag_pit_0;
		break;
		case (kPIT_Chnl_1):
			status = g_intr_status_flag.flag_pit_1;
		break;
		case (kPIT_Chnl_2):
			status = g_intr_status_flag.flag_pit_2;
		break;
		default:
			status = g_intr_status_flag.flag_pit_3;
		break;
	}
	return (status);
}

/**Initializes the PIT*/
void PIT_init(pit_chnl_t chnl) {
	pit_config_t pitConfig;

	CLOCK_SetSimSafeDivs();

	PIT_GetDefaultConfig(&pitConfig);

	/**Initialize pit module */
	PIT_Init(DEMO_PIT_BASEADDR, &pitConfig);
	 /**Enters default period*/
	PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, chnl, USEC_TO_COUNT(10059u, PIT_SOURCE_CLOCK));
	/**Enable interrupts*/

}

// Habilita las interrupciones y prioridades //
void PIT_Enable_IRQs(void)
{
	PIT_EnableInterrupts(DEMO_PIT_BASEADDR, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(DEMO_PIT_BASEADDR, kPIT_Chnl_1, kPIT_TimerInterruptEnable);
	PIT_EnableInterrupts(DEMO_PIT_BASEADDR, kPIT_Chnl_2, kPIT_TimerInterruptEnable);

	NVIC_set_basepri_threshold(PRIORITY_10);

	// Interrupciones de PIT//
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_7);
	NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_8);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_9);

	// Interrupciones de GPIO //
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ,PRIORITY_6);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ,PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ,PRIORITY_4);

	NVIC_global_enable_interrupts;
	PIT_StartTimer(DEMO_PIT_BASEADDR, kPIT_Chnl_0);
	PIT_StartTimer(DEMO_PIT_BASEADDR, kPIT_Chnl_1);
	PIT_StartTimer(DEMO_PIT_BASEADDR, kPIT_Chnl_2);

}

// Deshabilita IRQ //

void PIT_Disable_IRQ(pit_chnl_t chnl)
{
	PIT_DisableInterrupts(DEMO_PIT_BASEADDR, chnl, kPIT_TimerInterruptEnable);
}

/**Initialize callbacks*/
void PIT_callback_init (pit_chnl_t chnl, void (*handler) (uint32_t flags))
{
	if(kPIT_Chnl_0 == chnl)
	{
		pit_0_callback = handler;
	}
	else
	{
		pit_2_callback = handler;
	}
}

/**Interrupt function*/
void PIT0_IRQHandler(void)
{
	uint32_t irq_status = false;
	irq_status = PIT_GetStatusFlags(DEMO_PIT_BASEADDR, kPIT_Chnl_0);
	static uint32_t counter = false;

	if(pit_0_callback)
	{
		pit_0_callback(counter);
		counter ++;
	}
	if(bp_6 == counter)
		counter = false;
	PIT_ClearStatusFlags(DEMO_PIT_BASEADDR,kPIT_Chnl_0, irq_status);
	__DSB(); //Used to enter the interrupt
}

void PIT1_IRQHandler(void)
{
	g_intr_status_flag.flag_pit_1 = TRUE;
	uint32_t irq_status = false;
	irq_status = PIT_GetStatusFlags(DEMO_PIT_BASEADDR, kPIT_Chnl_1);
	PIT_ClearStatusFlags(DEMO_PIT_BASEADDR,kPIT_Chnl_1,  irq_status);
	__DSB(); //Used to enter the interrupt
}

void PIT2_IRQHandler(void)
{
	uint32_t irq_status = false;
		irq_status = PIT_GetStatusFlags(DEMO_PIT_BASEADDR, kPIT_Chnl_2);

		if(pit_2_callback)
		{
			pit_2_callback(irq_status);
		}
		PIT_ClearStatusFlags(DEMO_PIT_BASEADDR,kPIT_Chnl_2, irq_status);
		__DSB(); //Used to enter the interrupt
}

void PIT_Set_timer (pit_chnl_t chnl, uint32_t sec)
{
	/**Set Timer*/
	PIT_SetTimerPeriod(DEMO_PIT_BASEADDR, chnl, USEC_TO_COUNT(sec, PIT_SOURCE_CLOCK));
}


