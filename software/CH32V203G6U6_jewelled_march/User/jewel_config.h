/*
 * config.h
 *
 *  Created on: 3 Mar 2025
 *      Author: matth
 */

#include "ch32v20x.h"
#include "ch32v20x_conf.h"

#ifndef USER_JEWEL_CONFIG_H_
#define USER_JEWEL_CONFIG_H_

/* Global define */
#define true 1
#define false 0

#define IO_OPA_P_PORT GPIOB
#define IO_OPA_P_PIN GPIO_Pin_0
#define IO_OPA_N_PORT GPIOA
#define IO_OPA_N_PIN GPIO_Pin_6
#define IO_OPA_O_PORT GPIOB
#define IO_OPA_O_PIN GPIO_Pin_1
#define IO_MICOUT_PORT GPIOA
#define IO_MICOUT_PIN GPIO_Pin_5

#define IO_BUT1_PORT GPIOA
#define IO_BUT1_PIN GPIO_Pin_0
#define IO_BUT2_PORT GPIOA
#define IO_BUT2_PIN GPIO_Pin_1
#define IO_PERI_PORT GPIOA
#define IO_PERI_PIN GPIO_Pin_4
#define IO_BUZZ_PORT GPIOA
#define IO_BUZZ_PIN GPIO_Pin_2
#define IO_BAT_PORT GPIOA
#define IO_BAT_PIN GPIO_Pin_3
#define IO_CHARGE_PORT GPIOB
#define IO_CHARGE_PIN GPIO_Pin_3

#define IO_BOOT0_PORT GPIOB
#define IO_BOOT0_PIN GPIO_Pin_8
#define IO_SCL_PORT GPIOB
#define IO_SCL_PIN GPIO_Pin_6
#define IO_SDA_PORT GPIOB
#define IO_SDA_PIN GPIO_Pin_7
#define IO_LED_SDB_PORT GPIOB
#define IO_LED_SDB_PIN GPIO_Pin_4
#define IO_TX_PORT GPIOA
#define IO_TX_PIN GPIO_Pin_9
#define IO_RX_PORT GPIOA
#define IO_RX_PIN GPIO_Pin_10
#define IO_USB_N_PORT GPIOA
#define IO_USB_N_PIN GPIO_Pin_11
#define IO_USB_P_PORT GPIOA
#define IO_USB_P_PIN GPIO_Pin_12

#define ADC_MICOUT_CH ADC_Channel_5
#define ADC_MICOUT_ADC ADC1
#define ADC_BAT_CH ADC_Channel_3
#define ADC_BAT_ADC ADC2

#define DEBUG_ENABLED 0
#define I2C_CLOCK_SPEED      400000      // 400kHz
#define LED_DRIVER_ADDR      0x60        // Example address, adjust as needed

#define LED_COUNT 18
#define LED_GLOBAL_CURRENT 0x13 // Imax * [0x10=1];[0x11=11/12];[0x12=9/12];[0x13=7/12]
#define LED_USEGAMMA64 // automatically scales 0-255 to 0-63
//#define LED_USEGAMMA32 // automatically scales 0-255 to 0-31
//#define LED_USENOGAMMA // range 0-255
#define LED_PWM_DIV_1 // divides (post-gamma) input in the driver by this int
//#define LED_PWM_DIV_2 // TODO: save ~1-2mA, maybe switch "dynamically" when battery drains?
//#define LED_PWM_DIV_4

#define ANIM_FPS 120
#define ANIM_DEFAULT_BPM 120
extern volatile uint16_t anim_bps;

#define RAND_A 1664525
#define RAND_C 1013904223
#define RAND_M 4294967296 // maybe 1 lower?


/* global variables */
extern volatile uint32_t system_ticks;

void init_rand(void);
uint32_t rand(void);

#endif /* USER_JEWEL_CONFIG_H_ */
