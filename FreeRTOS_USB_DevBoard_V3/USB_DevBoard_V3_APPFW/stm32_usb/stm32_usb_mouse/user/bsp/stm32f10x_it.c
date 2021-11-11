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
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "usb_istr.h"
#include "main.h"
#include "bsp_sw_delay.h"
#include "bsp.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void SysTick_Handler(void)
{

}
*/

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    USB_Istr();
}


/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int8_t report_data[3];

void EXTI9_5_IRQHandler(void)
{
    // Mouse move left -> axis X(left --> negitive; right --> positive)
    if(EXTI_GetITStatus(KEY1_EXTI_Line) != RESET)
    {
        bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)){
            report_data[1] = -20;
        }else{
            report_data[1] = 0;
        }    
        // Clear the  EXTI line 5 pending bit
        EXTI_ClearITPendingBit(KEY1_EXTI_Line);
    }

    // Mouse move down -> axis Y(down --> negitive; up --> positive)
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
 
        // Clear the  EXTI line 7 pending bit
        EXTI_ClearITPendingBit(EXTI_Line7);
    }

    // Mouse move right -> axis X(left --> negitive; right --> positive)
    if(EXTI_GetITStatus(KEY4_EXTI_Line) != RESET)
    {
        bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN)){
            report_data[1] = 20;
        }else{
            report_data[1] = 0;
        }    
        // Clear the  EXTI line 8 pending bit
        EXTI_ClearITPendingBit(KEY4_EXTI_Line);
    }

    // Mouse button left
    if(EXTI_GetITStatus(KEY5_EXTI_Line) != RESET)
    {
        // left button coresponding to Key5
        bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY5_PORT, KEY5_PIN)){
            report_data[0] |= 0x01;
        }else{
            report_data[0] &= ~0x01;
        }
        // Clear the  EXTI line 9 pending bit
        EXTI_ClearITPendingBit(KEY5_EXTI_Line);
    }

    USB_SIL_Write(EP1_IN, (uint8_t *)report_data, 3);
    
    /* Enable endpoint for transmission */
    SetEPTxValid(ENDP1);
    LED2_ON;

}


/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
    // Mouse button right
    if(EXTI_GetITStatus(KEY2_EXTI_Line) != RESET)
    {
        //bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)){
            report_data[2] = -20;
        }else{
            report_data[2] = 0;
        }
        
        // Clear the EXTI line 10 pending bit
        EXTI_ClearITPendingBit(KEY2_EXTI_Line);
    }

    
    if(EXTI_GetITStatus(KEY3_EXTI_Line) != RESET)
    {
        //bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)){
            report_data[2] = 20;
        }else{
            report_data[2] = 0;
        }
        
        // Clear the EXTI line 10 pending bit
        EXTI_ClearITPendingBit(KEY3_EXTI_Line);
    }

    if(EXTI_GetITStatus(KEY6_EXTI_Line) != RESET)
    {

        //bsp_sw_delay_ms(10);
        if(RESET == GPIO_ReadInputDataBit(KEY6_PORT, KEY6_PIN)){
            report_data[0] |= 0x02;
        }else{
            report_data[0] &= ~0x02;
        }  

        // Clear the EXTI line 10 pending bit
        EXTI_ClearITPendingBit(KEY6_EXTI_Line);
    }
    
    USB_SIL_Write(EP1_IN, (uint8_t *)report_data, 3);
    
    /* Enable endpoint for transmission */
    SetEPTxValid(ENDP1);
    LED2_ON;
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
