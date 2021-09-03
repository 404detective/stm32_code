#ifndef __ENCODER_H
#define __ENCODER_H	 
#include "sys.h"
#include "delay.h"

#define ENCODER_PORT_A	GPIOA		//定义IO接口组
#define ENCODER_L	GPIO_Pin_6	//定义IO接口
#define ENCODER_D	GPIO_Pin_7	//定义IO接口

#define ENCODER_PORT_B	GPIOB		//定义IO接口组
#define ENCODER_R	GPIO_Pin_2	//定义IO接口


  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

void Encoder_Init_TIM2(u16 arr,u16 psc);
void Encoder_Init_TIM4(u16 arr,u16 psc);
int Read_Encoder_TIM2(void);
int Read_Encoder_TIM4(void);

  /**************************************************************************
下面为以前的
**************************************************************************/

void ENCODER_Init(void);//初始化
u8 ENCODER_READ(void);



		 				    
#endif

