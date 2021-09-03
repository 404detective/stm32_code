
#include "pwm.h"

// void TIM3_PWM_Init(u16 arr,u16 psc)
// {
// GPIO_InitTypeDef GPIO_InitStructure;
// TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
// TIM_OCInitTypeDef TIM_OCInitStructure;
// 	
// RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①使能定时器 3 时钟
// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
// 	
// GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //②重映射 TIM3_CH2->PB5
// 	
// //设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5
// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //TIM_CH2
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
// //初始化 TIM3
// TIM_TimeBaseStructure.TIM_Period = arr;  //设置在自动重装载周期值
// TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置预分频值
// TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
// TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
// TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //③初始化 TIMx
// 	
// //初始化 TIM3 Channel2 PWM 模式
// TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
// TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
// TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高

// TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC2
// TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); //使能预装载寄存器
// TIM_Cmd(TIM3, ENABLE);  //⑤使能 TIM3
// }

void My_PWM_Init(u16 arr,u16 psc)
{
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
	
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //①使能定时器 2 时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //①使能 GPIO 和 AFIO 复用功能时钟
	
GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE); //②重映射 TIM3_CH2->PB5
	
//设置该引脚为复用输出功能,输出 TIM3 CH2 的 PWM 脉冲波形 GPIOB.5
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  //TIM_CH2
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure); //①初始化 GPIO
//初始化 TIM3
TIM_TimeBaseStructure.TIM_Period = arr;  //设置在自动重装载周期值
TIM_TimeBaseStructure.TIM_Prescaler =psc;  //设置预分频值
TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数模式
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //③初始化 TIMx
	
//初始化 TIM3 Channel2 PWM 模式
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择 PWM 模式 2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高

TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //④初始化外设 TIM3 OC2
TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能预装载寄存器
TIM_Cmd(TIM2, ENABLE);  //⑤使能 TIM3
}


void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef 		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	//使能GPIO外设时钟
	
 
  //引脚功能配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //TIM2_CH1 TIM2_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出！！
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; 					//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				 //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	 //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 	//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel1、Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  	 //根据T指定的参数初始化外设TIM2 OC1
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  	//根据T指定的参数初始化外设TIM2 OC2
 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}


//TIM4 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能定时器4时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO 
   //初始化TIM4
 TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
 TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
 TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
 //初始化TIM4 Channel/2/3/4 PWM模式  
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
 TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC1
 TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC2
 TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC3
 TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM4 OC4 
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
 TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
 TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR3上的预装载寄存器
 TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR4上的预装载寄存器 
 TIM_ARRPreloadConfig(TIM4,ENABLE);
 TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}
void TIM3_PWM_Init(u16 arr,u16 psc)//PA 6 7对应通道1 2   
{  
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设|RCC_APB2Periph_GPIOB
   //设置该引脚为复用输出功能,输出TIM1 PWM脉冲波形 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CHX
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //TIM_CHX
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);


   //初始化TIM1
 TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
 TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
 TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 //初始化TIM4 Channel/2/3/4 PWM模式  
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
 TIM_OCInitStructure.TIM_OutputNState=TIM_OutputState_Disable;//高级定时器有一个输出N不使能
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
 TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;//
 TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//这三个是高级定时器要配置的
 TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;//
 //TIM_OCInitStructure.TIM_Pulse=10;
 TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
 TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
 //TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
 //TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4 
 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM1在CCR3上的预装载寄存器
 //TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM1在CCR4上的预装载寄存器 
 //TIM_ARRPreloadConfig(TIM3,ENABLE);//使能自动重装载值
 TIM_CtrlPWMOutputs(TIM3,ENABLE);//主输出使能 
 TIM_Cmd(TIM3, ENABLE);  //使能TIM1 
}


void TIM1_PWM_Init(u16 arr,u16 psc)//PA 8  11对应通道1  4   9、10未使用
{  
 GPIO_InitTypeDef GPIO_InitStructure;
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 TIM_OCInitTypeDef  TIM_OCInitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //使能定时器1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设
   //设置该引脚为复用输出功能,输出TIM1 PWM脉冲波形 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11; //TIM_CHX
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
   //初始化TIM1
 TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
 TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
 TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
 TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 //初始化TIM4 Channel/2/3/4 PWM模式  
 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
 TIM_OCInitStructure.TIM_OutputNState=TIM_OutputState_Disable;//高级定时器有一个输出N不使能
 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
 TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;//
 TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;//这三个是高级定时器要配置的
 TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;//
 //TIM_OCInitStructure.TIM_Pulse=10;
 TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
 //TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC2
 //TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC3
 TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4 
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
// TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
 //TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR3上的预装载寄存器
 TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR4上的预装载寄存器 
 TIM_ARRPreloadConfig(TIM1,ENABLE);//使能自动重装载值
 TIM_CtrlPWMOutputs(TIM1,ENABLE);//主输出使能 
 TIM_Cmd(TIM1, ENABLE);  //使能TIM1 
}
//TIM1中的PA8 A9代表左前轮， A10 A11代表左后轮； TIM4中的PB6 B7代表右前轮，B8 B9代表右后轮


void Prun()
{
 TIM_SetCompare1(TIM1,10); //左前前进
 TIM_SetCompare2(TIM1,0); 
 TIM_SetCompare3(TIM1,10); //左后前进
 TIM_SetCompare4(TIM1,0); 

}
void Nrun()
{
 TIM_SetCompare1(TIM1,0); //左前后退
 TIM_SetCompare2(TIM1,10); 
 TIM_SetCompare3(TIM1,0); //左后后退
 TIM_SetCompare4(TIM1,10);  

}
void Tleft()  //  原地左转  左边的两个轮子往后转，右边的两个轮子往前转
{
  TIM_SetCompare1(TIM1,0); //左前后退
 TIM_SetCompare2(TIM1,10); 
 TIM_SetCompare3(TIM1,0); //左后后退
 TIM_SetCompare4(TIM1,10); 

}
void Trighet()
{
  TIM_SetCompare1(TIM1,10); //左前前进
 TIM_SetCompare2(TIM1,0); 
 TIM_SetCompare3(TIM1,10); //左后前进
 TIM_SetCompare4(TIM1,0); 

}
