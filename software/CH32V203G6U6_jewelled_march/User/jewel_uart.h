/*
 * jewel_uart.h
 *
 *  Created on: 9 Mar 2025
 *      Author: matth
 */

#ifndef USER_JEWEL_UART_H_
#define USER_JEWEL_UART_H_

#include "jewel_config.h"
#include "ch32v20x_conf.h"
#include "jewel_anim.h"

// uart test buffers
#define UART_RX_BUFFER_SIZE 32

void UART_Init_Config(void);

#endif /* USER_JEWEL_UART_H_ */
