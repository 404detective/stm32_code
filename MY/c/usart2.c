
#include "usart2.h"
#include "led.h"

void usart2_Init(){
		
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
    	NVIC_InitTypeDef NVIC_InitStructure;
	 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能PORTA时钟
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //使能USART2
 	  USART_DeInit(USART2);  //复位串口2
	  //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3       
	
	  //Usart2 NVIC 配置

    	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

		USART_InitStructure.USART_BaudRate = 9600; //波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据长度
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
		USART_InitStructure.USART_Parity = USART_Parity_No,//校验方式
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制流
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送和接收

		USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
		USART_Cmd(USART2, ENABLE);                    //使能串口 


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

}

void USART2_IRQHandler(void){
	u8 res;
	//int i =0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
		res=USART_ReceiveData(USART2);
		
	{
		
		USART_SendData(USART1,res);
		
		LED2_REV;//LED2取反
		LED3_REV;//LED3取反

}
}



