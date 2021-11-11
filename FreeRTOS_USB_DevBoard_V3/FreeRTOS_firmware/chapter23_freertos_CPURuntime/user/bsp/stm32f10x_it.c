/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "FreeRTOS.h"					//FreeRTOS使用		  
#include "task.h" 
#include "queue.h"
#include "semphr.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

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
//void SVC_Handler(void)
//{
//}

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
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void xPortSysTickHandler(void);
//systick中断服务函数
void SysTick_Handler(void)
{	
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
      {
    #endif  /* INCLUDE_xTaskGetSchedulerState */  
        xPortSysTickHandler();
    #if (INCLUDE_xTaskGetSchedulerState  == 1 )
      }
    #endif  /* INCLUDE_xTaskGetSchedulerState */
}

/*
extern QueueHandle_t 		TestQueue_Handle;
extern SemaphoreHandle_t 	SemaphoreBinary_Handle;

static uint32_t send_data1 = 1;
static uint32_t send_data2 = 2;
*/

/**
  * @brief		Key1 -- PD8		Key2 -- PD10
  * @atten		Key1 pressed, send_data1 written into message queue
  * 			Key2 pressed, send_data2 written into message queue
  */
 /*
void EXTI9_5_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
	uint32_t ulReturn;

	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if (EXTI_GetITStatus(EXTI_Line8) != RESET){
		xQueueSendFromISR(TestQueue_Handle, &send_data1, &pxHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}

void EXTI15_10_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
	uint32_t ulReturn;

	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if (EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		xQueueSendFromISR(TestQueue_Handle, &send_data2, &pxHigherPriorityTaskWoken);

		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

		EXTI_ClearITPendingBit(EXTI_Line10);
	}
	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}


void USART1_IRQHandler(void)
{
	BaseType_t pxHigherPriorityTaskWoken;
	uint32_t ulReturn;

	ulReturn = taskENTER_CRITICAL_FROM_ISR();

	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET){
		DMA_Cmd(DMA1_Channel5, DISABLE);		// 关闭中断，防止干扰
		DMA_ClearFlag(DMA1_FLAG_TC5);				// 清除发送完成标志位
		DMA1_Channel5->CNDTR = 1000;
		DMA_Cmd(DMA1_Channel5, ENABLE);

		xSemaphoreGiveFromISR(SemaphoreBinary_Handle, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

		USART_ReceiveData(USART1);					// Clear interrupt flag
	}


	taskEXIT_CRITICAL_FROM_ISR(ulReturn);
}
*/

// CPU run time statistic
volatile uint32_t CPU_RunTime = 0ul;

void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){
		CPU_RunTime++;
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
	}
}


	/******************************************************************************/
	/*                 STM32F10x Peripherals Interrupt Handlers                   */
	/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
	/*  available peripheral interrupt handler's name please refer to the startup */
	/*  file (startup_stm32f10x_xx.s).                                            */
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

	/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
