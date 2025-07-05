/*
 * Ideaboard:
 *   - CRC for UART Data transmission? for loading patterns into internal flash or eeprom
 *   - EEPROM function to load/store animations and states? or debug logs (turn ons etc.)
 *   - Maybe use RTC if available on G6U6?
 *   - UART function to receive animation data and other states?
 *   - use chip on programmer to turn on/off led power, boot0 and reset to connect via usb for programming?
 *   - PWM for speaker
 *   - ADC for Battery sense, temperature, speaker
 *   - Timer for LED Display drawing, interrupt based sleep stupp
 *   - External Interrupt for button? and maybe other inputs like battery charger stuff?
 *
 *
 *
 *
 *
 */

/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

//#define LOGDEBUG

#include "debug.h"
#include "helpers.h"
//#include "LedDriver.h"
#include "i2cdriver.h"

/* Global typedef */

/* Global define */
#define RXAddress   0b11011110
#define TxAddress   0x00

/* Global Variable */

const u8 brightness = 0x13;

u8 TxData[40] = {
        0x01,
        10, 0, 10, 0, 10, 0, 10, 0, 10,
        10, 0, 10, 0, 10, 0, 10, 0, 10,
        0x00,
        brightness, brightness, brightness, brightness, brightness, brightness, brightness, brightness, brightness,
        brightness, brightness, brightness, brightness, brightness, brightness, brightness, brightness, brightness,
        0x00, 0x00 };

u8 ledArray[18] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

u8 TxData2[1] = { 0x00};


/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure_Power = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure_Power.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure_Power.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure_Power.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure_Power);

    GPIO_InitTypeDef GPIO_InitStructure_Buzz = {0};

    GPIO_InitStructure_Buzz.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure_Buzz.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure_Buzz.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure_Buzz);

    GPIO_InitTypeDef GPIO_InitStructure_LEDSDB = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure_LEDSDB.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure_LEDSDB.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure_LEDSDB.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure_LEDSDB);
}



//u8 animTest[64] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,62,60,58,56,54,52,50,48,46,44,42,40,38,36,34,32,30,28,26,24,22,20,18,16,14,12,10,8,6,4,2};

u8 animTest[64] = {
        0, 4, 8, 12, 16, 20, 24, 28, 32, 36,
        40, 44, 48, 52, 56, 60, 63, 61, 60, 58,
        57, 55, 53, 52, 50, 49, 47, 45, 44, 42,
        41, 39, 37, 36, 34, 33, 31, 29, 28, 26,
        25, 23, 21, 20, 18, 17, 15, 13, 12, 10,
        9, 7, 5, 4, 2, 1, 0, 0, 0, 0,
        0, 0, 0, 0
};

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    //u8 i = 0;

    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("GPIO Toggle TEST\r\n");
    Delay_Ms(1000);
    GPIO_Toggle_INIT();
    printf("GPIO Toggle TEST2\r\n");
    Delay_Ms(1000);
    GPIO_WriteBit(GPIOA, GPIO_Pin_7, 1); // Turn on VAA (Peripheral Power, doesnt work now!
    GPIO_WriteBit(GPIOB, GPIO_Pin_4, 0); // Turn OFF LED Driver Output

    I2CDriver i2cdrv = {0};
    I2CDriver_Init(&i2cdrv, 0x00, RXAddress);

    I2CDriver_write_data(&i2cdrv, 0x00, TxData, 40);
    I2CDriver_write_data(&i2cdrv, 0x13, TxData2, 1);

    GPIO_WriteBit(GPIOB, GPIO_Pin_4, 1); // Turn ON LED Driver Output

    int j = 0;
    int dir = 1;
    while(1)
    {
        I2CDriver_write_data(&i2cdrv, 0x13, TxData2, 1);
        //printf("GPIO Toggle TEST\r\n");
        Delay_Ms(20);

//        for(int i = 0; i < 9; i++) {
//
//            int val = (j + 5 * i) % 64;
//            val = animTest[val];
//
//
//            // mirror left/right
//            ledArray[i] = gammaCorrection64(val);
//            ledArray[17 - i] = ledArray[i];
//        }
//        if(dir == 1) {
//            j++;
//            if(j == 64) {
//                dir = 0;
//                j = 0;
//            }
//        }
////        else {
////            j--;
////            if(j == 0) {
////                dir = 1;
////                j = 0;
////            }
////        }
//        if(j>=64 || ( j == 0 && dir == 0)) {
//            j = 0;
//            dir++;
//            if(dir >= 2) {
//                dir = 0;
//            }
//        }

        for(int k = 0; k < 18; k++) {
            ledArray[k] = j;
        }
        j = j + 1;
        if ( j > 255) j = 0;


        I2CDriver_write_data(&i2cdrv, 0x01, ledArray, 18);


    }
}
