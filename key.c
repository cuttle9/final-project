#include "KEY.h"
 
 uint8_t KEY = 1;

void KEY_Inti (void)
{
	LPC_GPIO3->DIR = 0x00;
	LPC_GPIO3->IE |= (1<<4);	
	LPC_GPIO3->IS &= ~(1<<4);	
	LPC_GPIO3->IEV &= ~(1<<4);	
	NVIC_EnableIRQ(EINT3_IRQn);	
}


void PIOINT3_IRQHandler (void)
{
	uint32_t i,x;
	for(i=0;i<2500;i++)	
	for(x=0;x<100;x++);	
	if((LPC_GPIO3->MIS&(1<<4))==(1<<4))
	{
		if(KEY==0)	KEY=1;	
		else	KEY=0;
		LPC_GPIO3->IC = (1<<4);
	}
}
