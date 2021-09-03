
#include <stdio.h>

#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"
#include "adc.h"
#include "key.h"
#include "beep.h"
#include "lcd.h"
#include "i2c.h"
#include "24c02.h"
#include "pwm.h"
#include "encoder.h"
#include "capture.h"
#include "HCSR04.h"
#include "Infrared.h"


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
//extern __IO uint16_t ADC_ConvertedValue;
 
     

//Ҫд�뵽24c02���ַ�������
const u8 TEXT_Buffer[]={"404detective TEST"};
#define SIZE sizeof(TEXT_Buffer)	 


// int main(void)
// {

// 	delay_Init();//��ʱ��ʼ��
// 	LED_Init();  //LED �˿ڳ�ʼ��
// 	The_BEEP_Init();
// 	while(1){
// 	BEEP_REV;
// 	Delay_ms(2000);
// }
// 	
// 	

// }
float ADC_Value[3];//�������澭��ת���õ��ĵ�ѹֵ

// �ֲ����������ڱ���ת�������ĵ�ѹֵ			 
float ADC_ConvertedValueLocal;




uint32_t IR_Key=0;
uint8_t flag=0;
uint8_t  dat=0;



// int main()  
// {  
//     extern u8 TIM_CAP_STA;  
//     extern u16 TIM_CAP_VAL;  
//     u32 temp = 0;  
//       
//     delay_Init(); // ��ʼ��systickʱ��  
//     KEY_Init(); // ��ʼ��KEY_WAKEUP  
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����  
//     TIM5_Cap_Init(0xFFFF, 72000-1); // ����Ϊ65.535s  
//     usart_Init(); // ��ʼ������  
//       
//     while(1)  
//     {  
//         Delay_ms(10);  
//         if((TIM_CAP_STA&0x80) == 0x80)  // �����Ч��ƽ�Ĳ���
//         {  
//             temp = TIM_CAP_VAL + (TIM_CAP_STA&0x3F) * 0xFFFF; // �����������ʱ��  
//             printf("%d\r\n", temp); // ���ڴ�ӡ  
//             TIM_CAP_STA = 0; // ���TIM_CAP_STA�ڵ������Ա��ٴβ���
//         }  
//     }     
// }


#include "stm32f10x.h"
#include "sys.h"
int pwm=0;




/****************************************
PA15:AD0   PB10:IIC_SCL   PB11:IIC_SDA
PA1:IIC_SCL_OLED      PA2:IIC_SDA_OLED
****************************************/
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

