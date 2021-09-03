/*-------------------------------------------------------------------------------
文件名称：key.c
文件描述：配置按键初始化参数       
备    注：无
---------------------------------------------------------------------------------*/
#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	//打开PB口时钟
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//打开PA口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//打开PE口时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//PE2,PE3,PE4引脚设置	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4| GPIO_Pin_6;
	//端口速度
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//端口模式，此为输入上拉模式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	//初始化对应的端口
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	//PA0引脚设置	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_6;
	//端口速度
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//端口模式，此为输入下拉模式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//初始化对应的端口
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

void The_Key_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	//打开PA口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);


// 	//端口模式，此为输入上拉模式
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//端口输入低电平0要上拉  输入高电平1要下拉
	//PA0引脚设置	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//端口速度
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	//端口模式，此为输入shang拉模式
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	//初始化对应的端口
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(S1==0||S2==0||S3==0||S4==1||The_Key==1))
	{
		Delay_ms(10);//去抖动 
		key_up=0;
		if(S1==0)return 1;
		else if(S2==0)return 2;
		else if(S3==0)return 3;
		else if(S4==1)return 4;
		else if(The_Key==1)return 5;
	}else if(S1==1&&S2==1&&S3==1&&S4==0&&The_Key==1)key_up=1; 	    
 	return 0;// 无按键按下
}
