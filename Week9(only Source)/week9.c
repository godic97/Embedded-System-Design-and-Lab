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
NVIC_InitTypeDef nvic, nvic_btn, nvic_up, nvic_down, nvic_sel, nvic_USART1, nvic_USART2;

USART_InitTypeDef usart;

#define PORTD_SET (*(volatile unsigned *)0x40011410)
#define PORTD_RESET (*(volatile unsigned *)0x40011414)

int counter = 3;
int flag = 0;
void USART2_SendData(char data) {
	USART2->DR = data & 0xFF;
//	USART1->DR += '0';
	//데이터를 다 보내는 것을 기다린다]
}

void USART1_SendData(char data) {
	USART1->DR = data & 0xFF;
//	USART1->DR += '0';
	//데이터를 다 보내는 것을 기다린다]
}

// PC-> bluetooth message
void sendData_USART2(){
	char rvData;

	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	rvData = USART_ReceiveData(USART1);

	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2,rvData);
}


void usart_Set()
{
	usart.USART_BaudRate = 9600;

	//data length 8 = 8 bit
	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//패리티 Even
	//usart.USART_Parity = USART_Parity_Even;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Init(USART1, &usart);
	USART_Cmd(USART1, ENABLE);

	usart.USART_WordLength = USART_WordLength_8b;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//패리티 Even
	//usart.USART_Parity = USART_Parity_Even;

	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1;
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Init(USART2, &usart);
	USART_Cmd(USART2, ENABLE);
}

void gpio_set()
{
	// Open Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	// set LED Pin
//	gpD.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpD.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7);
//	gpD.GPIO_Speed = GPIO_Speed_50MHz;
//
//	// set joy stick 2=down,  5=up
//	gpC.GPIO_Mode = GPIO_Mode_IPD;
//	gpC.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_5);
//	gpC.GPIO_Speed = GPIO_Speed_50MHz;

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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART2 TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//USART2 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_Init(GPIOD, & gpD);
	GPIO_Init(GPIOC, & gpC);
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

//	nvic_up.NVIC_IRQChannel = EXTI9_5_IRQn;
//	nvic_up.NVIC_IRQChannelCmd = ENABLE;
//	nvic_up.NVIC_IRQChannelPreemptionPriority = 1;
//	nvic_up.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Init(&nvic_up);
//
//	nvic_down.NVIC_IRQChannel = EXTI2_IRQn;
//	nvic_down.NVIC_IRQChannelCmd = ENABLE;
//	nvic_down.NVIC_IRQChannelPreemptionPriority = 2;
//	nvic_down.NVIC_IRQChannelSubPriority = 2;
//	NVIC_Init(&nvic_down);

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

	// USART2
	nvic_USART2.NVIC_IRQChannel = USART2_IRQn;
	nvic_USART2.NVIC_IRQChannelCmd = ENABLE;
	nvic_USART2.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_USART2.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_USART2);
}

void delay() {
	int i = 0;
	for (i = 0; i < 1000000; i++);
}


void USART1_IRQHandler(void){
	int a;
	a = USART_ReceiveData(USART1);

	USART_SendData(USART2,a);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

/*	char buff[255];
	int idx = 0;
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET){
		while((USART1->SR & USART_SR_RXNE)){
			buff[idx++] = USART_ReceiveData(USART1);
		}
		while(idx >= 0){
			USART_SendData(USART2, buff[idx--]);
		}
	}

	delay();
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);*/
}

void USART2_IRQHandler(void){
	int a;
	a = USART_ReceiveData(USART2);

	USART_SendData(USART1,a);
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);

	/*char buff[255];
	int idx = 0;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
		while((USART2->SR & USART_SR_RXNE)){
			buff[idx++] = USART_ReceiveData(USART2);
		}
		while(idx >= 0){
			USART_SendData(USART1, buff[idx--]);
		}
	}
	delay();
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);*/
}

int main() {
	SystemInit();
	gpio_set();
	usart_Set();
	NVIC_Config();
	while (1) {
		;
	}
}
