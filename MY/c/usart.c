
#include "usart.h"
#include "led.h"
#include "stm32f10x.h"
void usart_Init(){
		
		GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	  USART_DeInit(USART1);  //复位串口1
	  //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;//50
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10     
	
	  //Usart1 NVIC 配置

    	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

		USART_InitStructure.USART_BaudRate = 9600; //波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
		USART_InitStructure.USART_Parity = USART_Parity_No,//校验方式
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制流
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送和接收

		USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
		USART_Cmd(USART1, ENABLE);                    //使能串口 


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}

void USART1_IRQHandler(void){
	u8 res;
	//int i =0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
		res=USART_ReceiveData(USART1);
		
	{
		
		USART_SendData(USART1,res);
		
		//LED2_REV;//LED2取反
		LED3_REV;//LED3取反

}

}

