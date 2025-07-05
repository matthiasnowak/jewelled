/*
 * jewel_leddriver.c
 *
 *  Created on: 4 Mar 2025
 *      Author: matth
 */

#include "jewel_leddriver.h"


const uint8_t gamma64 [64] = {
    0,1,2,3,4,5,6,7,8,10,12,14,16,18,20,22,24,26,29,32,35,38,41,44,47,50,53,57,61,65,69,73,77,81,85,89,94,99,104,109,114,119,124,129,134,140,146,152,158,164,170,176,182,188,195,202,209,216,223,230,237,244,251,255
};

const uint8_t gamma64_div2 [64] = {
        0,0,1,1,2,2,3,3,4,4,5,6,6,7,7,8,8,10,10,12,12,14,14,16,16,18,18,20,20,22,22,24,24,26,26,29,29,32,32,35,35,38,38,41,41,44,44,47,47,50,50,53,57,57,61,61,65,65,69,73,73
};

const uint8_t gamma64_div22 [64] = {
        0,0,1,1,2,2,3,3,4,5,6,7,8,9,10,11,12,13,24,16,17,19,20,22,23,25,26,28,30,32,34,36,38,40,42,44,47,49,52,54,57,59,62,64,67,70,73,76,79,82,85,88,91,94,74,101,104,108,111,115,118,122,125,127
};

const uint8_t gamma32 [32] = {
    0,1,2,4,6,10,13,18,22,28,33,39,46,53,61,69,78,86,96,106,116,126,138,149,161,173,186,199,212,226,240,255
};

void LEDDriver_Init_Config(void) {
    I2C_Init_Config();

    // Turn on Driver
    GPIO_WriteBit(IO_LED_SDB_PORT, IO_LED_SDB_PIN, Bit_SET);

    // Reset
    I2C_WriteReg(DRV_ADDR, DRV_REG_RESET, DRV_REGVAL_RESET);

    // Turn on Software shutdown
    I2C_WriteReg(DRV_ADDR, DRV_REG_SHUTDOWN, DRV_REGVAL_SSD_ON);

    //set pwm registers to 0 and global current
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + i, 0);
        I2C_WriteReg(DRV_ADDR, DRV_REG_CURRENT + i, LED_GLOBAL_CURRENT);
    }

    // Turn off global led control
    I2C_WriteReg(DRV_ADDR, DRV_REG_GLOBAL, DRV_REGVAL_GLOBALLED_OFF);

    // set frequency
    I2C_WriteReg(DRV_ADDR, DRV_REG_FREQUENCY, DRV_REGVAL_FREQ_23K);

    //call driver update
    I2C_WriteReg(DRV_ADDR, DRV_REG_UPDATE, DRV_REGVAL_UPDATE);
}

// Call update to copy temp vals to live registers
void LEDDriver_Update(void) {
    I2C_WriteReg(DRV_ADDR, DRV_REG_UPDATE, DRV_REGVAL_UPDATE);
}

// Write LED values to temporary registers
void LEDDriver_UpdateLEDsTemp(uint8_t* vals) {
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        LEDDriver_UpdateSingleLEDTemp(i, vals[i]);
    }
}

// Write LED values to temporary registers
void LEDDriver_UpdateSingleLEDTemp(uint8_t lednum, uint8_t val) {
#ifdef LED_USEGAMMA64
    #ifdef LED_PWM_DIV_1
            I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + lednum, gamma64[(uint8_t)(val/4)]);
    #endif
    #ifdef LED_PWM_DIV_2
            I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + lednum, gamma64_div2[(uint8_t)(val/4)]);
    #endif
    #ifdef LED_PWM_DIV_22
            I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + lednum, gamma64_div22[(uint8_t)(val/4)]);
    #endif
#endif
#ifdef LED_USEGAMMA32
        I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + lednum, gamma64[(uint8_t)(val/8)]/LED_PWM_DIV);
#endif
#ifdef LED_USENOGAMMA
        I2C_WriteReg(DRV_ADDR, DRV_REG_PWM_BASE + lednum, val/LED_PWM_DIV);
#endif
}

// Write LED values to temporary registers
void LEDDriver_UpdateAllTemp(uint8_t val) {
    for(uint8_t i = 0; i < LED_COUNT; i++) {
        LEDDriver_UpdateSingleLEDTemp(i, val);
    }
}

void LEDDriver_Sleep(void) {
    I2C_WriteReg(DRV_ADDR, DRV_REG_GLOBAL, DRV_REGVAL_GLOBALLED_OFF);
    I2C_WriteReg(DRV_ADDR, DRV_REG_SHUTDOWN, DRV_REGVAL_SSD_ON);
}

void LEDDriver_Wakeup(void) {
    I2C_WriteReg(DRV_ADDR, DRV_REG_SHUTDOWN, DRV_REGVAL_SSD_OFF);
    I2C_WriteReg(DRV_ADDR, DRV_REG_GLOBAL, DRV_REGVAL_GLOBALLED_ON);
}

void I2C_Init_Config(void) {
    I2C_InitTypeDef I2C_InitStructure;

    /* Enable I2C clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* I2C configuration */
    I2C_InitStructure.I2C_ClockSpeed = I2C_CLOCK_SPEED;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    /* Initialize I2C */
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);

    if(DEBUG_ENABLED) {
        //UART_Printf("I2C Initialized at %dkHz\n", I2C_CLOCK_SPEED/1000);
    }
}

void I2C_DeInit_Config(void) {
    I2C_DeInit(I2C1);
}

void I2C_Write(uint8_t addr, uint8_t* data, uint8_t len) {
    /* Wait until I2C bus is free */
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    /* Generate START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send slave address */
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Send data */
    for(uint16_t i = 0; i < len; i++) {
        I2C_SendData(I2C1, data[i]);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }

    /* Generate STOP condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_WriteReg(uint8_t addr, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    I2C_Write(addr, data, 2);
}
