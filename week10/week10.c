// NOTICE
// don't use C++ Project , You Must use C Project, if you use C++ Project, Program never run.

#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0;
void RCC_Configure(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

void GPIO_Configure(){
	GPIO_InitTypeDef PD;
	PD.GPIO_Pin = (GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_7);
	PD.GPIO_Mode = GPIO_Mode_Out_PP;
	PD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &PD);
}


void ADC_Configure(){
	GPIO_InitTypeDef PA1;
	ADC_InitTypeDef ADCch1;

	PA1.GPIO_Pin = GPIO_Pin_1;
	PA1.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &PA1);

	ADCch1.ADC_Mode = ADC_Mode_Independent;
	ADCch1.ADC_ScanConvMode = DISABLE;
	ADCch1.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCch1.ADC_DataAlign = ADC_DataAlign_Right;
	ADCch1.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADCch1);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void NVICconfig(void){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = ADC1_2_IRQn;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic);
}

void ADC1_2_IRQHandler(void){
	int adc_value;
	uint16_t pos_x, pos_y;
	uint16_t pix_x, pix_y;

	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	adc_value = ADC_GetConversionValue(ADC1);
	LCD_ShowNum(50, 110, adc_value, 4, BLACK, WHITE);
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
	Touch_GetXY(&pos_x, &pos_y,1);
	Convert_Pos(pos_x, pos_y, &pix_x, &pix_y);

	if((pix_x > 30 && pix_x < 80) && (pix_y > 180 && pix_y < 210)){
		if(flag1 == 0){
			GPIO_SetBits(GPIOD, GPIO_Pin_2);
			flag1 = 1;
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);
			flag1 = 0;
		}
	}
	else if((pix_x > 120 && pix_x < 170) && (pix_y > 180 && pix_y < 210)){
		if(flag2 == 0){
			GPIO_SetBits(GPIOD, GPIO_Pin_3);
			flag2 = 1;
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_3);
			flag2 = 0;
		}
	}
	else if((pix_x > 30 && pix_x < 80) && (pix_y > 220 && pix_y < 250)){
		if(flag3 == 0){
			GPIO_SetBits(GPIOD, GPIO_Pin_4);
			flag3 = 1;
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_4);
			flag3 = 0;
		}
	}
	else if((pix_x > 120 && pix_x < 170) && (pix_y > 220 && pix_y < 250)){
		if(flag4 == 0){
			GPIO_SetBits(GPIOD, GPIO_Pin_7);
			flag4 = 1;
		}
		else{
			GPIO_ResetBits(GPIOD, GPIO_Pin_7);
			flag4 = 0;
		}
	}
	else{
		Draw_Circle(pix_x,pix_y, 6);
	}
}


int main() {

SystemInit();
RCC_Configure();
GPIO_Configure();


LCD_Init();
Touch_Configuration();
Touch_Adjust();
LCD_Clear(WHITE);

LCD_ShowString(50, 50, "WED_TEAM03", BLACK, WHITE);
// LED1
LCD_DrawRectangle(30, 180, 80, 210);
LCD_ShowString(35, 185, "LED1", BLACK, WHITE);
// LED2
LCD_DrawRectangle(120, 180, 170, 210);
LCD_ShowString(125, 185, "LED2", BLACK, WHITE);
//LED3
LCD_DrawRectangle(30, 220, 80, 250);
LCD_ShowString(35, 225, "LED3", BLACK, WHITE);
//LED4
LCD_DrawRectangle(120, 220, 170, 250);
LCD_ShowString(125, 225, "LED4", BLACK, WHITE);

ADC_Configure();
NVICconfig();

while(1){
//todo
}


}
