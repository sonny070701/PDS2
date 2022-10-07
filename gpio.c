/*
 * gpio.c
 *
 *      Author:
 *      Sonny Ceja Celis
 *      Sergio Borrayo Anzaldo
 */

#include "MK64F12.h"
#include "gpio.h"
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "config.h"

volatile static gpio_interrupt_flags_t g_intr_status_flag = {false};

static void (*gpio_C_callback)(uint32_t flags) = false;
static void (*gpio_A_callback)(uint32_t flags) = false;
static void (*gpio_B_callback)(uint32_t flags) = false;
static void (*gpio_D_callback)(uint32_t flags) = false;
static void (*gpio_E_callback)(uint32_t flags) = false;

// iniciar callbacks del gpio //
void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags))
{
	if(GPIO_A == gpio)
	{
		gpio_A_callback = handler;
	}
	if(GPIO_C == gpio)
	{
		gpio_C_callback = handler;
	}
	if(GPIO_E == gpio)
	{
		gpio_E_callback = handler;
	}
	if(GPIO_D == gpio)
	{
		gpio_D_callback = handler;
	}
	else
	{
		gpio_B_callback = handler;
	}
}

// HANDLER del puerto C //
void PORTC_IRQHandler(void)
{
	uint32_t irq_status = false;

	irq_status = GPIO_PortGetInterruptFlags(GPIOC);

	if(gpio_C_callback)
	{
		gpio_C_callback(irq_status);
	}

	GPIO_PortClearInterruptFlags(GPIOC, irq_status);

}

// HANDLER del puerto A //
void PORTA_IRQHandler(void)
{
	uint32_t irq_status = false;

	irq_status = GPIO_PortGetInterruptFlags(GPIOA);

	if(gpio_A_callback)
	{
		gpio_A_callback(irq_status);
	}

	GPIO_PortClearInterruptFlags(GPIOA, irq_status);
}

// HANDLER del puerto B //
void PORTB_IRQHandler(void)
{
	uint32_t irq_status = false;

	irq_status = GPIO_PortGetInterruptFlags(GPIOB);

	if(gpio_B_callback)
	{
		gpio_B_callback(irq_status);
	}

	GPIO_PortClearInterruptFlags(GPIOB, irq_status);
}

void PORTD_IRQHandler(void)
{
	uint32_t irq_status = false;

	irq_status = GPIO_PortGetInterruptFlags(GPIOD);
	g_intr_status_flag.flag_port_d = true;

	GPIO_PortClearInterruptFlags(GPIOD, irq_status);
}

void PORTE_IRQHandler(void)
{
	uint32_t irq_status = false;

	irq_status = GPIO_PortGetInterruptFlags(GPIOE);
	g_intr_status_flag.flag_port_e = true;

	GPIO_PortClearInterruptFlags(GPIOE, irq_status);
}

// iniciar puertos GPIO //
void GPIO_init(void)
{
	gpio_pin_config_t gpio_input_config = {
			        kGPIO_DigitalInput,
			        false,
			    };

	gpio_pin_config_t gpio_output_config = {
			        kGPIO_DigitalOutput,
			        true,
			    };

	const port_pin_config_t button_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterEnable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
		  };


	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);
	CLOCK_EnableClock(kCLOCK_PortE);

	//Push Buttons
	GPIO_PinInit(SW2_GPIO, SW2_PIN, &gpio_input_config);
	PORT_SetPinConfig(PORTC, sw2_p, &button_config);
	PORT_SetPinInterruptConfig(PORTC, sw2_p, kPORT_InterruptFallingEdge);

	GPIO_PinInit(SW3_GPIO, SW3_PIN, &gpio_input_config);
	PORT_SetPinConfig(PORTA, sw3_p, &button_config);
	PORT_SetPinInterruptConfig(PORTA, sw3_p, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOC, time_stopwatch, &gpio_input_config);
	PORT_SetPinConfig(PORTC, time_stopwatch, &button_config);
	PORT_SetPinInterruptConfig(PORTC, time_stopwatch, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOC, time_alarm, &gpio_input_config);
	PORT_SetPinConfig(PORTC, time_alarm, &button_config);
	PORT_SetPinInterruptConfig(PORTC, time_alarm, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOB, conf_time, &gpio_input_config);
	PORT_SetPinConfig(PORTB, conf_time, &button_config);
	PORT_SetPinInterruptConfig(PORTB, conf_time, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOB, hour_start, &gpio_input_config);
	PORT_SetPinConfig(PORTB, hour_start, &button_config);
	PORT_SetPinInterruptConfig(PORTB, hour_start, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOB, min_stop, &gpio_input_config);
	PORT_SetPinConfig(PORTB, min_stop, &button_config);
	PORT_SetPinInterruptConfig(PORTB, min_stop, kPORT_InterruptFallingEdge);

	GPIO_PinInit(GPIOB, clear, &gpio_input_config);
	PORT_SetPinConfig(PORTB, clear, &button_config);
	PORT_SetPinInterruptConfig(PORTB, clear, kPORT_InterruptFallingEdge);

	//LEDS
	PORT_SetPinMux(PORTB, red_p, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOB, red_p, &gpio_output_config);

	PORT_SetPinMux(PORTB, blue_p, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOB, blue_p, &gpio_output_config);

	PORT_SetPinMux(PORTE, green_p, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOE, green_p, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_a, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_a, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_b, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_b, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_c, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_c, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_d, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_d, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_e, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_e, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_f, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_f, &gpio_output_config);

	PORT_SetPinMux(PORTD, 6u, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, 6u, &gpio_output_config);

	PORT_SetPinMux(PORTD, seg_g, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, seg_g, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_0, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_0, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_1, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_1, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_2, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_2, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_3, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_3, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_4, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_4, &gpio_output_config);

	PORT_SetPinMux(PORTC, disp_5, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOC, disp_5, &gpio_output_config);

}

