#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

//flash load "C:\Users\Team02\Desktop\team02\week11\Debug\week11.axf"
//flash load "C:\Users\Team02\Desktop\team02\week11\Debug\flashclear.axf"
void week11_init(void);
void delay(int i);
int flag_LED = 0;
int flag_ONOFF = 0;

uint16_t count = 0;
uint16_t mil = 0;
uint16_t sec = 0;
uint16_t min = 0;

uint16_t pos_x,pos_y;
uint16_t pix_x,pix_y;
// 버튼의 위치를 저장하는 변수
int button_pos[4][2];
// led 켜지고 꺼진 상태 토글
int button[] = {0, 0, 0, 0};
void delay(int i){
   int j;
   for(j=0; j<=i * 100000; j++);
}

int main(){
   int rSize = 15;
   int i;
//   char str[10];
//   int prevCircleX = -1, prevCircleY = -1;

   SystemInit();
   LCD_Init();
   Touch_Configuration();
   Touch_Adjust();
   LCD_Clear(WHITE);
   week11_init();
   while(1){
      Touch_GetXY(&pos_x, &pos_y, 0);
      Convert_Pos(pos_x, pos_y, &pix_x, &pix_y);
      LCD_ShowString(50, 50, "WED_TEAM03", BLACK, WHITE);
      	// LED1
      	LCD_DrawRectangle(30, 180, 80, 210);
      	LCD_ShowString(35, 185, "START", BLACK, WHITE);
      	// LED2
      	LCD_DrawRectangle(120, 180, 170, 210);
      	LCD_ShowString(125, 185, "STOP", BLACK, WHITE);
      	//LED3
      	LCD_DrawRectangle(30, 220, 80, 250);
      	LCD_ShowString(35, 225, "RESET", BLACK, WHITE);


      	if((pix_x > 30 && pix_x < 80) && (pix_y > 180 && pix_y < 210)){
      		flag_ONOFF = 1;
      	}
      	else if((pix_x > 120 && pix_x < 170) && (pix_y > 180 && pix_y < 210)){
      		flag_ONOFF = 0;
      	}
      	else if((pix_x > 30 && pix_x < 80) && (pix_y > 220 && pix_y < 250)){
			count = 0;
			min = 0;
			sec = 0;
			mil = 0;
      	}

        LCD_ShowNum(200, 20, min, 2, BLACK, WHITE);
        LCD_ShowNum(200, 50, sec, 2, BLACK, WHITE);
        LCD_ShowNum(200, 80, mil, 2, BLACK, WHITE);

      }

}

void TIM2_IRQHandler(void) {
   if(flag_ONOFF == 1){
	   count = count + 1;

	   mil = count % 100;
	   sec = count % 6000 /100;
	   min = count / 6000;

	   if(count == 360000)
		   count = 0;

      }

   if(sec % 2 == 0){
	   GPIO_SetBits(GPIOD, GPIO_Pin_2);
	   GPIO_SetBits(GPIOD, GPIO_Pin_3);
	   GPIO_SetBits(GPIOD, GPIO_Pin_4);
	   GPIO_SetBits(GPIOD, GPIO_Pin_7);
   }
   else{
	   GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	   GPIO_ResetBits(GPIOD, GPIO_Pin_7);
  }
   TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);

}


void week11_init(void){
   GPIO_InitTypeDef GPIOD_Init;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
   NVIC_InitTypeDef NVIC_InitStructure;
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPDEN, ENABLE);
   //LED Init
   GPIOD_Init.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7 ;
   GPIOD_Init.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIOD_Init.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOD, &GPIOD_Init);
   //TIM2 Init
   TIM_TimeBaseStructure.TIM_Period = 120-1;
   TIM_TimeBaseStructure.TIM_Prescaler = 6000-1;
   // 시스템 클락 72mhz
   //디지털 필터에 사용됨. 0으로 설정시 사용하지 않음
   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
   //타이머 카운터를 up, down으로 설정
   //up으로 설정시 0부터 카운터
   //down으로 설정시 period값에서 부터 카운터
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
   // TIM2 NVIC Init
   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   //TIM2 Enable
   TIM_ARRPreloadConfig(TIM2, ENABLE);
   TIM_Cmd(TIM2, ENABLE);
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // interrupt enable
}