#include "stdio.h"
#include "motor.h"
int pwm1=0;
int pwm2=0;
int Encoder_Left=0;
int Encoder_Right=0;
int Kp_pitch=150;
int Kp_encoder=1;
void Change_Kp(int*Kp_pitch){
	 	if(!S1&&*Kp_pitch<200)
		{
			Delay_ms(10);
			if(!S1)	
			{
				 while(!S1);//�ȴ������ͷ�
				 *Kp_pitch+=10;
				LED2_REV;
			}
		}
		
		if(!S2)
		{
			Delay_ms(10);
			if(!S2)	
			{
				 while(!S2);//�ȴ������ͷ�
				 *Kp_pitch=150;
				LED2_REV;

			}
		}
		
		if(!S3&&*Kp_pitch>100)
		{
			Delay_ms(10);
			if(!S3)	
			{
				 while(!S3);//�ȴ������ͷ�
				 *Kp_pitch-=10;
				LED2_REV;

			}
		}
}

 int main(void)
 {	 
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;								//�¶�	

	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	delay_Init();				       //��ʱ��ʼ��
	KEY_Init();
	OLED_Init();
	OLED_ColorTurn(0);         //0������ʾ��1 ��ɫ��ʾ
  OLED_DisplayTurn(0);       //0������ʾ 1 ��Ļ��ת��ʾ
	MPU_Init();					       //��ʼ��MPU6050
	//usart_Init();
	TIM3_PWM_Init(7199,0);     //���PA6 7
	TIM1_PWM_Init(7199,0);     //���PA8  11
	 
	Encoder_Init_TIM4(0xffff,0);
	Encoder_Init_TIM2(0xffff,0);

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();
	
	while(mpu_dmp_init())
 	{
		OLED_Clear();
		OLED_Refresh();
	}
	OLED_Clear();
	OLED_Refresh();
	OLED_ShowString(0,0,"PITCH:",16,1);
	OLED_ShowString(0,15,"Left:",16,1);
	OLED_ShowString(0,31,"Right:",16,1);
	OLED_ShowString(0,47,"PWM:",16,1);
	OLED_Refresh();
 	while(1)
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			temp=MPU_Get_Temperature();							  //�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			
			/*
			printf("Pitch:  %f\r\n",(float)pitch);
			printf("Roll:  %f\r\n",(float)roll);
			printf("yaw:  %f\r\n",(float)yaw);
			printf("temp:  %f\r\n",(float)temp);
			printf(" \r\n");
			*/
			
			Encoder_Left=Read_Encoder_TIM4();
			Encoder_Right=Read_Encoder_TIM2();
			pwm1=Kp_pitch*pitch-Kp_encoder*Encoder_Right;
			pwm2=Kp_pitch*pitch-Kp_encoder*Encoder_Right;
			OLED_ShowFloat(50, 0,pitch,1,16,1);
			//OLED_ShowFloat(50,15,roll,5,16,1);
			
			OLED_ShowFloat(50,15,(float)Encoder_Left/10,5,16,1);
			OLED_ShowFloat(50,31,(float)Encoder_Right/10,5,16,1);
			OLED_ShowFloat(50,47,(float)pwm1/10,5,16,1);
			//OLED_ShowFloat(50,31,yaw,5,16,1);
			//OLED_ShowFloat(50,47,(float)temp/100,5,16,1);
			OLED_Refresh();
			Change_Kp(&Kp_pitch);
		}
		//Delay_us(2000); //ÿ��2ms��ȡһ�α��������������ٶȡ�
		
		MotorCtrl(pwm1,pwm2);
//     printf("LLLLLLLEncoder=%d\r\n", Read_Encoder_TIM4());
// 		printf("RRRRRRREncoder=%d\r\n", Read_Encoder_TIM2());


		
//		OLED_ShowFloat(50,47,(float)pwm2/10,5,16,1);
//		OLED_ShowFloat(50,15,(float)pwm2/10,5,16,1);


//  		printf("PWM1:  %d\r\n",pwm1);
// 		printf("PWM2:  %d\r\n",pwm2);
		
	} 	
}

int fputc(int ch, FILE *f)

{
USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��

while (!(USART1->SR & USART_FLAG_TXE));

return (ch);

}

// int main(void)
// {
// 	BEEP_Init();
// 	LED_Init(); //��ʼ��LED Ĭ��Ϊ�� setbitsΪ��
// 	delay_Init();//��ʼ����ʱʱ��
// 	usart_Init();
// //BEEP_REV;
// 	LED3_ON;
// 	
//   while (1)
//   {

// 	//ʹ��printf����ѭ�����͹̶���Ϣ
//   printf("\n\rUSART Printf Example: 404detective���ڲ��Գ���\r");
// 	Delay_ms(1000);
// 	LED2_REV;
// 	}
// }


// int main(void)
// {
// 	delay_Init();	
// 	The_Key_Init();
// 	LED_Init();	
//   while(1)
// 	{		 
// 			if(The_Key)
// 		{
// 			Delay_ms(10);
// 			if(The_Key)	
// 			{
// 				 while(The_Key);//�ȴ������ͷ�
// 				 pwm+=100;
// 				LED2_REV;
// 			}
// 		}
// 	}
// }








// int main(void)
// {
// 	delay_Init();
// 	usart_Init();
// 	Encoder_Init_TIM4(0xffff,0);
//   while(1)
// 	{		 
// 		Delay_ms(200); //ÿ��200ms��ȡһ�α��������������ٶȡ�
// 		               //����ʹ�ö�ʱ�ж�ʵ�ָ���׼���ٶȼ��㣬�û����Զ���
//     printf("Encoder=%d\r\n", Read_Encoder_TIM4());
// 	}
// }


