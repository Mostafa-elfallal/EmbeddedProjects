/*
 * keypad.c
 *
 * Created: 6/30/2021 8:57:37 PM
 *  Author: Darsh
 */ 
#include "keypad.h"
#include <avr/io.h>
void KEYPAD_init(){
	KEY_DDR = 0xF0;
	KEY_PRT = 0xFF;
}
typedef enum{
	NOT_PRESSED,
	PRESSED
	}e_KeyPadState_t;
int KEYPAD_read(){
	static e_KeyPadState_t e_KeyPadState = NOT_PRESSED;
	KEY_PRT &= 0x0F ;
	unsigned char firstreading = KEY_PIN & 0x0F ;
	if(	firstreading == 0x0F ){
		e_KeyPadState = NOT_PRESSED;
		return -1;
	}
	else{
		if(e_KeyPadState == PRESSED)
			return -1;
		firstreading = KEY_PIN & 0x0F;
		for(unsigned char i = 0;i<4;i++){
			KEY_PRT =0xFF - (1<<(i+4));
			firstreading = KEY_PIN & 0x0F;
			if (firstreading != 0x0f )			//in this row
			{
				int j=0;
				firstreading =0x0F - firstreading;
				for(;j<4 &&( firstreading != 1);j++){
					firstreading>>=1;
				}
				e_KeyPadState = PRESSED;
				return j*4+i;	
			}
		}
	
	}
	return -1;
}