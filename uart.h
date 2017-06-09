#ifndef __UART_H__
#define __UART_H__

#include "LPC11xx.h"

extern void UART_Init (uint32_t UART_BPS);
extern uint8_t UART_GetByte (void);
extern void UART_SendByte (uint8_t TData);
extern void UART_SendStr (char *PStr);

#endif