/*-------------------------------------------------------------------------------
文件名称：usart.c
文件描述：adc库函数
硬件平台：尼莫M3S开发板
           ----------------------------------------------------
          |                                                    |
          | PC1 - ADC1 连接外部电压(通过一个滑动变阻器分压)|
          |                                                    |
           ----------------------------------------------------
编写整理：shifang
固件库  ：V3.5
技术论坛：www.doflye.net
备    注：通过简单修改可以移植到其他开发板，部分资料来源于网络。
---------------------------------------------------------------------------------*/

#include "adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue;
volatile uint16_t ADC[10][3];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数


/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC.01
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PC.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PC1,输入时不用设置速率
	
	
	//test
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	//test
}


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
	//test
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC;//该参数用以定义DMA内存基地址(转换结果保存的地址)
	

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_BufferSize = 3*10;//定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是ADCConvertedValue的大小
	//test
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	
	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 3;	 	//要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_239Cycles5);//通道一转换结果保存到ADCConvertedValue[0~10][0]
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_239Cycles5);//通道二转换结果保存到ADCConvertedValue[0~10][1]
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_239Cycles5);//通道三转换结果保存到ADCConvertedValue[0~10][2]

	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}



