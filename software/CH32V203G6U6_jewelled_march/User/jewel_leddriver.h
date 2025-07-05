/*
 * jewel_leddriver.h
 *
 *  Created on: 4 Mar 2025
 *      Author: matth
 */

#ifndef USER_JEWEL_LEDDRIVER_H_
#define USER_JEWEL_LEDDRIVER_H_

#include "ch32v20x.h"
#include "ch32v20x_conf.h"
#include "jewel_config.h"

#define DRV_ADDR           0b1101111    // Default I2C address
#define DRV_REG_SHUTDOWN   0x00    // Shutdown register
#define DRV_REGVAL_SSD_ON   0x00
#define DRV_REGVAL_SSD_OFF  0x01
#define DRV_REG_PWM_BASE  0x01    // PWM register base (0x01-0x12)
#define DRV_REG_UPDATE    0x13    // Update register
#define DRV_REGVAL_UPDATE   0x00
#define DRV_REG_CURRENT    0x14    // Configuration register
#define DRV_REG_GLOBAL    0x26    // Global control register
#define DRV_REGVAL_GLOBALLED_ON 0x00
#define DRV_REGVAL_GLOBALLED_OFF 0x01
#define DRV_REG_FREQUENCY 0x27
#define DRV_REGVAL_FREQ_23K 0x00
#define DRV_REGVAL_FREQ_3K45    0x01
#define DRV_REG_RESET   0x2F
#define DRV_REGVAL_RESET 0x00


void LEDDriver_Init_Config(void);
void I2C_DeInit_Config(void);
void LEDDriver_Update(void);
void LEDDriver_UpdateLEDsTemp(uint8_t* vals);
void LEDDriver_UpdateSingleLEDTemp(uint8_t lednum, uint8_t val);
void LEDDriver_UpdateAllTemp(uint8_t val);
void LEDDriver_Sleep(void);
void LEDDriver_Wakeup(void);


void I2C_Init_Config(void);
void I2C_Write(uint8_t addr, uint8_t* data, uint8_t len);
void I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t value);


#endif /* USER_JEWEL_LEDDRIVER_H_ */
