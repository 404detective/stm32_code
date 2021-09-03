#include "stm32f10x_it.h"
#include "Infrared.h"

extern uint32_t IR_Key;
extern uint8_t flag;
  uint8_t IR_Sta = 0;
  uint8_t IR_Up = 0;
  uint32_t IR_Code = 0;
  uint8_t IR_PulseCnt = 0;
  uint16_t IR_ThisPulse = 0;
  uint16_t IR_LastPulse = 0;
  uint16_t IR_PulseSub = 0;
  uint16_t LianfaCnt;


//TIM3中断入口函数
// void TIM4_IRQHandler(void)
// { 
//   if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) != RESET ) 
//     {    
//       TIM_ClearITPendingBit(TIM4 , TIM_IT_CC4);
// 	  IR_Up = 0;                                //每次进入捕获中断就清除溢出次数计数值
// 	  IR_ThisPulse = TIM_GetCapture4(TIM4);	    //获取通道2的捕获值
// 	  if(IR_ThisPulse>IR_LastPulse)	            //这次捕获的值大于上次捕获的值
// 	   { 
// 		 IR_PulseSub = IR_ThisPulse-IR_LastPulse;  //得到时间差
// 	   }
// 	  else										//小于时要加一个0xffff
// 	   { 
// 		 IR_PulseSub = 0xffff-IR_LastPulse+IR_ThisPulse;  //得到时间差
// 	   }
// 	  IR_LastPulse = IR_ThisPulse;  //将本次得到值作为下一位编码解码的前一个值
// 	  IR_PulseCnt++;   //解码位数加1
// 	  if(IR_PulseCnt == 2)
// 	   {  
//           if((IR_PulseSub>6000)&&(IR_PulseSub<8000))  	 //引导码范围，13.5ms	6750
// 		   {
// 	         IR_Sta = 0x01;				              //标志位1，表示引导码已收到
// 		   }
// 		  else
// 		   IR_PulseCnt = 0;							   //如果时间差没在引导码范围内，中断次数清零，重新解码
// 	   }
// 	  if((IR_PulseCnt > 2)&&(IR_Sta == 0x01))		  //如果中断次数大于2，并且引导码已经解完
// 	   { 
// 	     IR_Code <<= 1;								   //存储红外编码的寄存器向左移一位，以便于存储下一个位放到最低位
// 		 if((IR_PulseSub>450)&&(IR_PulseSub<700))	   //编码‘0’范围，1.125ms 562.5
// 		  {
// 		    
// 		    IR_Code|=0x00;							   //存储0
// 		  }
// 		 else if((IR_PulseSub>800)&&(IR_PulseSub<1300))	   //编码‘1’范围，2.25ms 1125
// 		        {
// 		          IR_Code|=0x01;						  //存储1
// 				  
// 				}
// 			  else										 //如果不是0码也不是1码，变量清零，重新解码
// 			   { 
// 				 IR_Sta = 0;
// 	       IR_Code = 0;
// 				 IR_PulseCnt = 0;
// 			   }
// 	   }

// 	   if(IR_PulseCnt == 34)						 //如果解完码，第34次进入中断正好解完一帧码
// 	    { 
// 	      IR_Key = IR_Code;							 //存放解出的码
// 		  IR_Sta = 0x02;								 //进入连发码状态
// 		  flag = 1;									 //解完码标志位置1，进行编码处理
// 	    }
// 	   if((IR_PulseCnt == 36)&&(IR_Sta == 0x02))
// 	     {
// 		    IR_PulseCnt = 34;
// 		    if((IR_PulseSub>4500)&&(IR_PulseSub<6000))
// 			  {
// 				 LianfaCnt++;
// 				 IR_Key = IR_Code;
// 				 flag = 1;
// 			  }
// 		 }	      
// 	} 

//   if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
//    {
// 	 TIM_ClearITPendingBit(TIM4 , TIM_IT_Update);
// 	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 1)
// 	  {
// 	    IR_Up++;
// 		if(IR_Up>=2)
// 		 {
// 		   IR_Code = 0;
// 		   IR_Sta  = 0;
// 		   IR_PulseCnt = 0;
// 		   LianfaCnt = 0;
// 		 }
// 	  }
//    }
// }


void Infrared_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
   TIM_ICInitTypeDef  TIM_ICInitStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;
	
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);	//GPIOF时钟打开
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//TIM3时钟打开
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //推挽输出
   GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	 TIM_TimeBaseInitStructure.TIM_Period        = 0xffff;   // 16位计数
   TIM_TimeBaseInitStructure.TIM_Prescaler     = 72*2-1;   // 144分频 2us
   TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;        // 不分割
   TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up; // 上升计数

   TIM_TimeBaseInit( TIM4, &TIM_TimeBaseInitStructure );
	 TIM_ICInitStructure.TIM_Channel     = TIM_Channel_4;   // 选择通道4
   TIM_ICInitStructure.TIM_ICPolarity  = TIM_ICPolarity_Falling;  // 下降沿触发
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
   TIM_ICInitStructure.TIM_ICFilter    = 0x0; 
   TIM_ICInit( TIM4, &TIM_ICInitStructure );
 
   TIM_Cmd( TIM4, ENABLE );
   TIM_ITConfig( TIM4,TIM_IT_CC4|TIM_IT_Update, ENABLE );
	 TIM_ClearFlag(TIM4, TIM_IT_CC4|TIM_IT_Update);
	 
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  			   
   NVIC_InitStructure.NVIC_IRQChannel        = TIM4_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority       = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

