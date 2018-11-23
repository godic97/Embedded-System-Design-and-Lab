/*
 * team3.c
 *
 *  Created on: 2018. 9. 19.
 *      Author: Team03
 */
#include <stdio.h>

#define RCC_APB2ENR (*(volatile unsigned *)0x40021018)
#define PORTD (*(volatile unsigned *)0x40011400)
#define PORTD_HIGH (*(volatile unsigned *)0x40011404)
#define PORTD_SET (*(volatile unsigned *)0x40011410)
#define PORTD_RESET (*(volatile unsigned *)0x40011414)
#define PORTD_DATA (*(volatile unsigned *)0x40011408)
#define PORTC (*(volatile unsigned *)0x40011000)
#define PORTC_SET (*(volatile unsigned *)0x40011010)
#define PORTC_RESET (*(volatile unsigned *)0X40011414)
#define PORTC_DATA (*(volatile unsigned *)0x40011008)
#define PORTB (*(volatile unsigned *)0x40010C04)
#define PORTB_DATA (*(volatile unsigned *)0x40010C08)
#define PORTB_SET (*(volatile unsigned *)0x40010C10)
// 0000 0000 0100 0100 0100 0100 0000 0000
static void delay(volatile unsigned int nTime){
	for(; nTime > 0; nTime--);
}

int main(){
    // (*(volatile unsigned*)0x40021018) |= 0x38;
	RCC_APB2ENR = 0x38;

	PORTD = 0x30033300;
	PORTD_HIGH = 0x00008000;
	PORTB = 0x8;
	PORTB_SET = 0x100;
	//PORTD_SET &= 0x00000000;
	//PORTD_RESET &= 0x00000000;
//	PORTD_SET = 0x0; // first button
//	PORTD_RESET = 0x0;
	PORTC = 0x00888800;
	PORTC_SET = 0x3C; // 0011 1100
	int i = 0;
	while(1){
		if(PORTB_DATA == 0xD3){
			PORTD_RESET = 0x9C;
			i=0;
		}
		else if(PORTC_DATA == 0x0000FF38){
			PORTD_SET = 0x8;
//			delay(5000000);
//			PORTC_RESET = 0x80;
			PORTC_DATA = 0x00000000;
		}
		else if(PORTC_DATA == 0x0000FF34){	// 0000 1000 l
			PORTD_SET = 0x10;
//			delay(5000000);
//			PORTC_RESET = 0x10;
			PORTC_DATA = 0x00000000;
		}
		else if(PORTC_DATA == 0x0000FF2C ){	// 0001 0000 /r
			PORTD_SET = 0x80;
//			delay(5000000);
//			PORTC_RESET = 0x8;
			PORTC_DATA = 0x00000000;
		}
		else if(PORTC_DATA == 0x0000FF1C ){	// 0010 0000 /d
			PORTD_SET = 0x4;
//			delay(5000000);
//			PORTC_RESET = 0x4;
			PORTC_DATA = 0x00000000;
		}
		else if(~(PORTD_DATA)&0x00000800){
			i++;
			if(i > 15) i = 0;
			PORTD_RESET = 0x9C;
			if(i & 1){
				PORTD_SET = 0x4;
			}
			if(i & 2){
				PORTD_SET = 0x8;
			}
			if(i & 4){
				PORTD_SET = 0x10;
			}
			if(i & 8){
				PORTD_SET = 0x80;
			}
			delay(5000000);

		}

	}

//
//	if(Joystic_up){
//		PORTD_SET = 0x04; // first led
//	}
//	else if(Joystic_down){
//		PORTD_SET = 0x04; // second led
//	}
//	else if(Joystic_left){
//		PORTD_SET = 0x04; // third led
//	}
//	else if(Joystic_right){
//		PORTD_SET = 0x04; // fourth led
//	}
//	else if(Joystic_selection){
//
//	}
//
	//PORTD_SET = 0x04;
	//PORTD_SET = 0x04;
}
