
#include <stdio.h>
#include "stm32f10x.h"
#include "led.h"
#include "usart.h"

int main(void)
{	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	usart_init();
	LED_Init();
	LED2_ON;
	LED3_OFF;
  while (1);
	
}
