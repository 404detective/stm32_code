#include "motor.h"
#include "stm32f10x_gpio.h"
#include "usart.h"

void MotorCtrl(int motor1,int motor2){
		if(motor1>7000)
			motor1=5000;
		if(motor2>7000)
			motor2=5000;
		if(motor1<-7000)
			motor1=-5000;
		if(motor2<-7000)
			motor2=-5000;
		if (motor1 > 0)
    {		
        TIM_SetCompare1(TIM3,motor1+1000);
        TIM_SetCompare2(TIM3,0);
    }
    else
    {
        TIM_SetCompare1(TIM3,0);
        TIM_SetCompare2(TIM3,-motor1+1000);
    }
		if (motor2 > 0)
    {
        TIM_SetCompare1(TIM1,motor2+1000);
        TIM_SetCompare4(TIM1,0);
    }
    else
    {
        TIM_SetCompare1(TIM1,0);
        TIM_SetCompare4(TIM1,-motor2+1000);
    }
		
	
}