/*************������**************************/
// int main(void)//PA8 9 10 11
// {
//   TIM1_PWM_Init(7199,0);
// 	LED_Init();
// 	KEY_Init();
// 	delay_Init();
//   while(1)
//    {
// 	if(!S1&&pwm<7000)
// 		{
// 			Delay_ms(10);
// 			if(!S1)	
// 			{
// 				 while(!S1);//�ȴ������ͷ�
// 				 pwm+=100;
// 				LED2_REV;
// 			}
// 		}
// 		
// 		if(!S2)
// 		{
// 			Delay_ms(10);
// 			if(!S2)	
// 			{
// 				 while(!S2);//�ȴ������ͷ�
// 				 pwm=0;
// 				LED2_REV;

// 			}
// 		}
// 		
// 		if(!S3&&pwm>-7000)
// 		{
// 			Delay_ms(10);
// 			if(!S3)	
// 			{
// 				 while(!S3);//�ȴ������ͷ�
// 				 pwm-=100;
// 				LED2_REV;

// 			}
// 		}
// 		
// 	 
// 	 if (pwm > 0)
//     {
//         TIM_SetCompare1(TIM1,pwm);
//         TIM_SetCompare4(TIM1,0);
//     }
//     else
//     {
//         TIM_SetCompare1(TIM1,0);
//         TIM_SetCompare4(TIM1,-pwm);
//     }
// 	 
// }
// }



/*************������**************************/
// int main(void)
// {
//   Infrared_Config();
// 	usart_Init();
// 	LED_Init();
//   while(1)
//    {
// 	 if(flag==1)
// 	 {
// 	   flag = 0;
// 	   dat = (uint8_t)(IR_Key>>8);
// 		 printf("%x\r\n",dat);
//       if(dat == 0x30)
// 				 LED2_ON;
// 			 if(dat == 0x18)
// 				  LED2_OFF; 
// 	 }
// 	 
//    }
// }

// /****************************************
//   0xa2      0x62      0xe2
//   0x22      0x02      0xc2
//   0xe0      0xa8      0x90
//   0x68      0x98      0xb0
//   0x30      0x18      0x7a
//   0x10      0x38      0x5a
//   0x42      0x4a      0x52


// *****************************************/



// int main(void)
// {	 
//     
//     float distance = 0;
// 	delay_Init();	    	 //��ʱ������ʼ��	  

// 	HCSR04_Init();
//     usart_Init();
//     while(1) 
// 	{		 	
//         
//    		 //printf("%f \r\n",distance);
// 		Delay_ms(1000);	
//     distance = Get_Distance();
//         printf("%f \r\n",distance);
//         
// 	} 
// }





// int main(void)
// {
// 	    extern u8 TIM_CAP_STA;  
//     extern u16 TIM_CAP_VAL; 
//  u32 temp = 0; 	
// 	LED_Init();
// 	delay_Init();
// 	KEY_Init();
// 	TIM5_Cap_Init(0xFFFF, 72-1); // ����Ϊ65.535s  
//   usart_Init(); // ��ʼ������  
// 	    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����  

//     while(1)  
//     {  
//         Delay_ms(10);  
//         if((TIM_CAP_STA&0x80) == 0x80)  // �����Ч��ƽ�Ĳ���
//         {  
//             temp = TIM_CAP_VAL + (TIM_CAP_STA&0x3F) * 0xFFFF; // �����������ʱ��  
//             printf("%d\r\n", temp); // ���ڴ�ӡ  
//             TIM_CAP_STA = 0; // ���TIM_CAP_STA�ڵ������Ա��ٴβ���
//         }  
//     }     


// }


// #include "sys.h"

// #include "encoder.h"


// int main (void)
// {
// 	u8 a=0,b=0,c=0x01;

//   delay_Init();
// 	usart_Init();
// 	ENCODER_Init(); //��ת��������ʼ��


