#ifndef __delay_H
#define __delay_H

#include "stm32f10x.h"

#define Delay_ms(x) Delay_us(1000*x)//单位ms

void delay_Init(void);

#endif

