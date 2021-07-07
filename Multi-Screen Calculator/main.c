/*
 * unit7-lesson2.c
 *
 * Created: 6/30/2021 8:56:08 PM
 * Author : Darsh
 */ 

#include <avr/io.h>
#define  F_CPU 1000000UL
#include <util/delay.h>
#include "LCD.h"
#include "keypad.h"
#include "calc.h"

const unsigned char keys[16] = {
								'7','8','9','/',
								'4','5','6','*',
								'1','2','3','-',
								'c','0','=','+'
								};
const s_LCD_Config_t InputLCD =
{
	{
		(unsigned char *)&PORTC,
		(unsigned char *)&DDRC,
		(unsigned char *)&PINC,
		5,6,7
	},
	{
		(unsigned char *)&PORTD,
		(unsigned char *)&DDRD,
		(unsigned char *)&PIND,
		0,3
	},
	NOSHIFT_INCREMENT,
	ONDISP_OFFCURSOR,
	FOURBIT_2LINE_7DOT
	
};
const s_LCD_Config_t OutputLCD =
{
	{
		(unsigned char *)&PORTC,
		(unsigned char *)&DDRC,
		(unsigned char *)&PINC,
		0,1,2
	},
	{
		(unsigned char *)&PORTD,
		(unsigned char *)&DDRD,
		(unsigned char *)&PIND,
		4,7
	},
	NOSHIFT_INCREMENT,
	ONDISP_OFFCURSOR,
	FOURBIT_2LINE_7DOT
	
};	
s_CalcStatus_t s_CalcStatus={
	"0","0","0",'\0',NOANSWER_PROVIDED,WAITING_1ST_SIGN
};
int InputLCD_counter =0;
void badinput(){
	LCD_sendchar(&InputLCD,'X',DATA);
	_delay_ms(200);
	LCD_shift(&InputLCD,CURSOR_LEFT);
	LCD_sendchar(&InputLCD,' ',DATA);
	LCD_shift(&InputLCD,CURSOR_LEFT);
}
void addchar(s_CalcStatus_t *ps_CalcStatus,int ch_index){
	calcadd(ps_CalcStatus,keys[ch_index]);
	if(InputLCD_counter == 16){
		LCD_gotoxy(&InputLCD,0,1);
	}
	InputLCD_counter++;
	LCD_sendchar(&InputLCD,keys[ch_index],DATA);
}
int main(void)
{
	
	LCD_init(&InputLCD);
	LCD_init(&OutputLCD);
	KEYPAD_init();
	int ch_index;
	int numcount =0;
	LCD_sendstring(&OutputLCD,"NO ANSWER PROVIDED.");
    while (1) 
	{
		ch_index = KEYPAD_read();
		if(ch_index >= 0){					// is there an input ?
			switch(charcheck(keys[ch_index])){
				case CLEAR:
					InputLCD_counter =0;
					numcount =0;
					calcabort(&s_CalcStatus); 
					LCD_clearscreen(&InputLCD);
					LCD_clearscreen(&OutputLCD);
					LCD_sendstring(&OutputLCD,"NO ANSWER PROVIDED.");
				break;
				case SIGN:
				case OPER:
					if (s_CalcStatus.e_InputState== WAITING_EQUAL)
					{
						numcount =0;
						docalc(&s_CalcStatus);
						LCD_clearscreen(&InputLCD);
						LCD_sendstring(&InputLCD,"ANS");
						InputLCD_counter=3;
						LCD_clearscreen(&OutputLCD);
						LCD_sendstring(&OutputLCD,s_CalcStatus.Output);
						addchar(&s_CalcStatus,ch_index);
					}
					else{
						if(IsLegal(&s_CalcStatus,charcheck(keys[ch_index])))
						{
							numcount =0;
							addchar( &s_CalcStatus, ch_index);
						}
						else
						{
							badinput();
						}
					}
				break;
				case NUM:
				/*
					if(InputLCD_counter!=9 && InputLCD_counter!=19)
					{
						addchar( &s_CalcStatus, ch_index);
					}
					else
					{
						badinput();
					}
					*/
				
					if(numcount == 8 || (s_CalcStatus.e_InputState==WAITING_OP  && s_CalcStatus.e_OutputState==ANSWER_PROVIDED )){
						badinput();
					}
					else{
						numcount++;
						addchar( &s_CalcStatus, ch_index);
					}
				break;
				default:
					if(IsLegal(&s_CalcStatus,charcheck(keys[ch_index])))
					{
						docalc(&s_CalcStatus);
						LCD_clearscreen(&OutputLCD);
						LCD_sendstring(&OutputLCD,s_CalcStatus.Output);
						LCD_clearscreen(&InputLCD);
						LCD_sendstring(&InputLCD,"ANS");
						ch_index=3;
					}
					else
					{
						badinput();
					}
				break;
			}
		}
	}

}