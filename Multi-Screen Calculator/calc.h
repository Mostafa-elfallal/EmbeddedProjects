/*
 * calc.h
 *
 * Created: 7/6/2021 3:41:08 PM
 *  Author: Darsh
 */ 
#define CLEAR	0
#define	SIGN	1
#define OPER	2
#define NUM		3
#define EQUAL	4
#ifndef CALC_H_
#define CALC_H_
typedef enum{
	ANSWER_PROVIDED,
	NOANSWER_PROVIDED
}e_OutputState_t;
typedef enum{
	WAITING_1ST_SIGN,
	WAITING_1ST_NUMBER,
	WAITING_OP,
	WAITING_2ND_SIGN,
	WAITING_2ND_NUMBER,
	WAITING_EQUAL
}e_InputState_t;
typedef struct{
	char FirstInput[10];				//sign + 8 max char input
	char SecondInput[10];
	char Output[18];					//sign + max 16 char
	char Oper;							// + , - , * , /
	e_OutputState_t e_OutputState;		// the current state of output
	e_InputState_t	e_InputState;
}s_CalcStatus_t;
int charcheck(char );
void docalc(s_CalcStatus_t * ps_CalcStatus);
void calcabort(s_CalcStatus_t *);
int IsLegal(s_CalcStatus_t * ps_CalcStatus,int b);
void calcadd(s_CalcStatus_t * ps_CalcStatus,char a);
#endif /* CALC_H_ */