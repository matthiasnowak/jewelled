#include "i2cdriver.h"
#include <ch32v20x_gpio.h>
#include <ch32v20x_i2c.h>

//#define LOGDEBUG

///* MASK */
//#define ECR_PORTPINCONFIG_MASK    ((uint16_t)0xFF80)
//#define DBGAFR_NUMBITS_MASK       ((uint32_t)0x00100000)
//
//#if defined (CH32V20x_D6)
//uint8_t MCU_Version = 0;
//#endif


//void I2CDriver_Init(I2CDriver* driver, uint8_t altMode = I2CDriver_DefaultMode, uint8_t txAddress = 0x03, uint8_t rxAddress = 0x02) {
void I2CDriver_Init(I2CDriver* driver, uint8_t txAddress, uint8_t rxAddress) {

    driver->initialized = I2CDriver_NotInitialized;
    driver->txAddress = txAddress;
    driver->rxAddress = rxAddress;

    GPIO_InitTypeDef GPIO_InitStructure={0};
    I2C_InitTypeDef I2C_InitSturcture={0};

    // G6 only has I2C1 DefaultMode!
    //   - I2C1 DefaultMode: SCL = PB6 ; SDA = PB7 ; (SMBA = PB5)

    // Enable Clocks and Power
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE );

    // Init SDA/SCL IO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // SCL
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; // SDA
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    I2C_InitSturcture.I2C_ClockSpeed = 400000; // low enough speed idk? ; alt = 5000
    I2C_InitSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitSturcture.I2C_DutyCycle = I2C_DutyCycle_2; // old : I2C_DutyCycle_16_9;
    I2C_InitSturcture.I2C_OwnAddress1 = driver->txAddress; // alt = =
    I2C_InitSturcture.I2C_Ack = I2C_Ack_Enable; // Alt = disable
    I2C_InitSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init( I2C1, &I2C_InitSturcture );

    I2C_Cmd( I2C1, ENABLE );

    driver->initialized = I2CDriver_Initialized;
    printf("I2C bus initialized\n");
}

void I2CDriver_DeInit(I2CDriver* driver) {
    driver->initialized = I2CDriver_NotInitialized;
    I2C_DeInit(I2C1);

    GPIO_InitTypeDef GPIO_DeInitStructure={0};

    GPIO_StructInit(&GPIO_DeInitStructure);
    GPIO_DeInitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_DeInitStructure);
    GPIO_DeInitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_DeInitStructure);
}

void I2CDriver_write_data(I2CDriver* driver, u8 reg, char* data, u8 length) {
#ifdef LOGDEBUG
    printf("I2CDriver_write_data a=%d r=%d l=%d\r\n", driver->rxAddress, reg, length);
#endif // LOGDEBUG
#ifdef LOGDEBUG
    printf("IIC Not Busy\r\n");
#endif // LOGDEBUG
    I2C_GenerateSTART( I2C1, ENABLE );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, driver->rxAddress, I2C_Direction_Transmitter );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );
    if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2C1, reg);
    }
    for(u8 i = 0; i < length; i++) {
        if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
        {
            I2C_SendData( I2C1, data[i]);
        }
        while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
#ifdef LOGDEBUG
            printf("sent data i=%d data=%d\r\n", i, data[i]);
#endif // LOGDEBUG
    }
    I2C_GenerateSTOP( I2C1, ENABLE );
#ifdef LOGDEBUG
    printf("I2CDriver_write_data finished\r\n");
#endif // LOGDEBUG
}

void I2CDriver_read_data(I2CDriver *driver, u8 reg, char* buffer, u8 length) {
#ifdef LOGDEBUG
    printf("I2CDriver_read_data a=%d r=%d l=%d", driver->rxAddress, reg, length);
#endif // LOGDEBUG
    while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
    printf("IIC Not Busy\r\n");
    I2C_GenerateSTART( I2C1, ENABLE );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
    I2C_Send7bitAddress( I2C1, driver->rxAddress, I2C_Direction_Transmitter );
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );
    if( I2C_GetFlagStatus( I2C1, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2C1, reg);
    }
    for(u8 i = 0; i < length; i++) {
        if( I2C_GetFlagStatus( I2C1, I2C_FLAG_RXNE ) !=  RESET )
        {

            buffer[i] = I2C_ReceiveData(I2C1);
        }
    }
    while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_GenerateSTOP( I2C1, ENABLE );
#ifdef LOGDEBUG
    printf("I2CDriver_read_data finished");
#endif // LOGDEBUG
}

void I2CDriver_setRxAddress(I2CDriver *driver, uint8_t newAddress) {
    driver->rxAddress = newAddress;
}

uint8_t status(I2CDriver* driver) {
    return driver->initialized;
}

///*********************************************************************
// * @fn      GPIO_DeInit
// *
// * @brief   Deinitializes the GPIOx peripheral registers to their default
// *        reset values.
// *
// * @param   GPIOx - where x can be (A..G) to select the GPIO peripheral.
// *
// * @return  none
// */
//void GPIO_DeInit(GPIO_TypeDef *GPIOx)
//{
//    if(GPIOx == GPIOA)
//    {
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
//    }
//    else if(GPIOx == GPIOB)
//    {
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
//    }
//    else if(GPIOx == GPIOC)
//    {
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
//    }
//    else if(GPIOx == GPIOD)
//    {
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
//    }
//    else if(GPIOx == GPIOE)
//    {
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
//        RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
//    }
//}
