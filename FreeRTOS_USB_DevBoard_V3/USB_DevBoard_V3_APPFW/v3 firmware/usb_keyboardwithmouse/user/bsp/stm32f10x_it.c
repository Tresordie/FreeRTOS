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
#include "usb.h"

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


/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/






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
void SysTick_Handler(void)
{
    //TimingDelay_Decrement();

}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void RTC_IRQHandler(void)
{
}
*/

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* Toggle LED */
        

        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
    
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
}

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
void USB_LP_CAN_RX0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
// report_data[0] : Special function buttons. D0 ~ left Ctrl, D1 ~ left Shift, D2 ~ left Alt, D3 ~ left Win
//                                            D4 ~ right Ctrl, D5 ~ right Shift, D6 ~ right Alt, D7 ~ right Win
// report_data[1] : Reserved fro OEM
// report_data[2] : Number 1 of small number keyboard
// report_data[3] : Number 2 of small number keyboard
// report_data[4] : Number 3 of small number keyboard
// report_data[5] : Caps key
// report_data[6] : Number key enable/disable
// report_data[7] : 
int8_t report_data[4] = {0};
extern uint8_t ConfigValue;


void EXTI9_5_IRQHandler(void)
{
    // Key8 pressed --> used as Keyboard
    if(!GPIO_ReadInputDataBit(KEY8_PORT, KEY8_PIN))
    {
        report_data[0] = 0x01;
        if(EXTI_GetITStatus(KEY1_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)){
                report_data[1] |= 0x01;
            }else{
                report_data[1] &= ~0x01;
            }
            // Clear the  EXTI line 5 pending bit
            EXTI_ClearITPendingBit(KEY1_EXTI_Line);
        }

        if(EXTI_GetITStatus(KEY4_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN)){
                report_data[1] |= 0x08;
            }else{
                report_data[1] &= ~0x08;
            } 
            // Clear the  EXTI line 8 pending bit
            EXTI_ClearITPendingBit(KEY4_EXTI_Line);
        }

        if(EXTI_GetITStatus(KEY5_EXTI_Line) != RESET)
        {
            // left button coresponding to Key5
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY5_PORT, KEY5_PIN)){
                report_data[1] |= 0x10;
            }else{
                report_data[1] &= ~0x10;
            }
            // Clear the  EXTI line 9 pending bit
            EXTI_ClearITPendingBit(KEY5_EXTI_Line);
        }
        
        if(0 != ConfigValue){
            D12WriteEndpointBuffer(Ep1_IN, 2, report_data);
        }
    }
    else                        // Key8 not pressed --> used as Mouse
    {
        report_data[0] = 0x02; 
        
        // Mouse move left -> axis X(left --> negitive; right --> positive)
        if(EXTI_GetITStatus(KEY1_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY1_PORT, KEY1_PIN)){
                report_data[2] = -20;
            }else{
                report_data[2] = 0;
            }    
            // Clear the  EXTI line 5 pending bit
            EXTI_ClearITPendingBit(KEY1_EXTI_Line);
        }

        // Mouse move right -> axis X(left --> negitive; right --> positive)
        if(EXTI_GetITStatus(KEY4_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY4_PORT, KEY4_PIN)){
                report_data[2] = 20;
            }else{
                report_data[2] = 0;
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
                report_data[1] |= 0x01;
            }else{
                report_data[1] &= ~0x01;
            }
            // Clear the  EXTI line 9 pending bit
            EXTI_ClearITPendingBit(KEY5_EXTI_Line);
        }

        if(0 != ConfigValue){
            D12WriteEndpointBuffer(Ep1_IN, sizeof(report_data), report_data);
            LEDsControl(LED1, LED_ON);
        }
    }
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__IO uint16_t ICValue1 = 0;
__IO float DutyCycle1 = 0;
__IO float Frequency1 = 0;

void TIM2_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__IO uint16_t ICValue2 = 0;
__IO float DutyCycle2 = 0;
__IO float Frequency2 = 0;

void TIM3_IRQHandler(void)
{

}


