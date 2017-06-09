#include "LPC11xx.h"                    
#include "key.h"
#include "ADC.h"
#include "uart.h"
#include "camera.h"
extern volatile uint8_t UART_Flag;
char AD_Data_Buff[20];
extern uint8_t KEY;



int main (void)
{
	float Temp,buff; 
	uint32_t i;
	UART_Init(9600);
	SMG_Init();
	ADC_Init();
	KEY_Inti();
	while(1)
	{
		buff=ADC_Get();		
		Temp=MF52(buff);	
		if(KEY==1)			
			for(i=0;i<100;i++)
				SMG((int)Temp);
		else						
			for(i=0;i<100;i++)
				SMG((int)(Temp*1.8+32));
		if(UART_Flag==1)	
		{
			UART_Flag = 0;
			sprintf(AD_Data_Buff , "Temp--%4.2f \r\n" , Temp);	
			UART_SendStr(AD_Data_Buff);
		}
	}	
}