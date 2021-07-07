/*
 * keypad.h
 *
 * Created: 6/30/2021 8:57:24 PM
 *  Author: Darsh
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_
#define KEY_PRT 	PORTA
#define KEY_DDR		DDRA
#define KEY_PIN		PINA

void KEYPAD_init();
int KEYPAD_read();		//returns the (row * 4 + column) or -1 if not pressed


#endif /* KEYPAD_H_ */