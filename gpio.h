/*
 * gpio.h
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stdint.h"
#include "config.h"

/*! These constants are used to select an specific port in the different API functions*/
typedef enum
{
	GPIO_A, /*!< Definition to select GPIO A */
	GPIO_B, /*!< Definition to select GPIO B */
	GPIO_C, /*!< Definition to select GPIO C */
	GPIO_D, /*!< Definition to select GPIO D */
	GPIO_E, /*!< Definition to select GPIO E */
} gpio_name_t;

// banderas de interrupcion del gpio //
typedef struct
{
	uint8_t flag_port_a : true;
	uint8_t flag_port_b : true;
	uint8_t flag_port_c : true;
	uint8_t flag_port_d : true;
	uint8_t flag_port_e : true;
} gpio_interrupt_flags_t;

// iniciar callbacks del gpio //
void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags));

// Limpiar la bandera del interrupcion //
void GPIO_clear_irq_status(gpio_name_t gpio);

// Obtener el status de la interrupcion //
uint8_t GPIO_get_irq_status(gpio_name_t gpio);

// iniciar puertos GPIO //
void GPIO_init(void);

// HANDLER del puerto C //
void PORTC_IRQHandler(void);

// HANDLER del puerto A //
void PORTA_IRQHandler(void);

// HANDLER del puerto B //
void PORTB_IRQHandler(void);

// Limpiar puertos //
void GPIO_Clear_PortC(void);

void GPIO_Port_Clear(gpio_name_t gpio, uint32_t mask);

// Establecer puerto //
void GPIO_Port_Set(gpio_name_t gpio, uint32_t mask);

// Leer la entrada del puerto //
uint32_t GPIO_Pin_Read_PORTB(gpio_name_t gpio, uint32_t pin);
uint32_t GPIO_Pin_Read_PORTA(gpio_name_t gpio, uint32_t pin);
uint32_t GPIO_Pin_Read_PORTC(gpio_name_t gpio, uint32_t pin);
#endif /* GPIO_H_ */
