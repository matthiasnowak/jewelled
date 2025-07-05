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

/*
 *@Note
 *USART Print debugging routine:
 *USART1_Tx(PA9).
 *This example demonstrates using USART1(PA9) as a print debug port output.
 *
 */

#include "debug.h"
#include "ch32v20x_conf.h"

#include "jewel_config.h"
#include "jewel_gpio.h"
#include "jewel_uart.h"
#include "jewel_adc.h"
#include "jewel_power.h"
#include "jewel_ui.h"
#include "jewel_leddriver.h"
#include "jewel_anim.h"
/* Global typedef */

/* Global define */

/* Global Variable */
extern volatile uint32_t system_ticks;


void Timer_Config(void) {

}


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */

// add to project properties -> c/c++ builds -> settings -> gnu riscv cross linker -> at the end of command line patterns " -u _printf_float" for printing floats!
// remove for prod, can cost size or ram?
#define ADC_CACHE_SIZE 1000
uint16_t adc_cache[ADC_CACHE_SIZE];

uint32_t idlecounter = 0;
uint8_t buttonpress = 0;

/////////////////
// TODO LIST:
//  - Add calcualtion of "mean led current per frame by summing led vals"
//  - add animation of battery voltage
//      (either print or use a few leds to show battery voltage)
//  - turn off LEDs when charging or switch to low power charging anim
////////////////

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    // TODO: Disable printing for prod! saves 0.3mA!
    //USART_Printf_Init(115200);
//    printf("SystemClk:%d\r\n", SystemCoreClock);
//    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
//    printf("This is printf example\r\n");
//    if(RCC->RSTSCKR & RCC_LPWRRSTF) printf("- Lowpower Reset!\r\n");
//    if(RCC->RSTSCKR & RCC_WWDGRSTF) printf("- Windows Watchdog Reset!\r\n");
//    if(RCC->RSTSCKR & RCC_IWDGRSTF) printf("- Independent Watchdog Reset!\r\n");
//    if(RCC->RSTSCKR & RCC_SFTRSTF) printf("- Software Reset!\r\n");
//    if(RCC->RSTSCKR & RCC_PORRSTF) printf("- Power-on/-down Reset!\r\n");
//    if(RCC->RSTSCKR & RCC_PINRSTF) printf("- External Reset!\r\n");
    RCC->RSTSCKR = RCC->RSTSCKR | RCC_RMVF;

    for(uint32_t testit = 0; testit <= ADC_CACHE_SIZE; testit++) {
        adc_cache[testit] = 1;
    }

    GPIO_Init_Config();
    GPIO_Peripheral_Wakeup();
    LEDDriver_Init_Config();
    __enable_irq();

    LEDDriver_Wakeup();

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000); // so hopefully 1khz
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
//    TIM_TimeBaseStructure.TIM_Period = 1000;
//    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//    // TIMx_IRQn for 2-4 or TIM1_UP_IRQn
//    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
//    NVIC_EnableIRQ(TIM1_UP_IRQn);
//    TIM_Cmd(TIM1, ENABLE);

    Anim_Init();
    UI_Init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/10000); // so hopefully 1khz
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 255;
    TIM_TimeBaseStructure.TIM_Period = 10000/ANIM_FPS;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // TIMx_IRQn for 2-4 or TIM1_UP_IRQn
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);


    NVIC_InitTypeDef NVIC_InitStructure = {0};
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_Cmd(TIM2, ENABLE);

    UART_Init_Config();
    ADC_Init_Config();


    uint8_t chargingcounter = 0;
    uint8_t disconnectcounter = 0;

    while(1)
    {
        if(buttonpress >0) {
            //printf("button %d pressed\r\n", buttonpress);
            buttonpress = 0;
        }
        // charge logic?
        if(GPIO_ReadInputDataBit(IO_CHARGE_PORT, IO_CHARGE_PIN) == 0) {
            // is charging!
            if (chargingcounter >= 100) {
                Anim_CustomAnimationLED(0, 64);
                Anim_CustomAnimationLED(1, 127);
                Anim_CustomAnimationLED(2, 255);
                //Anim_SetAnimation(ANIM_CUSTOM_DEBUG_UART);
            } else {
                chargingcounter++;
            }
        } else {
            if (chargingcounter == 0) {
                // not charging, return to normal!
                //Anim_SetAnimation(ANIM_WAVE);
            } else {
                chargingcounter--;
            }
        }
        // detecting idle



//        if(USART_ReceiveData(USART1) == 'a') {
//            printf("received 'a'!\r\n");
//        }


//        idlecounter++;
//        if(idlecounter >= 10000000) {
//            printf("idlecounter reached %d\r\n",idlecounter);
//            idlecounter = 0;
//        }


        //LEDDriver_UpdateLEDsTemp(ledvar);

        //__WFI();

        // TEST - SAMPLE MICROPHONE
#define SAMPLE_RATE_HZ 1000
#define SAMPLE_TIME_US (1000000/SAMPLE_RATE_HZ)
#define NUM_SAMPLES 2000
        uint16_t samples[NUM_SAMPLES];
        volatile uint16_t sampleIndex = 0;
        for(sampleIndex = 0; sampleIndex < NUM_SAMPLES; sampleIndex++) {
            ADC_SoftwareStartConvCmd(ADC1, ENABLE);
            while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
            samples[sampleIndex] = ADC_GetConversionValue(ADC1);
            Delay_Us(SAMPLE_TIME_US);
        }

        for(sampleIndex = 0; sampleIndex < NUM_SAMPLES; sampleIndex++) {
            if(sampleIndex > 0) printf(",");
            printf("%u", samples[sampleIndex]);
            if(sampleIndex % 200 == 0) {
                Delay_Ms(100);
                //printf("\r\n");
            }
        }
        printf("\r\n");
        Delay_Ms(1500);

        u8 val = Bit_RESET;
        for(uint16_t i = 0; i < 400; i++) { // 1khz, so 200ms = 400x switching
            //GPIO_WriteBit(GPIOA, GPIO_Pin_2, val);
            if(val == Bit_RESET) {
                val = Bit_SET;
                Delay_Us(900);
            } else {
                val = Bit_RESET;
                Delay_Us(100);
            }
        }
    }
}

void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) {
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        I2C_Init_Config();
        Anim_UpdateFrame();
        I2C_DeInit_Config();
    }
}

void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM3_IRQHandler(void) {
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        I2C_Init_Config();
        Anim_UpdateFrame();
        I2C_DeInit_Config();
    }
}

void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

uint8_t but1counter = 0;

void EXTI0_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        but1counter ++;
        printf("EXTI0\r\n");
        if(but1counter > 10) {
            but1counter = 0;
//            // Restart to uart?
//            // Unlock the ability to write to the MODE flag of STATR register.
//            //FLASH->BOOT_MODEKEYR = 0x45670123; // KEY1
//            //FLASH->BOOT_MODEKEYR = 0xCDEF89AB; // KEY2
//
//            // Set to run BOOT flash at next reset (MODE = 1).
//            FLASH->STATR = 0x4000;
//
//            // Clear all reset status flags (RMVF = 1).
//            RCC->RSTSCKR |= 0x1000000;
//
//            // Perform software reset (KEYCODE = 0xBEEF, RESETSYS = 1).
//            PFIC->CFGR = 0xBEEF0080;
        }
        /* Button state change will be processed in timer callback */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void EXTI1_IRQHandler(void) {
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        buttonpress = 2;
        printf("EXTI1\r\n");
        /* Button state change will be processed in timer callback */
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
