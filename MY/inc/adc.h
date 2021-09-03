#ifndef __ADC_H
#define	__ADC_H

#include "stm32f10x.h"

extern __IO uint16_t ADC_ConvertedValue;
extern volatile uint16_t ADC[10][3];

void ADC1_Init(void);

#endif /* __ADC_H */
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
