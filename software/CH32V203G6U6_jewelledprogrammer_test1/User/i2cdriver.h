/********************************** (C) COPYRIGHT  *******************************
 * File Name          : ch32v20x_gpio.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2024/03/02
 * Description        : This file contains all the functions prototypes for the
 *                      GPIO firmware library.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#ifndef __I2CDRIVER_H
#define __I2CDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32v20x.h"
#include "ch32v20x_i2c.h"
#include "ch32v20x_gpio.h"

/* Configuration Mode enumeration */
typedef struct {
    uint8_t txAddress;  // I2C device address
    uint8_t rxAddress;
    uint8_t initialized;
} I2CDriver;

#define I2CDriver_Initialized 1
#define I2CDriver_NotInitialized 0

///* GPIO_pins_define */
//#define GPIO_Pin_0                      ((uint16_t)0x0001) /* Pin 0 selected */
//#define GPIO_Pin_1                      ((uint16_t)0x0002) /* Pin 1 selected */
//#define GPIO_Pin_All                    ((uint16_t)0xFFFF) /* All pins selected */
//#define GPIO_Remap_I2C1                 ((uint32_t)0x00000002) /* I2C1 Alternate Function mapping */
//#define GPIO_PinSource0                 ((uint8_t)0x00)
//#define GPIO_PinSource2                 ((uint8_t)0x02)
//
//
//void     GPIO_DeInit(GPIO_TypeDef *GPIOx);
//void     GPIO_AFIODeInit(void);
//void     GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);
//void     GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);
//uint8_t  GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx);
//uint8_t  GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx);
//void     GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//void     GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//void     GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
//void     GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal);
//void     GPIO_PinLockConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
//void     GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//void     GPIO_EventOutputCmd(FunctionalState NewState);
//void     GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState);
//void     GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource);
//void     GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface);
//void     GPIO_IPD_Unused(void);

void I2CDriver_Init(I2CDriver* driver, uint8_t txAddress, uint8_t rxAddress);
void I2CDriver_DeInit(I2CDriver* driver);

void I2CDriver_write_data(I2CDriver* driver, u8 address, char* data, u8 length);
void I2CDriver_read_data(I2CDriver *driver, u8 address, char* buffer, u8 length);

void I2CDriver_setRxAddress(I2CDriver* driver, uint8_t newAddress);

uint8_t status(I2CDriver* driver);

#ifdef __cplusplus
}
#endif

#endif
