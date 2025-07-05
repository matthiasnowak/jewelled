/*
 * jewel_ui.c
 *
 *  Created on: 9 Mar 2025
 *      Author: matth
 */

#include "jewel_ui.h"


void UI_Init(void) {
    // Setup interrupts for buttons

    /* Configure EXTI for buttons */
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        /* Enable AFIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

        /* Configure EXTI line for Button 1 (WAKEUP) */
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
        EXTI_InitStructure.EXTI_Line = EXTI_Line0;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Configure EXTI line for Button 2 */
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
        EXTI_InitStructure.EXTI_Line = EXTI_Line1;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set EXTI Interrupts */
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
        NVIC_Init(&NVIC_InitStructure);
}
