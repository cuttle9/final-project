#include "LPC11xx.h"
#include "UART.h"

volatile uint8_t UART_Flag;
uint8_t UART_Buff[10];
uint32_t UART_Num;

void UART_Init (uint32_t UART_BPS)
{
	uint16_t DD;
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);
	LPC_IOCON->PIO1_6 &=~0x07;
	LPC_IOCON->PIO1_6 |= (1<<0);
	LPC_IOCON->PIO1_7 &=~0x07;
	LPC_IOCON->PIO1_7 |= (1<<0);
	LPC_SYSCON->SYSAHBCLKCTRL &=~(1<<16);
	LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
	LPC_SYSCON->UARTCLKDIV = 0x01;
	LPC_UART->LCR = 0x83;
	DD = SystemCoreClock / LPC_SYSCON->UARTCLKDIV / 16 / UART_BPS;
	LPC_UART->DLM = DD / 256;
	LPC_UART->DLL = DD % 256;
	LPC_UART->LCR = 0x03;
	LPC_UART->FCR = 0x87;
	NVIC_EnableIRQ(UART_IRQn);
	NVIC_SetPriority(UART_IRQn , 0);
	LPC_UART->IER = 0x01;
}


uint8_t UART_GetByte (void)
{
	uint8_t RData;
	while((LPC_UART->LSR&0x01) == 0);
	RData = LPC_UART->RBR;
	return RData;
}


void UART_SendByte (uint8_t TData)
{
	LPC_UART->THR = TData;
	while((LPC_UART->LSR & 0x40) == 0);
}


void UART_SendStr (char *PStr)
{
	while(1)
	{
		if(*PStr == '\0') break;
		UART_SendByte(*PStr++);
	}
}


void UART_IRQHandler (void)
{
	UART_Num = 0;
	while((LPC_UART->IIR&0x01) == 0)
	{
		switch(LPC_UART->IIR&0x0e)
		{
			case 0x04:
				UART_Flag = 1;
			  for(UART_Num=0;UART_Num<8;UART_Num++)
					UART_Buff[UART_Num]=LPC_UART->RBR;
				break;
			
			case 0x0c:
				UART_Flag = 1;
				while((LPC_UART->LSR&0x01) == 0x01)
					UART_Buff[UART_Num++]=LPC_UART->RBR;
				break;
				
			default:
				break;	
		}
	}
}