// 	while(1)
// 	{
// 		b=ENCODER_READ();	//������ת������ֵ	
// 		if(b==1){a++;if(a>99)a=0;} //��������ֵ�����Ӽ�������ֵ��
// 		if(b==2){if(a==0)a=100;a--;}
// 		//if(b==3)a=0;
// 		
// 		if(b!=0)	 //�������ת���Ĳ���
// 		{
// 				printf("%d\r\n", a);
// 		}
// 	}
// }






///////////////////////////////////////////////////////////////////
//Encoder_Init_TIM2();


// int main(void)
// {
// 	
// 	LED_Init();
// 	delay_Init();
// 	KEY_Init();
// 	

//    	while(1)
// 	{ 
// 		
// 	//	TIM_SetCompare2(TIM3,pwm);

// }


// }

///////////////////////////////////////////////////////////////


// int main(void)
// {
// 	LED_Init();
// delay_Init();
// KEY_Init();
// //TIM2_PWM_Init(899,0); //����Ƶ,PWM Ƶ��=72000/900=8Khz
// 	
// TIM3_PWM_Init(1999,719);

//    	while(1)
// 	{ 
// 		
// 		if(!S1&&pwm<1990)
// 		{
// 			Delay_ms(10);
// 			if(!S1)	
// 				
// 			{
// 				 while(!S1);//�ȴ������ͷ�
// 				 pwm+=1;
// 				LED2_REV;
// 				TIM_SetCompare2(TIM3,pwm);
// 				
// 			}
// 		}
// 		
// 		if(!S2)
// 		{
// 			Delay_ms(10);
// 			if(!S2)	
// 				
// 			{
// 				 while(!S2);//�ȴ������ͷ�
// 				 pwm=1850;
// 				LED2_REV;
// 	TIM_SetCompare2(TIM3,pwm);
// 			}
// 		}
// 		
// 		if(!S3&&pwm>1740)
// 		{
// 			Delay_ms(10);
// 			if(!S3)	
// 				
// 			{
// 				 while(!S3);//�ȴ������ͷ�
// 				 pwm-=1;
// 				LED2_REV;
// 				TIM_SetCompare2(TIM3,pwm);
// 			}
// 		}
// 		
// 	//	TIM_SetCompare2(TIM3,pwm);

// }


// }








// int main(void)
// {
// 	int i=0;
// 	int sum=0;
//   uint8_t j; //�������
// 	LED_Init();//LED��ʼ��
// 	usart_Init();
// 	The_KEY_Init();
// 	ADC1_Init();
//   delay_Init();//��ʱ��ʼ��

//   while (1)
//   {
// 		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))
// 		{
// 			Delay_ms(1);
// 			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6))	
// 				
// 			{
// 				 while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6));//�ȴ������ͷ�
// 				 LED2_REV;
// 			}
// 			
// 		}
// 		for(i=0;i<3;i++)
// 		{
// 			sum=0;
// 			for(j=0;j<10;j++)
// 			{
// 				sum+=ADC[j][i];
// 			}
// 			ADC_Value[i]=(float)sum/(10*4096)*3.3;//��ƽ��ֵ��ת���ɵ�ѹֵ
// 			//��ӡ���ԣ�
// 		}

// 			//ADC_ConvertedValueLocal =(float) ADC[0][0]/4096*3.3; // ��ȡת����ADֵ
// 	
// 			printf("\r\n ת�����ʵ�ʵ�ѹֵ = %f V \r\n",ADC_Value[0]); 

// 	
// 			Delay_ms(1000);

// 		
// 		////////////////////////////////////////////

//   }
// }


// int main(void)
// {

// 	u16 led0pwmval=0;
// 	u16 led1pwmval=0;
// 	u16 led2pwmval=0;
// 	u8 dir=1;
// 	int i=5000;
//  
// TIM2_PWM_Init(899,0); //����Ƶ,PWM Ƶ��=72000/900=8Khz
// TIM3_PWM_Init(899,0);
//  while(1)
// {
// 	for(i=0;i<10000;i++);
// 	//while(i--); while�ܰ���С�Ʋ�����
// 	if(dir)
// 		led0pwmval++;
// 	else
// 		led0pwmval--;
// 	if(led0pwmval>899)
// 		dir=0;
// 	if(led0pwmval==0)
// 	dir=1;

