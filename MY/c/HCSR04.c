#include "HCSR04.h"
#include "delay.h"

u8 msHcCount = 0;//ms计数
void HCSR04_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;       //发送电平引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;     //返回电平引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOB,GPIO_Pin_7);    

    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period = (1000-1); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值    计数到1000为1ms
    TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位  
    
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);       //清除更新中断，免得一打开中断立即产生中断
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);    //打开定时器更新中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //选择定时器3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //抢占式中断优先级设置为0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //响应式中断优先级设置为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //使能中断
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM4,DISABLE);         
}

//与编码器冲突 暂时注释掉
// void TIM4_IRQHandler(void)   //TIM3中断
// {
//     if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//     {
//             TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM3更新中断标志 
//             msHcCount++;
//     }
// }

static void Open_TIM4()        //打开定时器
{
        TIM_SetCounter(TIM4,0);//清除计数
        msHcCount = 0;
        TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
}
 
static void Close_TIM4()        //关闭定时器
{
        TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
}

u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;//得到MS
        t += TIM_GetCounter(TIM4);//得到US
          TIM4->CNT = 0;  //将TIM2计数寄存器的计数值清零
                Delay_ms(50);
        return t;
}
 
float Get_Distance(void)
{
    int i = 0;
    int t=0;
    float distance = 0;
    float sum = 0;
    while(i!=5)    //做五次取平均，滤波作用
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_6);         //发送口高电平输出
        Delay_us(20);
        GPIO_ResetBits(GPIOB,GPIO_Pin_6); 
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 0);   //等待接收口高电平输出入
        Open_TIM4();        //打开定时器
        i++;
        while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == 1);
        Close_TIM4();        //关闭定时器
        t = GetEchoTimer();     //获取时间,分辨率为1US
        distance = (float)t * 170 / 10000;//cm


        sum = distance + sum ;        
    }
    distance = sum/5.0;
    return distance;
}
