/*
 * helpers.c
 *
 *  Created on: 30 Sep 2024
 *      Author: matth
 */

#ifndef USER_HELPERS_C_
#define USER_HELPERS_C_




/*
 *@Note
 *CRC routine:
 *Use CRC-32 polynomial 0x4C11DB7.
 *
 */
#include "helpers.h"
#include <ch32v20x_gpio.h>
#include <ch32v20x_i2c.h>
#include <core_riscv.h>








uint8_t gammaTable32[32] = {
        0, 1, 2, 4, 6, 10, 13, 18,
        22, 28, 33, 39, 46, 53, 61, 69,
        78, 86, 96, 106, 116, 126, 138, 149,
        161, 173, 186, 199, 212, 226, 240, 255
};

uint8_t gammaTable64[64] = {
        0, 1, 2, 3, 4, 5, 6, 7,
        8, 10, 12, 14, 16, 18, 20, 22,
        24, 26, 29, 32, 35, 38, 41, 44,
        47, 50, 53, 57, 61, 65, 69, 73,
        77, 81, 85, 89, 94, 99, 104, 109,
        114, 119, 124, 129, 134, 149, 146, 152,
        158, 164, 170, 176, 182, 188, 195, 202,
        209, 216, 223, 230, 237, 244, 251, 255
};

uint8_t gammaCorrection32(uint8_t gammaval) {
    return gammaTable32[gammaval];
}

uint8_t gammaCorrection64(uint8_t gammaval) {
    return gammaTable64[gammaval];
}




/* Global define */
#define Buf_Size    32

/* Global Variable */
u32 SRC_BUF[Buf_Size] = {0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
                         0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
                         0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
                         0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
                         0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
                         0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
                         0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
                         0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};

u32 CRCValue = 0;
/*********************************************************************
 * @fn      exampleCEC
 *
 * @brief   Example CEC calculation
 *
 * @return  none
 */
int exampleCEC(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    printf("CRC TEST\r\n");
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    CRCValue = CRC_CalcBlockCRC((u32 *)SRC_BUF, Buf_Size);

    printf("CRCValue: 0x%08x\r\n", CRCValue); /* CRCValue should be 0x199AC3CA in this example */
    while(1);
}




#endif /* USER_HELPERS_C_ */