// Limpiar la bandera del interrupcion //
void GPIO_clear_irq_status(gpio_name_t gpio)
{
	if(GPIO_A == gpio)
	{
		g_intr_status_flag.flag_port_a = false;
	}
	if(GPIO_C == gpio)
	{
		g_intr_status_flag.flag_port_c = false;
	}
	if(GPIO_D == gpio)
	{
		g_intr_status_flag.flag_port_d = false;
	}
	if(GPIO_E == gpio)
	{
		g_intr_status_flag.flag_port_e = false;
	}
	else
	{
		g_intr_status_flag.flag_port_b = false;
	}
}

// Obtener el status de la interrupcion //
uint8_t GPIO_get_irq_status(gpio_name_t gpio)
{
	uint8_t status = false;

	if(GPIO_A == gpio)
	{
		status = g_intr_status_flag.flag_port_a;
	}
	if(GPIO_C == gpio)
	{
		status = g_intr_status_flag.flag_port_c;
	}
	if(GPIO_D == gpio)
	{
		status = g_intr_status_flag.flag_port_d;
	}
	if(GPIO_E == gpio)
	{
		status = g_intr_status_flag.flag_port_e;
	}
	else
	{
		status = g_intr_status_flag.flag_port_b;
	}

	return(status);
}

// Limpiar puertos C //
void GPIO_Clear_PortC(void)
{
	GPIO_PortClear(GPIOC, mask_p0);
	GPIO_PortClear(GPIOC, mask_p1);
	GPIO_PortClear(GPIOC, mask_p2);
	GPIO_PortClear(GPIOC, mask_p3);
	GPIO_PortClear(GPIOC, mask_p4);
	GPIO_PortClear(GPIOC, mask_p5);
}

// Limpiar puertos //
void GPIO_Port_Clear(gpio_name_t gpio, uint32_t mask)
{
	switch(gpio)
	{
	case(GPIO_B):
		GPIO_PortClear(GPIOB, mask);
	break;
	case(GPIO_C):
		GPIO_PortClear(GPIOC, mask);
	break;
	case(GPIO_D):
		GPIO_PortClear(GPIOD, mask);
	break;
	case(GPIO_E):
		GPIO_PortClear(GPIOE, mask);
	break;
	default:
		GPIO_PortClear(GPIOA, mask);
	break;
	}

}

// Establecer puerto //
void GPIO_Port_Set(gpio_name_t gpio, uint32_t mask)
{
	switch(gpio){
		case(GPIO_B):
			GPIO_PortSet(GPIOB, mask);
		break;
		case(GPIO_C):
			GPIO_PortSet(GPIOC, mask);
		break;
		case(GPIO_D):
			GPIO_PortSet(GPIOD, mask);
		break;
		case(GPIO_E):
			GPIO_PortSet(GPIOE, mask);
		break;
		default:
			GPIO_PortSet(GPIOA, mask);
		break;
	}

}

// Leer la entrada del puerto (A//B/C) //
uint32_t GPIO_Pin_Read_PORTA(gpio_name_t gpio, uint32_t pin)
{
	uint32_t salida;
	salida = GPIO_PinRead(GPIOA, sw3_p);
	return (salida);
}

uint32_t GPIO_Pin_Read_PORTB(gpio_name_t gpio, uint32_t pin)
{
	uint32_t salida = false;
	if(pin == red_p)
		salida = GPIO_PinRead(GPIOB, red_p);
	if(pin == pin_11)
		salida = GPIO_PinRead(GPIOB, pin_11);
	if(pin == pin_10)
		salida = GPIO_PinRead(GPIOB, pin_10);
	if(pin == pin_3)
		salida = GPIO_PinRead(GPIOB, pin_3);
	if(pin == pin_2)
		salida = GPIO_PinRead(GPIOB, pin_2);
	return (salida);
}

uint32_t GPIO_Pin_Read_PORTC(gpio_name_t gpio, uint32_t pin)
{
	uint32_t salida = false;
	if(pin == pin_11)
		salida = GPIO_PinRead(GPIOC, pin_11);
	if(pin == pin_10)
		salida = GPIO_PinRead(GPIOC, pin_10);
	if(pin == sw2_p)
		salida = GPIO_PinRead(GPIOC, sw2_p);
	return (salida);
}
