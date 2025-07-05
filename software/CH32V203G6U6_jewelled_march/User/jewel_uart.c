/*
 * jewel_uart.c
 *
 *  Created on: 9 Mar 2025
 *      Author: matth
 */

#include "jewel_uart.h"

u8 UartRxBuffer[UART_RX_BUFFER_SIZE] = {0};                                             /* USART2 Using */
volatile u8 uartRxIndex = 0;

uint8_t debug_led_vals[LED_COUNT] = {0};

void UART_Init_Config(void) {
    // UART TEst RX interrupt?
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    /* USART configuration */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* Configure USART */
    USART_Init(USART1, &USART_InitStructure);

    /* Enable USART */
    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure = {0};
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_EnableIRQ(USART1_IRQn);

    for(uint8_t i = 0; i < LED_COUNT; i++) {
        debug_led_vals[i] = 0;
    }
    printf("Hello\r\n");
}




uint8_t isNum(uint8_t input) {
    if (input >= 48 && input <= 57) return 1;
    else return 0;
}

uint8_t toNum(uint8_t input) {
    return input - 48;
}
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        char receivedChar = USART_ReceiveData(USART1);
        if(uartRxIndex < UART_RX_BUFFER_SIZE-1) {
            UartRxBuffer[uartRxIndex] = receivedChar;
            uartRxIndex++;
        }
        //USART_SendData(USART1, receivedChar);

        if(receivedChar == 0x0A) {
            if(UartRxBuffer[0] == 'N') { // Normal mode animation
                Anim_SetAnimation(DEFAULT_ANIM);
            } else if (UartRxBuffer[0] == 'M') {// Manual mode Mxxy
                if (isNum(UartRxBuffer[1]) == 1 && isNum(UartRxBuffer[2]) == 1 && isNum(UartRxBuffer[3]) == 1) {
                    uint8_t idx = toNum(UartRxBuffer[1]) * 10 + toNum(UartRxBuffer[2]);
                    if(idx < LED_COUNT) {
                        uint8_t val = toNum(UartRxBuffer[3]);
                        if (val > 9) val = 9;
                        Anim_CustomAnimationLED(idx, val*10);

                    }

                }
                Anim_SetAnimation(ANIM_CUSTOM_DEBUG_UART);
            }
            uartRxIndex = 0; // reset index
        } else if (receivedChar == 0x0D) {
            uartRxIndex = 0;
        } else if (receivedChar == 0x00) {
            uartRxIndex = 0;
            debug_led_vals[11] = 255;
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
