#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

__IO uint32_t ADCConvertedValue[2];

void RCC_Configuration(){
	//DMA1
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//ADC and GPIO C clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);
}

void GPIO_Configuration(){ // ADC 체널 1,2사용
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//LED1,2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_Configuration(){
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 2; // 멀티채널 ADC, DMA사용하기 떄문에 2개
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 11,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 12,ADC_SampleTime_55Cycles5);

	ADC_DMACmd(ADC1, ENABLE);

	ADC_Cmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);

	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void DMA_Configuration(){
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR; // 데이터 레지스터를 주변 기기 소스로
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ADCConvertedValue; // 저장할 변수 값을 dest
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 변수가 배열로 들어가서 메모리를 증가 시켜줌 멀티 채널에 따른 위치에 저장 가능하도록 함
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);

}

void main(){
	SystemInit();
	LCD_Init();
	Touch_Configuration();
//	Touch_Adjust();
	LCD_Clear(WHITE);
	RCC_Configuration();
	GPIO_Configuration();
	DMA_Configuration();
	ADC_Configuration();

	while(1){
		LCD_ShowNum(50, 110, ADCConvertedValue[0], 4, BLACK, WHITE);
		LCD_ShowNum(120, 110, ADCConvertedValue[1], 4, BLACK, WHITE);
		if(ADCConvertedValue[0] > 1000){
			GPIO_SetBits(GPIOD, GPIO_Pin_2);
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		}
		if(ADCConvertedValue[1] > 1000){
			GPIO_SetBits(GPIOD, GPIO_Pin_3);
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_3);
		}
	}
}
