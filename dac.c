/*
 * dac.c
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#include "MK64F12.h"
#include "dac.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "config.h"
#include "fsl_dac.h"
#include "fsl_common.h"

// Inicializar DAC //
void dac_init(void)
{
	dac_config_t  dac_config_init = {
		kDAC_ReferenceVoltageSourceVref2, false
	};

	DAC_Init(DAC0, &dac_config_init);
	DAC_Enable(DAC0, true);
	DAC_GetDefaultConfig(&dac_config_init);
	DAC_SetBufferReadPointer(DAC0, 0U);

}

// Buffer del DAC //
void dac_buffer_init(void)
{
	uint8_t upperlimit = bufferlimit;

	dac_buffer_trigger_mode_t trigger_mode = {
			kDAC_BufferTriggerBySoftwareMode
	};

	dac_buffer_watermark_t watermark_buffer = {
			kDAC_BufferWatermark1Word
	};

	dac_buffer_work_mode_t work_mode = {
			kDAC_BufferWorkAsNormalMode
	};

	dac_buffer_config_t buffer_conf = {
			trigger_mode, watermark_buffer, work_mode, upperlimit
	};
	DAC_SetBufferReadPointer(DAC0, &buffer_conf);
}

// Agregar al buffer //
void set_buffer(uint32_t value)
{
	DAC_SetBufferValue(DAC0, false, value);
	DAC_SetBufferReadPointer(DAC0, index_0u);
}

// Salida del DAC //
void dac_signal_output(uint16_t sample)
{
	DAC0->DAT[0].DATL = sample & flag_ff;
	DAC0->DAT[0].DATH = (sample >> bp_8) & flag;
}
