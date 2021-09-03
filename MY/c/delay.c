
#include "delay.h"
//把"stm32f10x_it.h"里的systick_hanlder注释掉！

//#define Delay_ms(x) Delay_us(1000*x)//单位ms

static __IO u32 TimingDelay;


void delay_Init(void)
{
	 // SystemFrequency / 1000     1ms中断一次
	 // SystemFrequency / 100000	 10us中断一次
	 // SystemFrequency / 1000000  1us中断一次
	 
	if (SysTick_Config(SystemCoreClock / 1000000))	// 1us ST3.5.0库版本
	{ 
		/* Capture error */ 
		//NVIC_SetPriority(SysTick_IRQn, 0x0);
		while (1);
	}
}



void Delay_us(__IO u32 nTime)    //单位10us
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/*-------------------------------------------------------------------------------
程序名称：TimingDelay_Decrement
程序描述：数字递减直到0  
输入参数：无
返回参数：无
备    注：需要在中断函数SysTick_Handler()中调用
---------------------------------------------------------------------------------*/

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