// 	TIM_SetCompare2(TIM2,led1pwmval); 

// 	
// 	for(i=0;i<10000;i++);
// 	//while(i--); while�ܰ���С�Ʋ�����
// 	if(dir)
// 		led1pwmval++;
// 	else
// 		led1pwmval--;
// 	if(led1pwmval>899)
// 		dir=0;
// 	if(led1pwmval==0)
// 	dir=1;

// 	TIM_SetCompare2(TIM2,led1pwmval); 
// 	TIM_SetCompare1(TIM2,led1pwmval); 
// 	TIM_SetCompare2(TIM3,led0pwmval);
// 	
// 		for(i=0;i<10000;i++);
// 	//while(i--); while�ܰ���С�Ʋ�����
// 	if(dir)
// 		led0pwmval++;
// 	else
// 		led0pwmval--;
// 	if(led0pwmval>899)
// 		dir=0;
// 	if(led0pwmval==0)
// 	dir=1;

// 	TIM_SetCompare2(TIM2,led1pwmval); 
// 	TIM_SetCompare1(TIM2,led2pwmval); 
// 	TIM_SetCompare2(TIM3,led0pwmval);
// }
// }














// int main(void)
// {		
// 	u8 key;
// 	u16 i=0;
// 	u8 datatemp[SIZE];
// 	LED_Init();//LED��ʼ��
//   KEY_Init();//������ʼ��
//   delay_Init();//��ʱ��ʼ��
// 	BEEP_Init();   //��������ʼ��

// 	LCD_Init();
// 	
// 	AT24CXX_Init();			//IIC��ʼ�� 

//  	POINT_COLOR=RED;//��������Ϊ��ɫ 
// 	LCD_ShowString(60,50,200,16,16,"M3S STM32");	
// 	LCD_ShowString(60,70,200,16,16,"IIC test");	
// 	//LCD_ShowString(60,90,200,16,16,"www.doflye.net");
// 	//LCD_ShowString(60,110,200,16,16,"for IC 24c02 ");	
// 	LCD_ShowString(60,130,200,16,16,"S4:Write  S2:Read");	//��ʾ��ʾ��Ϣ		
//  	while(AT24CXX_Check())//��ⲻ��24c02
// 	{
// 		LCD_ShowString(60,150,200,16,16,"24C02 Check Failed!");
// 		Delay_ms(500);
// 		LCD_ShowString(60,150,200,16,16,"Please Check!      ");
// 		Delay_ms(500);
// 		LED2_REV;
// 	}
// 	LCD_ShowString(60,150,200,16,16,"24C02 Ready!");    
//  	POINT_COLOR=BLUE;//��������Ϊ��ɫ	  
// 	while(1)
// 	{
// 		key=KEY_Scan(0);
// 		if(key==KEY4)//S4����,д��24C02
// 		{
// 			LCD_Fill(0,170,239,319,WHITE);//�������    
//  			LCD_ShowString(60,170,200,16,16,"Start Write 24C02....");
// 			AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
// 			LCD_ShowString(60,170,200,16,16,"24C02 Write Finished!");//��ʾ�������
// 		}
// 		if(key==KEY2)//S2����,��ȡ�ַ�������ʾ
// 		{
//  			LCD_ShowString(60,170,200,16,16,"Start Read 24C02.... ");
// 			AT24CXX_Read(0,datatemp,SIZE);
// 			LCD_ShowString(60,170,200,16,16,"The Data Readed Is:  ");//��ʾ�������
// 			LCD_ShowString(60,190,200,16,16,datatemp);//��ʾ�������ַ���
// 		}
// 		i++;
// 		Delay_ms(10);
// 		if(i==20)
// 		{
// 			LED2_REV;
// 			i=0;
// 		}		   
// 	}
// }























