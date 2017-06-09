#include "LPC11xx.h"
#include "ADC.h"

uint32_t ADCBuf,ADCFlag;
uint8_t Buff[20];


void ADC_Init (void)
{
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);	
	LPC_IOCON->PIO1_11 &=~0xbf;	
	LPC_IOCON->PIO1_11 |= 0x01;	
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);	
	LPC_SYSCON->PDRUNCFG &=~(0x01<<4);		
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<13);	  
	LPC_ADC->CR = (0x01<<7)|((SystemCoreClock/1000000-1)<<8)|(0<<16)|(0<<17)|(0<<24)|(0<<27);	
}


uint32_t ADC_Read(uint8_t C)
{
	uint32_t adc_value=0;
	uint32_t ulADCBuf=0;
	uint8_t i;
	
	for(i=0;i<10;i++)
	{
		adc_value = ((LPC_ADC->DR[C]>>6)&0x3FF);			    
		ulADCBuf += adc_value;
	}
	adc_value = ulADCBuf / 10;
	adc_value = (adc_value*3300)/1024; 
	
	return adc_value; 
}


uint32_t ADC_Get (void)
{
	uint32_t ulADCDat=0 , ulADCBuf=0;
	uint8_t i=0;

	for(i=0;i<10;i++)
	{
		LPC_ADC->CR |= (1<<24);
		while((LPC_ADC->DR[7]&0x80000000) == 0);
		LPC_ADC->CR |= (1<<24);
		while((LPC_ADC->DR[7]&0x80000000) == 0);
		ulADCBuf = LPC_ADC->DR[7];
		ulADCBuf = (ulADCBuf>>6)&0x3ff;
		ulADCDat += ulADCBuf;
	}
	ulADCDat = ulADCDat / 10;
	ulADCDat = (ulADCDat*3300)/1024;
	
	return ulADCDat;
}


void ADC_IRQHandler (void)
{
	ADCBuf = LPC_ADC->DR[1];
	ADCBuf = (ADCBuf>>6)&0x3ff;
	ADCFlag = 1;
}


float MF52 (uint32_t ADdat)
{
	float res_value=0 , t=0;
	float k;
				
	res_value= (ADdat*10000)/(3300-ADdat);    
		if((res_value<33970)&&(res_value>20310))  
	{
			k = 1366;
		  t = (33970-res_value)/k;
	}		
	else if ((res_value<20310)&&(res_value>12570)) 
	{
			k = 774;
		  t = (20310-res_value)/k+10;
	}
	else if ((res_value<12570)&&(res_value>8034)) 
	{
			k = 453.7;
		  t = ((12570-res_value)/k)+20;
	}
	else if ((res_value<8034)&&(res_value>5298))  
	{
			k = 273.7;
		  t = ((8034-res_value)/k)+30;
	}
	else if ((res_value<5298)&&(res_value>3586))  
	{
			k = 171.7;
		  t = ((5298-res_value)/k)+40;
	}
	else if ((res_value<3586)&&(res_value>2484))  
	{   k = 110.2;
		  t = ((3586-res_value)/k)+50;
	}
	
	
	
	return t;
}
