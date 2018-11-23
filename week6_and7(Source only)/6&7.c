// NOTICE
// don't use C++ Project , You Must use C Project, if you use C++ Project, Program never run.

#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include <assert.h>
//define
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitTypeDef gpB, gpD, gpD_btn, gpC;
EXTI_InitTypeDef exti, exti_btn;
NVIC_InitTypeDef nvic, nvic_btn, nvic_up, nvic_down, nvic_sel, nvic_USART1;
USART_InitTypeDef usart;
#define PORTD_SET (*(volatile unsigned *)0x40011410)
#define PORTD_RESET (*(volatile unsigned *)0x40011414)
char a[16] = { '0', '1', '2', '3','4','5','6','7','8', '9','a', 'b', 'c', 'd', 'e', 'f' };

int counter = 3;
int flag = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void SendData(int data) {
	int i = 0;
	USART1->DR = a[data] & 0xFF;
//	USART1->DR += '0';
	//데이터를 다 보내는 것을 기다린다]
	for (i = 0; i < 300000; i++);
}

void usart_Set()
{
	usart.USART_BaudRate = 9600;

	//data length 8 + parity bit = 9 bit
	usart.USART_WordLength = USART_WordLength_9b;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Parity = USART_Parity_Even;
	usart.USART_StopBits = USART_StopBits_1;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);

}

void gpio_set()
{
	// Open Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	// set LED Pin
	gpD.GPIO_Mode = GPIO_Mode_Out_PP;
	gpD.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
	gpD.GPIO_Speed = GPIO_Speed_50MHz;

	// set joy stick 2=down,  5=up
	gpC.GPIO_Mode = GPIO_Mode_IPD;
	gpC.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_5);
	gpC.GPIO_Speed = GPIO_Speed_50MHz;

	// connect interrupt to pin
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);

	//USART TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_Init(GPIOD, & gpD);
	GPIO_Init(GPIOC, & gpC);
}

void EXTI_Config(void) {
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Falling;
	exti.EXTI_Line = (EXTI_Line2 | EXTI_Line5);
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);
	EXTI_Init(&exti);

	// 버튼의 INPUT을 Interrupt로 사용하기 위해
	// EXTI에 연결, 설정 및 Enable (Falling 모드)
	exti_btn.EXTI_LineCmd = ENABLE;
	exti_btn.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_btn.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_btn.EXTI_Line = (EXTI_Line11);
	//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11);
	EXTI_Init(&exti_btn);
}

void NVIC_Config() {

/*	nvic.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic);

	nvic.NVIC_IRQChannel = EXTI2_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic);

	//
	nvic.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&nvic);

	// UART 입력에 대한 Interrupt 우선순위를 설정하고 handler 연결
	nvic.NVIC_IRQChannel = USART1_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic);*/

	nvic_up.NVIC_IRQChannel = EXTI9_5_IRQn;
	nvic_up.NVIC_IRQChannelCmd = ENABLE;
	nvic_up.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_up.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_up);

	nvic_down.NVIC_IRQChannel = EXTI2_IRQn;
	nvic_down.NVIC_IRQChannelCmd = ENABLE;
	nvic_down.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_down.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&nvic_down);

	//
	nvic_btn.NVIC_IRQChannel = EXTI15_10_IRQn;
	nvic_btn.NVIC_IRQChannelCmd = ENABLE;
	nvic_btn.NVIC_IRQChannelPreemptionPriority = 3;
	nvic_btn.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&nvic_btn);

	// UART 입력에 대한 Interrupt 우선순위를 설정하고 hanlder 연결
	nvic_USART1.NVIC_IRQChannel = USART1_IRQn;
	nvic_USART1.NVIC_IRQChannelCmd = ENABLE;
	nvic_USART1.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_USART1.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_USART1);

}
void delay() {
	int i = 0;
	for (i = 0; i < 1000000; i++);
}

void EXTI2_IRQHandler(void) {
//	if(EXTI_GetITStatus(EXTI_Line2)!= RESET){
//		counter--;
//		SendData(counter);
		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
	HAL_GPIO_EXTI_Callback(GPIO_Pin_2);
}
void EXTI9_5_IRQHandler(void){
//	if(EXTI_GetITStatus(EXTI_Line5)!= RESET){
//		counter--;
		EXTI_ClearITPendingBit(EXTI_Line5);
//	}
//		HAL_GPIC_EXTI_Callback(GPIO_PIN_5);
	HAL_GPIO_EXTI_Callback(GPIO_Pin_5);
}

void USART1_IRQHandler(void){
	counter = USART_ReceiveData(USART1) - '0';
	SendData(counter);

	PORTD_RESET = 0x9C;
	if(counter & 1){
		PORTD_SET = 0x4;
	}
	if(counter & 2){
		PORTD_SET = 0x8;
	}

	if(counter & 4){
		PORTD_SET = 0x10;
	}
	if(counter & 8){
		PORTD_SET = 0x80;
	}
	delay();
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_Pin_5){
		counter++;
		if(counter > 15) counter = 0;
		SendData(counter);

		PORTD_RESET = 0x9C;
		if(counter & 1){
			PORTD_SET = 0x4;
		}
		if(counter & 2){
			PORTD_SET = 0x8;
		}
		if(counter & 4){
			PORTD_SET = 0x10;
		}
		if(counter & 8){
			PORTD_SET = 0x80;
		}
		delay();
	}else if(GPIO_Pin == GPIO_Pin_2){

		counter--;
		if(counter < 0) counter = 15;
		SendData(counter);

		PORTD_RESET = 0x9C;
		if(counter & 1){
			PORTD_SET = 0x4;
		}
		if(counter & 2){
			PORTD_SET = 0x8;
		}
		if(counter & 4){
			PORTD_SET = 0x10;
		}
		if(counter & 8){
			PORTD_SET = 0x80;
		}
		delay();
	}
}
int main() {
	SystemInit();
	gpio_set();
	usart_Set();
	EXTI_Config();
	NVIC_Config();

	while (1) {
		;
	}
}
