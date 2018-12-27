#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\misc.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_exti.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_gpio.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_rcc.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_dma.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_usart.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_adc.h"
#include "Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc\stm32f10x_tim.h"
#include "lcd.h"
#include "Touch.h"

NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

int duty = 1500;
int turn_mode = 0;
int dutylist[5] = {1300,1100,900,700,700};
int Time = 0;
int TimerOn = 1, k;

vu32 ADC_Sensor[5] = { (vu32)200, (vu32)200, (vu32)200, (vu32)200, (vu32)200 };
int distance[5];

void LCD_ShowNum(u8,u8,u32,u8, u16, u16);
void Setting(void);

void RCC_Setting(void);
void GPIO_Setting(void);
void NVIC_Setting(void);
void ADC_Setting(void);
void DMA_Setting(void);
//void LEDControl(int, int, int, int);
void ServoMotor_Config(void);

// DC
void Servo_Turn(int);

void delay(int t) {
    int i = 0;
    for (i = 0; i < t; i++);
}
void Setting() {
    RCC_Setting();
    GPIO_Setting();
    NVIC_Setting();
    ADC_Setting();
    DMA_Setting();
    ServoMotor_Config();
}
void Initializing() {
    SystemInit();
    LCD_Init();
    LCD_Clear(WHITE);
}
void RCC_Setting() {
    // GPIO RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    // Other RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // Timer RCC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
}
void GPIO_Setting() {
    GPIO_InitTypeDef GPIOA_Init;
//    GPIO_InitTypeDef GPIOB_Init;
    GPIO_InitTypeDef GPIOC_Init;
    GPIO_InitTypeDef GPIOD_Init;

    // For LED Test
    GPIOD_Init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
    GPIOD_Init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOD_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIOD_Init);

    // ADC Sensor (A1~A6)
    GPIOA_Init.GPIO_Pin = GPIO_Pin_1;
    GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOA_Init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_Init);

    GPIOA_Init.GPIO_Pin = GPIO_Pin_2;
    GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOA_Init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_Init);

    GPIOA_Init.GPIO_Pin = GPIO_Pin_3;
    GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOA_Init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_Init);

    GPIOA_Init.GPIO_Pin = GPIO_Pin_5;
    GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOA_Init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_Init);

    GPIOA_Init.GPIO_Pin = GPIO_Pin_6;
    GPIOA_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIOA_Init.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIOA_Init);

    // DC Motor ( C10, C11, C13, C14 )
    GPIOC_Init.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_11;
    GPIOC_Init.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOC_Init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIOC_Init);

}
void NVIC_Setting() {

}
void ADC_Setting() {
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;

    //ADC 채널 2개 사용
    ADC_InitStructure.ADC_NbrOfChannel = 5;

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 5, ADC_SampleTime_239Cycles5);

    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
}
void DMA_Setting() {
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Sensor;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 5;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);
}
void ADC_Start() {
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
// for DC Test
void TurnCW() {
    GPIOC->BRR = 0xCC0;
    GPIOC->BSRR = 0x480;
}
void TurnCCW() {
    GPIOC->BRR = 0xCC0;
    GPIOC->BSRR = 0x880;
}

void Launch(int k) {
	GPIOC->BRR = 0xCC0;
	GPIOC->BSRR = 0x0;
    LCD_ShowString(20,75, "Launch!!", RED, WHITE);
    LCD_ShowNum(20, 100, k, 4, BLUE, WHITE);
    delay(100000000);
    LCD_ShowString(20,75, "Launch!!", WHITE, WHITE);
    LCD_ShowNum(20, 100, k, 4, WHITE, WHITE);
    GPIOC->BRR = 0xCC0;
    GPIOC->BSRR = 0x80;
}

void MainMotorControl(void){
	Time++;
	if(Time < 20)
		TurnCW();	// Clock-Wise
	if(Time >= 20 && Time < 40)
		TurnCCW();	// Counter Clock-Wise
	if(Time >= 40)
		Time = 0;	// Reset the Time Flag
}

void Detecting(void){
	for(k=0; k<4; k++){
		if(distance[k] < 30){ 	// Detect
			TimerOn = 0;
			Servo_Turn(dutylist[k]);
			delay(5000000);
			Launch(k);
			TimerOn = 1;
			break;
		}
	}
}
void Display(void){
	for(k=0; k<5; k++){
		distance[k] = 67800 / ADC_Sensor[k];
	}

	LCD_ShowString(20, 50, "TEAM03", BLUE, WHITE);
	LCD_ShowNum(100, 100, distance[0], 4, BLUE, WHITE);
	LCD_ShowNum(100, 125, distance[1], 4, BLUE, WHITE);
	LCD_ShowNum(100, 150, distance[2], 4, BLUE, WHITE);
	LCD_ShowNum(100, 175, distance[3], 4, BLUE, WHITE);
	LCD_ShowNum(100, 200, distance[4], 4, BLUE, WHITE);
	LCD_ShowNum(150, 100, Time, 4, BLUE, WHITE);
}
void TIM2_IRQHandler(void) {

	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	if(TimerOn){
		MainMotorControl();
		Display();
		Detecting();
	}
	else{
		;
	}
}

void init_Timer2() {
    NVIC_InitTypeDef NVIC_InitStructure; // for interreupt
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // timerbase...

   /* TIM2 Clock Enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Enable TIM2 Global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* TIM2 Initialize */
    TIM_TimeBaseStructure.TIM_Period = 1200;
    TIM_TimeBaseStructure.TIM_Prescaler = 6000;
    //계산방법 : 1/72mhz * 1200 * 60000
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM2 Enale */
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // interrupt enable
}

// Servo
void ServoMotor_Config(void) {
    TIM_OCInitTypeDef TIM_OCInitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIM4 Initialize
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 50Hz
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500; //50 % duty cycle value;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);

    // TIM4 Enable
    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

void Servo_Turn(int duty) {
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = duty; // duty cycle;
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Disable);
}

int main() {

    Initializing();
    Setting();
    ADC_Start();
    init_Timer2();


    while (1) {
    	;
    }
}

