#include "TIM.h"
#include "USART.h"

// ��ͨ��ң�س�ʼֵ
uint16_t PWMInCh1 = 0, PWMInCh2 = 0, PWMInCh3 = 0, PWMInCh4 = 0;

// PWM�����ʼ��
void PWM_Out_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// ʱ�ӿ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	// GPIO���ã����츴��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// GPIO����TIM1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11, GPIO_AF_TIM1);

	// �ر�TIM1
	TIM_DeInit(TIM1);
	// TIM1���ã�Ԥ��ƵΪ80������Ϊ2500�����ϼ���
	TIM_TimeBaseStructure.TIM_Period = 2500 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 80 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	// TIM1������ã�PWM���ģʽ����ʼװ��1000
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 1000;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// TIM1�ĸ�ͨ��ʹ�ܼ�Ԥװ��
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	// ����ARRԤװ�ػ�����
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	
	// ʹ��TIM��PWM���
	TIM_Cmd(TIM1, ENABLE); 
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// PWM������ĸ����
void PWM_Output(uint16_t DR1,uint16_t DR2,uint16_t DR3,uint16_t DR4)
{
	TIM_SetCompare1(TIM1, DR1);
	TIM_SetCompare2(TIM1, DR2);
	TIM_SetCompare3(TIM1, DR3);
	TIM_SetCompare4(TIM1, DR4);
}

// PWM���벶���ʼ��
void PWM_In_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// ʱ�ӿ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// GPIO���ã����츴��
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// GPIO����TIM3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);

	// �ر�TIM3
	TIM_DeInit(TIM3);
	// TIM3���ã�Ԥ��ƵΪ80������Ϊ0xFFFF�����ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 80 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// TIM3�ĸ�ͨ���������ã����������أ��˲�ֵΪ0x0B
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0B;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0B;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0B;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0B;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	// NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	// ����TIM3�ĸ�ͨ�����ж�
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC2,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC3,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_CC4,ENABLE);

	// ʹ��TIM3
	TIM_Cmd(TIM3, ENABLE); 
}

// PID����ʱ�����ö�ʱ����ʼ��
void PID_Time_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	// ʱ�ӿ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	// �ر�TIM4
	TIM_DeInit(TIM4);
	// TIM4���ã�Ԥ��ƵΪ80������Ϊ0xFFFF�����ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 80 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	// ʹ��TIM4
	TIM_Cmd(TIM4, ENABLE);
}

// AHRS����ʱ�����ö�ʱ����ʼ��
void AHRS_Time_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	// ʱ�ӿ���
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	// �ر�TIM5
	TIM_DeInit(TIM5);
	// TIM5���ã�Ԥ��ƵΪ80������Ϊ0xFFFF�����ϼ���
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = 80 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	// ʹ��TIM5
	TIM_Cmd(TIM5, ENABLE);
}

// ��ȡPID����ʱ��
float Get_PID_Time(void)
{
	float temp = 0;
	static uint32_t now = 0;

	// ʱ���ȡ
 	now = TIM4->CNT;
	// ��ʱ������
  TIM4->CNT = 0;
	// ת����λΪHZ
	temp = (float)now / 1000000.0f;

	return temp;
}

// ��ȡAHRS����ʱ���һ��
float Get_AHRS_Time(void)
{
	float temp = 0;
	static uint32_t now = 0;

	// ʱ���ȡ
	now = TIM5->CNT;
	// ��ʱ������
	TIM5->CNT = 0;
	// ת����λΪHZ��ȡһ��
	temp = (float)now / 2000000.0f;

	return temp;
}
