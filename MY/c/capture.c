#include "capture.h"  
#include "stm32f10x.h"    
#include "led.h"  
  
void TIM5_Cap_Init(u16 ARR, u16 PSC)  
{  
    GPIO_InitTypeDef GPIO_InitStructure;  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;  
    TIM_ICInitTypeDef TIM_ICInitStructure;  
    NVIC_InitTypeDef NVIC_InitStructure;  
      
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // 使能APB1总线的TIM5时钟  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能APB2总线的GPIOA时钟  
      
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure); // PA0初始化为下拉输入  
      
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // AHB = APB1  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式  
    TIM_TimeBaseInitStructure.TIM_Period = ARR;  
    TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;  
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure); // 配置计数器的属性  
      
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  
    TIM_ICInitStructure.TIM_ICFilter = 0;  
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  
    TIM_ICInit(TIM5, &TIM_ICInitStructure); // 配置TIM5的输入属性  
      
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_Init(&NVIC_InitStructure); // 配置TIM5的中断优先级  
      
    TIM_ITConfig(TIM5, TIM_IT_CC1|TIM_IT_Update, ENABLE); // 使能TIM5_CH1的中断  
      
    TIM_Cmd(TIM5, ENABLE); // 使能TIM5  
}  
  
u8 TIM_CAP_STA = 0;  
u16 TIM_CAP_VAL = 0;  
  
void TIM5_IRQHandler()  
{  
    if((TIM_CAP_STA&0x80) == 0)  
    {  
        if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)  
        {  
            if((TIM_CAP_STA&0x40) == 0x00) // 上升沿  
            {  
                TIM_CAP_STA = 0; // 初始化  
                TIM_CAP_VAL = 0; // 初始化  
                TIM_SetCounter(TIM5, 0); // 计数器开始计数  
                TIM_CAP_STA |= 0x40;  // 高电平  
                TIM_OC1PolarityConfig(TIM5, TIM_OCPolarity_Low); // 改变极性/改变有效电平——捕获上升沿  
            }  
            else // 下降沿  
            {  
                TIM_CAP_VAL = TIM_GetCapture1(TIM5); // 捕获此时计数器的值  
                TIM_CAP_STA |= 0x80; // 完成捕获  
                TIM_OC1PolarityConfig(TIM5, TIM_OCPolarity_High); // 改变极性/改变有效电平——捕获下降沿  
            }  
        }  
        if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) //  计数器溢出  
        {  
            if((TIM_CAP_STA&0x40) == 0x40) // 上升沿  
            {  
                if((TIM_CAP_STA&0x3F) == 0x3F) // 当溢出次数过多，就默认"捕获完成并且此时计数器的值为MAX"  
                {  
                    TIM_CAP_VAL = 0xFFFF;  
                    TIM_CAP_STA |= 0x80; // 完成捕获  
                }  
                else  
                {  
                    TIM_CAP_VAL++; // 溢出次数递增  
                }  
            }  
        }  
    }  
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); // 清楚中断标志位  
}  












































// #include "capture.h"  
// #include "stm32f10x.h"    
// #include "led.h"  
//   
// void TIM5_Cap_Init(u16 ARR, u16 PSC)  
// {  
//     GPIO_InitTypeDef GPIO_InitStructure;  
//     TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;  
//     TIM_ICInitTypeDef TIM_ICInitStructure;  
//     NVIC_InitTypeDef NVIC_InitStructure;  
//       
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // 使能APB1总线的TIM5时钟  
//     RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能APB2总线的GPIOA时钟  
//       
//     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
//     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  
//     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
//     GPIO_Init(GPIOA, &GPIO_InitStructure); // PA0初始化为下拉输入  
//       
//     TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; // AHB = APB1  
//     TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式  
//     TIM_TimeBaseInitStructure.TIM_Period = ARR;  
//     TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;  
//     TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure); // 配置计数器的属性  
//       
//     TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;  
//     TIM_ICInitStructure.TIM_ICFilter = 1111;  
//     TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  
//     TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  
//     TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  
//     TIM_ICInit(TIM5, &TIM_ICInitStructure); // 配置TIM5的输入属性  
//       
//     NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//     NVIC_Init(&NVIC_InitStructure); // 配置TIM5的中断优先级  
//       
//     TIM_ITConfig(TIM5, TIM_IT_CC1|TIM_IT_Update, ENABLE); // 使能TIM5_CH1的中断  
//       
//     TIM_Cmd(TIM5, ENABLE); // 使能TIM5  
// }  
//   
// u8 TIM_CAP_STA = 0;  
// u16 TIM_CAP_VAL = 0;  
//   
// void TIM5_IRQHandler()  
// {  
//     if((TIM_CAP_STA&0x80) == 0)  
//     {  
//         if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)  
//         {  
//             if((TIM_CAP_STA&0x40) == 0x00) // 上升沿  
//             {  
//                 TIM_CAP_STA = 0; // 初始化  
//                 TIM_CAP_VAL = 0; // 初始化  
//                 TIM_SetCounter(TIM5, 0); // 计数器开始计数  
//                 TIM_CAP_STA |= 0x40;  // 高电平  
//                 TIM_OC1PolarityConfig(TIM5, TIM_OCPolarity_Low); // 改变极性/改变有效电平——捕获上升沿  
//             }  
//             else // 下降沿  
//             {  
//                 TIM_CAP_VAL = TIM_GetCapture1(TIM5); // 捕获此时计数器的值  
//                 TIM_CAP_STA |= 0x80; // 完成捕获  
//                 TIM_OC1PolarityConfig(TIM5, TIM_OCPolarity_High); // 改变极性/改变有效电平——捕获下降沿  
//             }  
//         }  
//         if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET) //  计数器溢出  
//         {  
//             if((TIM_CAP_STA&0x40) == 0x40) // 上升沿  
//             {  
//                 if((TIM_CAP_STA&0x3F) == 0x3F) // 当溢出次数过多，就默认"捕获完成并且此时计数器的值为MAX"  
//                 {  
//                     TIM_CAP_VAL = 0xFFFF;  
//                     TIM_CAP_STA |= 0x80; // 完成捕获  
//                 }  
//                 else  
//                 {  
//                     TIM_CAP_VAL++; // 溢出次数递增  
//                 }  
//             }  
//         }  
//     }  
//     TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); // 清楚中断标志位  
// }  