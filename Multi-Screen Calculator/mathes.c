/*
 * math.c
 *
 * Created: 7/7/2021 11:12:56 PM
 *  Author: Darsh
 */ 
#include "mathes.h"
#include <stdlib.h>
#include <string.h>
void provideAdd(s_CalcStatus_t *ps_CalcStatus){
	long num1,num2;
	char * endptr;
	if (ps_CalcStatus->e_OutputState==ANSWER_PROVIDED)
	{
		num1=strtol(ps_CalcStatus->Output,&endptr,10);
	}
	else{
		num1=strtol(ps_CalcStatus->FirstInput,&endptr,10);
	}
	num2=strtol(ps_CalcStatus->SecondInput,&endptr,10);
	ltoa(num1+num2,ps_CalcStatus->Output,10);
}
void provideSub(s_CalcStatus_t *ps_CalcStatus){
	long num1,num2;
	char * endptr;
	if (ps_CalcStatus->e_OutputState==ANSWER_PROVIDED)
	{
		num1=strtol(ps_CalcStatus->Output,&endptr,10);
	}
	else{
		num1=strtol(ps_CalcStatus->FirstInput,&endptr,10);
	}
	num2=strtol(ps_CalcStatus->SecondInput,&endptr,10);
	ltoa(num1-num2,ps_CalcStatus->Output,10);
}
void provideMul(s_CalcStatus_t *ps_CalcStatus){
		long num1,num2;
		char * endptr;
		if (ps_CalcStatus->e_OutputState==ANSWER_PROVIDED)
		{
			num1=strtol(ps_CalcStatus->Output,&endptr,10);
		}
		else{
			num1=strtol(ps_CalcStatus->FirstInput,&endptr,10);
		}
		num2=strtol(ps_CalcStatus->SecondInput,&endptr,10);
		ltoa(num1*num2,ps_CalcStatus->Output,10);
	
	
}
void provideDiv(s_CalcStatus_t *ps_CalcStatus){
		long num1,num2;
		char * endptr;
		if (ps_CalcStatus->e_OutputState==ANSWER_PROVIDED)
		{
			num1=strtol(ps_CalcStatus->Output,&endptr,10);
		}
		else{
			num1=strtol(ps_CalcStatus->FirstInput,&endptr,10);
		}
		num2=strtol(ps_CalcStatus->SecondInput,&endptr,10);
		ltoa(num1/num2,ps_CalcStatus->Output,10);
}