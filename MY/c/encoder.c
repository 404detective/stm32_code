#include "encoder.h"
#include "stm32f10x_gpio.h"

/**************************************************************************
函数功能：把TIM2初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
  TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;	//PA0、PA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM2, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM2, ENABLE); //使能定时器4
}

//读取编码器计数
int Read_Encoder_TIM2(void)
{
	int Encoder_TIM;
	Encoder_TIM=TIM2->CNT; //读取计数
	if(Encoder_TIM>0xefff)Encoder_TIM=Encoder_TIM-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM4->CNT范围为0-0xffff，初值为0。
	TIM2->CNT=0; //读取完后计数清零
	return Encoder_TIM; //返回值
}
/**************************************************************************
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;//定义一个定时器初始化的结构体
  TIM_ICInitTypeDef TIM_ICInitStructure; //定义一个定时器编码器模式初始化的结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //使能TIM4时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能CPIOB时钟
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//PB6、PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);	//根据GPIO_InitStructure的参数初始化GPIOB0

	TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; // 预分频器 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3：CH1、CH2同时计数，四分频
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM_ICInitStructure); //根TIM_ICInitStructure参数初始化定时器TIM4编码器模式

	TIM_Cmd(TIM4, ENABLE); //使能定时器4
}

//读取编码器计数
int Read_Encoder_TIM4(void)
{
	int Encoder_TIM;
	Encoder_TIM=TIM4->CNT; //读取计数
	if(Encoder_TIM>0xefff)Encoder_TIM=Encoder_TIM-0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
	                                                      //TIM4->CNT范围为0-0xffff，初值为0。
	TIM4->CNT=0; //读取完后计数清零
	return Encoder_TIM; //返回值
}









//以下为旋转编码器


u8 KUP;		//旋钮锁死标志（1为锁死）
u16 cou;	//锁死时间计数


//接口初始化
void ENCODER_Init(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = ENCODER_L | ENCODER_D; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(ENCODER_PORT_A,&GPIO_InitStructure);	

    GPIO_InitStructure.GPIO_Pin = ENCODER_R; //选择端口号                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //选择IO接口工作方式 //上拉电阻       
	GPIO_Init(ENCODER_PORT_B,&GPIO_InitStructure);				
}


//读出旋转编码器的旋转数据
u8 ENCODER_READ(void)
{
	u8 a;//存放按键的值
	u8 kt;
	a=0;
	
	//判断旋钮是否解除锁死
	if(GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L))KUP=0;	
	//判断是否旋转旋钮，同时判断是否有旋钮锁死
	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&(KUP==0))
	{ 
		Delay_us(100);
		kt=GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D);	//把旋钮另一端电平状态记录
		Delay_ms(3); //延时
// 		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L))	//去抖
// 		{ 
			if(kt==0)	//用另一端判断左或右旋转
			{ 
				a=1;//右转
			}
			else
			{
				a=2;//左转
			}
			cou=0; //初始锁死判断计数器
			//等待放开旋钮，同时累加判断锁死，如果在1.2S内回到高电平则退出while循环
			while(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_L)&&(cou<60000)) 
			{ 
				cou++;
				KUP=1;
				Delay_us(20); //共延时1.2S来判断是否锁死
			}
	//	}
	}
// 	if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D)&&(KUP==0))	//判断旋钮是否按下  
// 	{ 
// 		Delay_ms(20);
// 		if(!GPIO_ReadInputDataBit(ENCODER_PORT_A,ENCODER_D))	//去抖动
// 		{ 
// 			a=3;//在按键按下时加上按键的状态值
// 			//while(ENCODER_D==0);	等等旋钮放开
// 		}
// 	}
	return a;
} 
