#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void My_PWM_Init(u16 arr,u16 psc);

#endif

