/*
 * jewel_adc.c
 *
 *  Created on: 3 Mar 2025
 *      Author: matth
 */

#include "jewel_adc.h"

//#define SAMPLE_RATE_HZ 2000
//#define SAMPLE_TIME_US (1000000/SAMPLE_RATE_HZ)
//#define NUM_SAMPLES 1000
//uint16_t samples[NUM_SAMPLES];
//volatile uint16_t sampleIndex = 0;

void ADC_Init_Config(void) {
    // Init Microphone OPA
    OPA_InitTypeDef  OPA_InitStructure={0};
    ADC_InitTypeDef ADC_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );

    // Dont init OPA1_OUT1 as GPIO
    OPA_InitStructure.OPA_NUM=OPA1;
    OPA_InitStructure.PSEL=CHP1;
    OPA_InitStructure.NSEL=CHN1;
    OPA_InitStructure.Mode=OUT_IO_OUT1;
    OPA_Init(&OPA_InitStructure);
    OPA_Cmd(OPA1,ENABLE);

    ADC_DeInit(ADC_MICOUT_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_Pga = ADC_Pga_1;
    ADC_Init(ADC_MICOUT_ADC, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC_MICOUT_ADC, ADC_MICOUT_CH, 1, ADC_SampleTime_239Cycles5);
    ADC_Cmd(ADC_MICOUT_ADC, ENABLE);

    ADC_ResetCalibration(ADC_MICOUT_ADC);
    while (ADC_GetResetCalibrationStatus(ADC_MICOUT_ADC));
    ADC_StartCalibration(ADC_MICOUT_ADC);
    while (ADC_GetCalibrationStatus(ADC_MICOUT_ADC));

    // Init Battery sense
    ADC_DeInit(ADC_BAT_ADC);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_InitStructure.ADC_Pga = ADC_Pga_1;
    ADC_Init(ADC_BAT_ADC, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC_BAT_ADC, ADC_BAT_CH, 1, ADC_SampleTime_239Cycles5);
    ADC_Cmd(ADC_BAT_ADC, ENABLE);

    ADC_ResetCalibration(ADC_BAT_ADC);
    while (ADC_GetResetCalibrationStatus(ADC_BAT_ADC));
    ADC_StartCalibration(ADC_BAT_ADC);
    while (ADC_GetCalibrationStatus(ADC_BAT_ADC));

}
