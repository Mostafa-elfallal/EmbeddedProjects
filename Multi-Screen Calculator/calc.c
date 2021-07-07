/*
 * calc.c
 *
 * Created: 7/6/2021 3:40:55 PM
 *  Author: Darsh
 */ 
#include "calc.h"
#include  <string.h>
#include "mathes.h"
/*
* function that returns a rank for keypad characters
*/
int charcheck(char a){
	if (a=='c')			
		return	CLEAR;
	else if (a == '+' || a == '-')
		return	SIGN;
	else if (a == '*' || a == '/')
		return OPER;
	else if (a=='=')
		return EQUAL;
	else
	return	NUM;	
}

void resetcalc(s_CalcStatus_t * ps_CalcStatus){
			ps_CalcStatus->FirstInput[0]='0';
			ps_CalcStatus->SecondInput[0]='0';
			memset(&((ps_CalcStatus->FirstInput)[1]),0,9);
			memset(&((ps_CalcStatus->SecondInput)[1]),0,9);			
}
void calcabort(s_CalcStatus_t * ps_CalcStatus){
		ps_CalcStatus->e_InputState=WAITING_1ST_SIGN;
		ps_CalcStatus->e_OutputState=NOANSWER_PROVIDED;
		resetcalc(ps_CalcStatus);
		ps_CalcStatus->Output[0]='0';
		memset(&((ps_CalcStatus->Output)[1]),0,17);
}
int IsLegal(s_CalcStatus_t * ps_CalcStatus,int b){
	switch(b){
		case SIGN:
			if(ps_CalcStatus->e_InputState==WAITING_1ST_SIGN || ps_CalcStatus->e_InputState == WAITING_2ND_SIGN || ps_CalcStatus->e_InputState == WAITING_OP)
				return 1;
			else
				return 0;
		break;
		case OPER:
			if (ps_CalcStatus->e_InputState == WAITING_OP)
				return 1;
			else
				return 0;
		break;
		case EQUAL:
			if (ps_CalcStatus->e_InputState != WAITING_1ST_NUMBER && ps_CalcStatus->e_InputState != WAITING_2ND_NUMBER )
			return 1;
			else
			return 0;
		default:
			
		return 0;
	}
}
void calcadd(s_CalcStatus_t * ps_CalcStatus,char a){
	int ind;
	int checker = charcheck(a);
	switch(ps_CalcStatus->e_InputState){
		case WAITING_1ST_SIGN:
			if(checker == NUM){
				ps_CalcStatus->e_InputState=WAITING_OP;
				ps_CalcStatus->FirstInput[0]=a;
			}
			else{
				ps_CalcStatus->e_InputState=WAITING_1ST_NUMBER;
				ps_CalcStatus->FirstInput[0]=a;
			}
		break;
		case WAITING_1ST_NUMBER:
			ps_CalcStatus->FirstInput[1]=a;
			ps_CalcStatus->e_InputState=WAITING_OP;
		break;
		case WAITING_OP:
			if(checker == NUM){
				ind = strlen(ps_CalcStatus->FirstInput);
				ps_CalcStatus->FirstInput[ind]=a;
			}
			else{
				ps_CalcStatus->Oper=a;
				ps_CalcStatus->e_InputState=WAITING_2ND_SIGN;
			}
		break;
		case WAITING_2ND_SIGN:
			if(checker == NUM){
				ps_CalcStatus->e_InputState=WAITING_EQUAL;
				ps_CalcStatus->SecondInput[0]=a;
			}
			else{
				ps_CalcStatus->e_InputState=WAITING_2ND_NUMBER;
				ps_CalcStatus->SecondInput[0]=a;
			}
		break;
		case WAITING_2ND_NUMBER:
			ps_CalcStatus->SecondInput[1]=a;
			ps_CalcStatus->e_InputState=WAITING_EQUAL;
		break;
		case WAITING_EQUAL:
				ind = strlen(ps_CalcStatus->SecondInput);
				ps_CalcStatus->SecondInput[ind]=a;
		break;
	}
}
void docalc(s_CalcStatus_t * ps_CalcStatus){
	switch(ps_CalcStatus->Oper){
		case '+':
			provideAdd(ps_CalcStatus);
		break;
		case '-':
			provideSub(ps_CalcStatus);
		break;
		case '*':
			provideMul(ps_CalcStatus);
		break;
		case '/': 
			provideDiv(ps_CalcStatus);
		break;
		default:
		break;
	}
	ps_CalcStatus->e_InputState=WAITING_OP;
	ps_CalcStatus->e_OutputState=ANSWER_PROVIDED;
	resetcalc(ps_CalcStatus);
}