// int main(void)
// {
// 	
// 	u8 x=0;
// 	u8 lcd_id[12];			//���LCD ID�ַ���
// 	u8 str[12];			//���LCD ID�ַ���
// 	LED_Init();//LED��ʼ��
//   delay_Init();//��ʱ��ʼ��
// 	LCD_Init();
// 	ADC1_Init();
// 	POINT_COLOR=RED;
// 	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣				 	
//   while(1) 
// 	{		 
// 		ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ

// 		sprintf((char*)str,"V =%04f",ADC_ConvertedValueLocal);//��LCD ID��ӡ��lcd_id���顣		
// 		switch(x)
// 		{
// 			case 0:LCD_Clear(WHITE);break;
// 			case 1:LCD_Clear(BLACK);break;
// 			case 2:LCD_Clear(BLUE);break;
// 			case 3:LCD_Clear(RED);break;
// 			case 4:LCD_Clear(MAGENTA);break;
// 			case 5:LCD_Clear(GREEN);break;
// 			case 6:LCD_Clear(CYAN);break;

// 			case 7:LCD_Clear(YELLOW);break;
// 			case 8:LCD_Clear(BRRED);break;
// 			case 9:LCD_Clear(GRAY);break;
// 			case 10:LCD_Clear(LGRAY);break;
// 			case 11:LCD_Clear(BROWN);break;
// 		}
// 		POINT_COLOR=RED;	  
// 		LCD_ShowString(30,50,200,16,16,str);	
// 		LCD_ShowString(30,70,200,16,16,"TFT-LCD test");	
// 		LCD_ShowString(30,90,200,16,16,"404detecitve");
//  		LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
// 		LCD_ShowString(30,130,200,16,16,"01234567890��");	   
//     LCD_ShowString(30,150,200,16,16,"abcdefgABCDEFG");	
// 		Draw_Circle(100,180,20);	
//     Draw_Circle(100,180,25);	     					 
// 	    x++;
// 		if(x==12)x=0;
// 		LED2_REV;					 
// 		Delay_ms(1000);	
// 	} 
// }












// int main(void)
// {
// 	LED_Init();//LED��ʼ��

//   delay_Init();//��ʱ��ʼ��

//   usart_Init();
// 	ADC1_Init();
// 	

//   while (1)
//   {
// 	//ʹ��printf����ѭ�����͹̶���Ϣ
//   
// 	ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; // ��ȡת����ADֵ
// 	
// // 	USART_SendData(USART1,ADC_ConvertedValue);
// // 	USART_SendData(USART1,ADC_ConvertedValueLocal);
// 	printf("\r\n �ɼ�����16���� = 0x%04X \r\n", ADC_ConvertedValue); 
// 	printf("\r\n ת�����ʵ�ʵ�ѹֵ = %f V \r\n",ADC_ConvertedValueLocal); 
// 	Delay_ms(1000);		  
//   LED2_REV;	
//   }
// }

// PUTCHAR_PROTOTYPE
// {
//   /* Place your implementation of fputc here */
//   /* e.g. write a character to the USART */
//   USART_SendData(USART1, (uint8_t) ch);

//   /* ѭ���ȴ�ֱ�����ͽ���*/
//   while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//   {}

//   return ch;
// }

// int main(void)
// {
// BEEP_Init();
// LED_Init(); //��ʼ��LED Ĭ��Ϊ�� setbitsΪ��
// delay_Init();//��ʼ����ʱʱ��
// //BEEP_REV;
// while(1)
//  {
//   Delay_ms(1000);//��ʱ1s
// 	BEEP_REV;
// 	LED2_REV;//LED2ȡ��
// 	LED3_REV;//LED3ȡ��

// 	
//  } 
//  
// }

// int main(void)
// {	
// 	int n=0;
// 	char res[]="abc";
// 	usart_Init();
// 	//Delay_ms(2000);
// 	usart2_Init();
// 	LED_Init();
// 	LED2_ON;
// 	LED3_OFF;

//   while (1){
// // 		for(n=0;n<50000;n++);
// // 		USART_SendData(USART1,*res);
// 		//Delay_ms(1000);

// // 		printf("\r\n  123");
// }
// 		
	
//}
