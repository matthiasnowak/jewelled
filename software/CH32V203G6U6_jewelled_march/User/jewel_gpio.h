/*
 * jewel_gpio.h
 *
 *  Created on: 3 Mar 2025
 *      Author: matth
 */

#ifndef USER_JEWEL_GPIO_H_
#define USER_JEWEL_GPIO_H_

#include "jewel_config.h"
#include "ch32v20x_conf.h"

void GPIO_Init_Config(void);
void GPIO_Peripheral_Wakeup(void);
void GPIO_Peripheral_Sleep(void);

#endif /* USER_JEWEL_GPIO_H_ */