/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void USART1_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
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
    // Key8 pressed --> used as Keyboard
    if(!GPIO_ReadInputDataBit(KEY8_PORT, KEY8_PIN))
    {
        report_data[0] = 0x01;
        
        if(EXTI_GetITStatus(KEY2_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)){
                report_data[1] |= 0x02;
            }else{
                report_data[1] &= ~0x02;
            }  
            // Clear the EXTI line 10 pending bit
            EXTI_ClearITPendingBit(KEY2_EXTI_Line);
        }
        
        if(EXTI_GetITStatus(KEY3_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)){
                report_data[1] |= 0x04;
            }else{
                report_data[1] &= ~0x04;
            }
            
            // Clear the EXTI line 10 pending bit
            EXTI_ClearITPendingBit(KEY3_EXTI_Line);
        }    
        
        if(EXTI_GetITStatus(KEY6_EXTI_Line) != RESET)
        {

            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY6_PORT, KEY6_PIN)){
                report_data[1] |= 0x20;
            }else{
                report_data[1] &= ~0x20;
            }  

            EXTI_ClearITPendingBit(KEY6_EXTI_Line);
        }    
              
        if(EXTI_GetITStatus(KEY7_EXTI_Line) != RESET)
        {

            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY7_PORT, KEY7_PIN)){
                report_data[1] |= 0x40;
            }else{
                report_data[1] &= ~0x40;
            }  

            EXTI_ClearITPendingBit(KEY7_EXTI_Line);
        }  
        
        if(0 != ConfigValue){
            D12WriteEndpointBuffer(Ep1_IN, 2, report_data);
        }
    }
    else                // used as Mouse
    {
        report_data[0] = 0x02;
        
        // Mouse button right
        if(EXTI_GetITStatus(KEY2_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY2_PORT, KEY2_PIN)){
                report_data[3] = -20;
            }else{
                report_data[3] = 0;
            }
            
            EXTI_ClearITPendingBit(KEY2_EXTI_Line);
        }

        
        if(EXTI_GetITStatus(KEY3_EXTI_Line) != RESET)
        {
            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY3_PORT, KEY3_PIN)){
                report_data[3] = 20;
            }else{
                report_data[3] = 0;
            }
            
            // Clear the EXTI line 10 pending bit
            EXTI_ClearITPendingBit(KEY3_EXTI_Line);
        }

        if(EXTI_GetITStatus(KEY6_EXTI_Line) != RESET)
        {

            bsp_sw_delay_ms(10);
            if(RESET == GPIO_ReadInputDataBit(KEY6_PORT, KEY6_PIN)){
                report_data[1] |= 0x02;
            }else{
                report_data[1] &= ~0x02;
            }  

            // Clear the EXTI line 10 pending bit
            EXTI_ClearITPendingBit(KEY6_EXTI_Line);
        }
        
        if(0 != ConfigValue){
            D12WriteEndpointBuffer(Ep1_IN, sizeof(report_data), report_data);
            LEDsControl(LED2, LED_ON);
        }
    } 
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
__IO uint16_t ICValue3 = 0;
__IO float DutyCycle3 = 0;
__IO float Frequency3 = 0;

void TIM8_CC_IRQHandler(void)
{
    /*
    TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);

    ICValue3 = TIM_GetCapture1(TIM8);
    if (ICValue3 != 0)
    {
        DutyCycle3 = ((float)TIM_GetCapture2(TIM8) * 100) / ICValue3;
        Frequency3 = SystemCoreClock / (float)ICValue3;
        DEBUG("TIM8 Duty: %0.2f%%   TIM8 Freq: %0.2fHz\n",DutyCycle3, (Frequency3/7200));
    }
    else
    {
        DutyCycle3 = 0;
        Frequency3 = 0;
    }
    */

    
    /*
    if(TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET )               
    {   
        TIM_ICUserValueStructure.Capture_OverflowPeriodCnt++;       
        TIM_ClearITPendingBit (TIM8, TIM_FLAG_Update);      
    }

    if(TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET)
    {
        TIM_ICUserValueStructure.Capture_EdgeCounter++;
        if((TIM_ICUserValueStructure.Capture_StartFlag == 0) && (TIM_ICUserValueStructure.Capture_EdgeCounter == 1))
        {
            TIM_SetCounter(TIM8, 0);
            TIM_ICUserValueStructure.Capture_OverflowPeriodCnt = 0; 
            TIM_ICUserValueStructure.Capture_CurrentCcrValue = 0;
            TIM_OC3PolarityConfig(TIM8, END_ICPolarity);    
            TIM_ICUserValueStructure.Capture_StartFlag = 1;         
        }
        else if((TIM_ICUserValueStructure.Capture_StartFlag == 1) && (TIM_ICUserValueStructure.Capture_EdgeCounter == 2))
        {
            TIM_ICUserValueStructure.Capture_CurrentCcrValue = TIM_GetCapture3(TIM8);
            TIM_OC3PolarityConfig(TIM8, START_ICPolarity);
            TIM_ICUserValueStructure.Capture_DutyCycle = TIM_ICUserValueStructure.Capture_OverflowPeriodCnt*(TIMx_Period_Para + 1) +  \
                                                        (TIM_ICUserValueStructure.Capture_CurrentCcrValue + 1);
                
        }
        else if((TIM_ICUserValueStructure.Capture_StartFlag == 1) && (TIM_ICUserValueStructure.Capture_EdgeCounter == 3))
        {
            TIM_ICUserValueStructure.Capture_CurrentCcrValue = TIM_GetCapture3(TIM8);
            TIM_ICUserValueStructure.Capture_Frequency = TIM_ICUserValueStructure.Capture_OverflowPeriodCnt*(TIMx_Period_Para + 1) +  \
                                                        (TIM_ICUserValueStructure.Capture_CurrentCcrValue + 1);
            
            DEBUG("\r\n");
            DEBUG("*--------------------------------------------------------------*");
            DEBUG("PWM_DutyCycle is %0.2f%%\r\n", (float)(TIM_ICUserValueStructure.Capture_DutyCycle*100)/TIM_ICUserValueStructure.Capture_Frequency);
            DEBUG("PWM_Frequency is %0.2f Hz\r\n", (float)SystemCoreClock/(TIMx_Prescaler_Para*TIM_ICUserValueStructure.Capture_Frequency));
            DEBUG("PWM_Period is %0.2f us\r\n", (float)(TIMx_Prescaler_Para*TIM_ICUserValueStructure.Capture_Frequency)/(SystemCoreClock/1000000));     
            DEBUG("*--------------------------------------------------------------*");
            DEBUG("\r\n");
            //TIM_ICUserValueStructure.Capture_StartFlag = 0; 
            //TIM_ICUserValueStructure.Capture_FinishFlag = 1;
            memset(&TIM_ICUserValueStructure, 0, sizeof(TIM_ICUserValueStructure));
        }
        TIM_ClearITPendingBit (TIM8, TIM_IT_CC3);       
    }
    */
}


/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
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