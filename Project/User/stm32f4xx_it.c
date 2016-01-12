/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "ucos_ii.h"
#include "main.h"
#include "TIM.h"
#include "USART.h"
#include "COM.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// ��ͨ������״̬
unsigned char TIM2CH1_CAPTURE_STA = 1;
unsigned char TIM3CH1_CAPTURE_STA = 1;
unsigned char TIM3CH2_CAPTURE_STA = 1;
unsigned char TIM3CH3_CAPTURE_STA = 1;
unsigned char TIM3CH4_CAPTURE_STA = 1;

// �������½�������
uint16_t TIM3CH1_Rise, TIM3CH1_Fall,
         TIM3CH2_Rise, TIM3CH2_Fall,
         TIM3CH3_Rise, TIM3CH3_Fall,
         TIM3CH4_Rise, TIM3CH4_Fall;

// ����������
uint16_t TIM3_T;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles TIM3 exception.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	// CH1 - AIL - Roll
	// �����¼�
  if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  {
		// ����жϱ�־λ
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		// ����������
		if(TIM3CH1_CAPTURE_STA == 1)
		{
			// ��ȡ����������
			TIM3CH1_Rise = TIM_GetCapture1(TIM3);
			// ״̬��־��Ϊ�½���
			TIM3CH1_CAPTURE_STA = 0;
			// ����Ϊ�½��ز���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);			  
		}
		// �����½���
		else
		{
			// ��ȡ�½�������
			TIM3CH1_Fall = TIM_GetCapture1(TIM3);
			// ״̬��־��Ϊ������
			TIM3CH1_CAPTURE_STA = 1;
			
			// �������
			if(TIM3CH1_Fall < TIM3CH1_Rise)
			{
				TIM3_T = 65535;
			}
			else
			{
				TIM3_T = 0;
			}
			
			// �½��ؼ�ȥ�����صõ��ܵĸߵ�ƽʱ��
			PWMInCh1 = TIM3CH1_Fall - TIM3CH1_Rise + TIM3_T;
			// ����Ϊ�����ز���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
		}		    
  }

  // CH2 - ELE - Pitch	  
  if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  {	
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);

		if(TIM3CH2_CAPTURE_STA == 1)
		{ 
			TIM3CH2_Rise = TIM_GetCapture2(TIM3);
			TIM3CH2_CAPTURE_STA = 0;
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling); 			    			  
		}
		else
		{
			TIM3CH2_Fall = TIM_GetCapture2(TIM3);
			TIM3CH2_CAPTURE_STA = 1;
			if(TIM3CH2_Fall < TIM3CH2_Rise)
			{
				TIM3_T = 65535;
			}
			else
			{
				TIM3_T = 0;
			}	
			PWMInCh2 = TIM3CH2_Fall - TIM3CH2_Rise + TIM3_T;
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);	
		}		    
  }

  // CH3 - THR - Acc  
  if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)
  {	
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);

		if(TIM3CH3_CAPTURE_STA == 1)
		{ 
			TIM3CH3_Rise = TIM_GetCapture3(TIM3);
			TIM3CH3_CAPTURE_STA = 0;
			TIM_OC3PolarityConfig(TIM3,TIM_ICPolarity_Falling);			    			  
		}
		else
		{
			TIM3CH3_Fall = TIM_GetCapture3(TIM3);
			TIM3CH3_CAPTURE_STA = 1;
			if(TIM3CH3_Fall < TIM3CH3_Rise)
			{
				TIM3_T = 65535;
			}
			else
			{
				TIM3_T = 0;
			}	
      PWMInCh3 = TIM3CH3_Fall - TIM3CH3_Rise + TIM3_T;
      TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);	
		}		    
  }
	
  // CH4 - RUD -Yaw	  
  if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
  {	
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

		if(TIM3CH4_CAPTURE_STA == 1)
		{ 
			TIM3CH4_Rise = TIM_GetCapture4(TIM3);
			TIM3CH4_CAPTURE_STA = 0;
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);			    			  
		}
		else
		{
			TIM3CH4_Fall = TIM_GetCapture4(TIM3); 
			TIM3CH4_CAPTURE_STA = 1;
			if(TIM3CH4_Fall < TIM3CH4_Rise)
			{
				TIM3_T = 65535;
			}
			else
			{
				TIM3_T = 0;
			}	
      PWMInCh4 = TIM3CH4_Fall - TIM3CH4_Rise + TIM3_T;
      TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);		
		}		    
  }
}
/**
  * @brief  This function handles USART2 exception.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
	static uint8_t Rxcnt=0;
	
	// �����¼�
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
		// �����־λ���жϱ�־λ
    USART_ClearFlag(USART2,USART_FLAG_RXNE); 
    USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
		
		// ��ȡ���ڽ�������
    ComRxBuffer[Rxcnt] = (uint8_t)USART_ReceiveData(USART2);
		// �±����
		Rxcnt++;
		// ��32λ���������������½���
		if(Rxcnt == 4)
    {
      Rxcnt = 0;
    }
  }
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{
}*/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	#ifdef OS_CRITICAL_METHOD
	  OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();        /* Tell uC/OS-II that we are starting an ISR */
    OSIntNesting++;
    OS_EXIT_CRITICAL();

    OSTimeTick();               /* Call uC/OS-II's OSTimeTick()              */

    OSIntExit();                /* Tell uC/OS-II that we are leaving the ISR */
	#endif
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
