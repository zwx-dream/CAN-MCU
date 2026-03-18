#include "stm32f10x.h"                  // Device header
#include "PWM.h"
void PWM_Init()
{
    // 1. ???
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // ???2??
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // PA???

    // 2. ??PA0????????PWM?????
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // ??????
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;        // PWM?????PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. ???????????
    TIM_InternalClockConfig(TIM2);

    // 4. ????????????PWM???
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;      // ???????99
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;   // ????719
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // 5. ??PWM????????
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;    // PWM??1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // ?????
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // ????
    TIM_OCInitStructure.TIM_Pulse = 0; // ??????0??????
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);

    // 6. ?????
    TIM_Cmd(TIM2, ENABLE);
}
// ????1???????????
void PWM_SetComper1(uint16_t Comper)
{
    TIM_SetCompare1(TIM2, Comper);
}