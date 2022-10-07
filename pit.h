/*
 * pit.h
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#ifndef PIT_H_
#define PIT_H_

#include "stdint.h"
#include "config.h"
#include "fsl_pit.h"

#define DEMO_PIT_BASEADDR PIT

typedef struct
{
    uint8_t flag_pit_0 : TRUE;
    uint8_t flag_pit_1 : TRUE;
    uint8_t flag_pit_2 : TRUE;
    uint8_t flag_pit_3 : TRUE;
} pit_interrupt_flags_t;

// Limpiar bandera de status //
void PIT_clear_irq_status (pit_chnl_t chnl);

// obtiene bandera de status //
uint8_t PIT_get_irq_status (pit_chnl_t chnl);

// Iniciar el PIT //
void PIT_init (pit_chnl_t chnl);

// Habilitar IRQ del PIT //
void PIT_Enable_IRQs(void);

// Deshabilitar IRQ del PIT //
void PIT_Disable_IRQ(pit_chnl_t chnl);

// Iniciar el callback del PIT //
void PIT_callback_init (pit_chnl_t chnl, void (*handler) (uint32_t flags));

// Fijar tiempo del PIT
void PIT_Set_timer (pit_chnl_t chnl, uint32_t sec);

#endif /* PIT_H_ */
