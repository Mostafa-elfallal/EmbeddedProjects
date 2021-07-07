/*
 * LCD.c
 *
 * Created: 6/30/2021 8:56:53 PM
 *  Author: Darsh
 */ 
#define BITSET(reg,bit) (reg |=(1<< bit))
#define BITCLEAR(reg,bit) (reg &= ~(1<<bit))
#include "LCD.h"
#define ClEAR_SCREEN			0x01
#define RETURN_HOME				0x02
#define ENTRY_MODE_SET			0x04
#define	DISPLAY_CONTROL			0x08
#define CURSOR_DISPLAY_SHIFT	0x10
#define FUNCTION_SET			0x20
#define SET_CG_ADDRESS			0x40
#define SET_DD_ADDRESS			0x80
void LCD_kick(s_LCD_Config_t const * LCD){
	BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.enpin);									//enable pulse
	_delay_ms(1);
	BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.enpin);
}
void LCD_init(s_LCD_Config_t const * LCD ){
	*(LCD->s_LCDCtrlPins.p_DataDireReg) |= (1<<LCD->s_LCDCtrlPins.enpin) | (1<<LCD->s_LCDCtrlPins.rspin) | (1<<LCD->s_LCDCtrlPins.rwpin); 
	if(LCD->e_OperMode < EIGHTBIT_1LINE_7DOT)	// 4 bits	
	{			
		*(LCD->s_LCDDataPins.p_DataDireReg) |= (0x0F << LCD->s_LCDDataPins.DataPinStart);
	}
	else 
	{
		*(LCD->s_LCDDataPins.p_DataDireReg) |= (0xFF << LCD->s_LCDDataPins.DataPinStart);
	}
	LCD_sendchar(LCD ,0x02 , COMMAND );
	LCD_sendchar(LCD ,FUNCTION_SET + LCD->e_OperMode , COMMAND );
	LCD_sendchar(LCD ,DISPLAY_CONTROL + LCD->e_DispCtrl , COMMAND );
	LCD_sendchar(LCD ,ENTRY_MODE_SET + LCD->e_EntryMode , COMMAND );
	LCD_clearscreen(LCD);
	LCD_returnhome(LCD);
	LCD_sendchar(LCD ,0x80 , COMMAND );
}
void LCD_sendchar(s_LCD_Config_t const * LCD ,unsigned char data ,e_LCDCharType_t e_LCDCharType)
{
	LCDIsbusy(LCD);
	if(LCD->e_OperMode < EIGHTBIT_1LINE_7DOT)	// 4 bits	
	{
			BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rwpin);									// RW 0 for writing
			*(LCD->s_LCDDataPins.p_PortReg) &= ~(0x0F << LCD->s_LCDDataPins.DataPinStart);						// clear the data pins
			*(LCD->s_LCDDataPins.p_PortReg) = (((data & 0xF0) >> 4) << LCD->s_LCDDataPins.DataPinStart);		//the high nibble
				
			if(e_LCDCharType == COMMAND)
				BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			else
				BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
						
			LCD_kick(LCD);

			*(LCD->s_LCDDataPins.p_PortReg) &= ~(0x0F << LCD->s_LCDDataPins.DataPinStart);
			*(LCD->s_LCDDataPins.p_PortReg) |= ((data & 0x0F) << LCD->s_LCDDataPins.DataPinStart);
				
			LCD_kick(LCD);
			_delay_ms(2);
	}
	else
	{						//8bit mode
			*(LCD->s_LCDDataPins.p_PortReg) &= ~(0xFF << LCD->s_LCDDataPins.DataPinStart);
			*(LCD->s_LCDDataPins.p_PortReg) |= (data << LCD->s_LCDDataPins.DataPinStart);
			if(e_LCDCharType == COMMAND)
				BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			else
				BITSET(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rspin);
			BITCLEAR(*(LCD->s_LCDCtrlPins.p_PortReg),LCD->s_LCDCtrlPins.rwpin);
			LCD_kick(LCD);
	}

}
void LCD_sendstring(s_LCD_Config_t const * LCD , const char *  data)
{
	int counter=0;
	for(;*data != '\0';data++){
		if(counter==16){
		LCD_gotoxy(LCD,0,1);
		}
		LCD_sendchar(LCD,*data,DATA);
		counter ++;
	}
}
void LCDIsbusy(s_LCD_Config_t const * LCD)
{
	//currently I'm working on reading the actual busy flag 
	//for the end of the dev process xD let it simply a delay.
	_delay_ms(1);
}

void LCD_clearscreen(s_LCD_Config_t const * LCD)
{
	LCD_sendchar(LCD , ClEAR_SCREEN , COMMAND );
}
void LCD_returnhome(s_LCD_Config_t const * LCD)
{
	LCD_sendchar(LCD , RETURN_HOME , COMMAND );
}
void LCD_gotoxy(s_LCD_Config_t const * LCD , unsigned char x , unsigned char y)
{
	LCD_sendchar(LCD,SET_DD_ADDRESS+(0x40*y)+x,COMMAND);
}
void LCD_buildcustom(s_LCD_Config_t const * LCD ,unsigned char location ,const unsigned char * pattern )
{
	unsigned char i=0;
	if(location<8){
		LCD_sendchar(LCD,0x40+(location*8),COMMAND);
		for(i=0;i<8;i++)
		LCD_sendchar(LCD,pattern[ i ],DATA);
	}
}
void LCD_dispcustom(s_LCD_Config_t const * LCD ,unsigned char location){
	LCD_sendchar(LCD,location,DATA);
}
void LCD_shift(s_LCD_Config_t const * LCD ,e_ShiftDir_t e_ShiftDir){
	LCD_sendchar(LCD ,CURSOR_DISPLAY_SHIFT + e_ShiftDir , COMMAND );
}