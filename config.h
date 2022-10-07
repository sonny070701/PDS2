/*
 * config.h
 *
 *  Created on: 12 sep 2022
 * Author: Sonny Ceja, Sergio Borrayo
*/

#ifndef CONFIG_H_
#define CONFIG_H_

typedef enum
{
	FALSE, TRUE
}boolean_t;
/*! This definition is as a general definitions to bits turn-on or turn-off any bit*/
typedef enum
{
	BIT_OFF, BIT_ON
} bit_on_off_t;
/*! This definition is as a general definitions to bits turn-on or turn-off any bit*/


// RGB.C Y RGB.H //
#define _NON_COLOR          0
#define _YELLOW             1
#define _RED1               2
#define _PURPLE             3
#define _GREEN1             4
#define _RED2               5
#define _WHITE1				6
#define _BLUE				7
#define _GREEN2				8
#define _WHITE2				9
#define SW2_GPIO            (GPIOC)
#define SW2_PIN             (6u)
#define SW3_GPIO            (GPIOA)
#define SW3_PIN             (4u)
#define RED_LED_GPIO        (GPIOB)
#define RED_LED_PIN         (22u)
#define BLUE_LED_GPIO       (GPIOB)
#define BLUE_LED_PIN        (21u)
#define GREEN_LED_GPIO      (GPIOE)
#define GREEN_LED_PIN       (26u)
#define mask_red            1 << 22
#define mask_blue           1 << 21
#define mask_green          1 << 26

// MISC //
#define bp_8 8
#define bp_6 6
#define flag 0x0F
#define flag_ff 0xFF
#define false 0
#define true  1
#define bp_16 16U
#define ciclo_3 3
#define ciclo_6 6

// GPIO //
#define all      0xFFFFFFFF
#define sw2_p    6u
#define sw3_p    4u
#define red_p    22u
#define blue_p   21u
#define green_p  26u
#define pin_11   11u
#define pin_10   10u
#define pin_3    3u
#define pin_2    2u

// Displays //
#define seg_a	 0u //PTD
#define seg_b	 1u
#define seg_c	 2u
#define seg_d	 3u
#define seg_e	 4u
#define seg_f	 5u
#define seg_g	 7u
#define disp_0	 0u //PTC
#define disp_1	 1u
#define disp_2	 2u
#define disp_3	 3u
#define disp_4	 4u
#define disp_5	 5u

// PUERTOS //
#define mask_p0  1 << 0
#define mask_p1  1 << 1
#define mask_p2  1 << 2
#define mask_p3  1 << 3
#define mask_p4  1 << 4
#define mask_p5  1 << 5
#define mask_p6  1 << 6
#define mask_p7  1 << 7
#define mask_p10 1 << 10
#define mask_p11 1 << 11

// Buttons //
#define time_stopwatch 10u //PTC
#define time_alarm     11u
#define conf_time 	   11u //PTB
#define hour_start     10u
#define min_stop       3u
#define clear          2u
#define alarm_on       4u //PTA
#define stop           6u //PTC

// State machine //
#define push_state      0
#define pull_up_state_a 1
#define pull_up_state_c 2
#define tinput_b        3

// DAC //
#define index_0u     0U
#define bufferlimit  15
#define all          0xFFFFFFFF
#define off          0
#define muestras     16

// CLOCK //
#define centena 100
#define decena  10
#define seis    6
#define cuatro  4
#define dos     2

typedef enum{
	CASO_0,
	CASO_1,
	CASO_2,
	CASO_3,
	CASO_4,
	CASO_5,
	CASO_6,
	CASO_7,
	CASO_8,
	CASO_9,
}caso_t;

#define posicion_0 0
#define posicion_1 1
#define posicion_2 2
#define posicion_3 3

// PIT //
#define freq_1k 1000u
#define freq_10 10u

// Watchdog //
#define WDOG_WCT_INSTRUCITON_COUNT (256U)
#define w_time 0x3ffU

// PDS //
#define N_ELEMENTS (7)
#define N_DIM (2)

#define MAX_ANALOG_VAL (3.3F)
#define MAX_DIGITAL_VAL (4095U)

#endif /* CONFIG_H_ */
