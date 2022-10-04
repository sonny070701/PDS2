/*
 * dac.h
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#ifndef DAC_H_
#define DAC_H_

#include "stdint.h"
#include "fsl_dac.h"

// Inicializar DAC //
void dac_init(void);

// Buffer del DAC //
void dac_buffer_init(void);

// Agregar al buffer //
void set_buffer(uint32_t value);

// Salida del DAC //
void dac_signal_output(uint16_t sample);

#endif /* DAC_H_ */
