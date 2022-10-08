/*
 * nvic.c
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#include "NVIC.h"
#include "gpio.h"
#include "MK64F12.h"

// interrucpiones y prioridades //
void NVIC_enable_interrupt_and_priotity(interrupt_t interrupt_number, priority_level_t priority)
{
	/**This functions are part of CMSIS Core functions*/
	/**It enables the IRQ*/
	NVIC_EnableIRQ(interrupt_number);
	/**It Sets the priority of the IRQ*/
	NVIC_SetPriority(interrupt_number, priority);
}

// basepri //
void NVIC_set_basepri_threshold(priority_level_t priority)
{
	/**Sets the threshold level to attend interrupts*/
	/**A shift is needed to align in a correct manner the data in priority inside BASEPRI register*/
	__set_BASEPRI(priority << (bp_8 - __NVIC_PRIO_BITS));
}

// Iniciar interrupciones //
void NVIC_INIT_interrupts(void)
{
	/**Sets the threshold for interrupts, if the interrupt has higher priority constant that the BASEPRI, the interrupt will not be attended*/
	NVIC_set_basepri_threshold(PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_1);
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ, PRIORITY_2);
	NVIC_global_enable_interrupts;
}
