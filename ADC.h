#ifndef __ADC_H__
#define __ADC_H__

#include "LPC11xx.h"

void ADC_Init (void);
extern uint32_t ADC_Read(uint8_t C);
uint32_t ADC_Get (void);
extern float MF52 (uint32_t ADdat);

#endif