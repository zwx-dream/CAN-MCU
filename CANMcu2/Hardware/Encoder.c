#include "stm32f10x.h"                  // Device header
void Encoder_Init()
{
    // 1. 开启时钟：定时器3 + GPIOA端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  // TIM3挂载在APB1总线上，开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); // GPIOA挂载在APB2总线上，开时钟
	
    // 2. 配置GPIO口：PA6、PA7 作为编码器输入引脚
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;  // 上拉输入模式（抗干扰，编码器常用）
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;  // 编码器A、B相接在PA6、PA7
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
    // 3. 配置定时器基本参数（只是填格式，编码器模式用不到这些）
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;  // 向上计数
	TIM_TimeBaseStruct.TIM_ClockDivision=TIM_CKD_DIV1;      // 不分频
	TIM_TimeBaseStruct.TIM_Period=65535;  // 自动重装载值（最大值）
	TIM_TimeBaseStruct.TIM_Prescaler=0;   // 预分频器（不分频）
	TIM_TimeBaseStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStruct);
	
    // 4. 配置输入捕获参数（滤波+边沿）
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);  // 赋默认值
	
	// 配置通道1（PA6）
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter=0XF;    // 输入滤波（防抖，数值越大滤波越强）
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;  // 上升沿触发
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	// 配置通道2（PA7）
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter=0XF;
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
    // 5. 【核心】配置定时器3为正交编码器模式
    // TIM_EncoderMode_TI12：A、B两相都计数（精度最高）
    // 最后两个参数：A、B相都检测上升沿+下降沿
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
    // 6. 启动定时器3
	TIM_Cmd(TIM3,ENABLE);
}

int16_t Get_Encoder()
{
	int16_t nnumer;       // 定义16位有符号变量（能存正负）
	nnumer = TIM_GetCounter(TIM3);  // 读取TIM3的计数值
	TIM_SetCounter(TIM3,0);         // 读完立刻清零，方便下次读取
	return nnumer;                  // 返回脉冲数（正负代表方向）
}