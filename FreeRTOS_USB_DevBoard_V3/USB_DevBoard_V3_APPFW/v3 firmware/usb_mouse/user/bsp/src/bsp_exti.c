#include "bsp_exti.h"
#include "bsp_gpio.h"
#include "bsp_nvic.h"

/**
  * @brief  bsp_EXTILine_Config
  * @param  EXTI Line interrupt config
  * @retval None
  */
static void bsp_EXTILine_Config(uint32_t EXTI_LineX, EXTITrigger_TypeDef EXTI_Trigger_Edge)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    EXTI_InitStructure.EXTI_Line = EXTI_LineX;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Edge;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}


static void NVIC_Configuration(uint32_t NVIC_PriorityGroup_x, uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure; 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);

    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  bsp_BothEdge_EXTI_Init --- GPIO must be as IN_FLOATING or it will 
  * @param  BothEdge Init
  * @retval None
  */
void bsp_BothEdge_EXTI_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, uint8_t GPIO_PinSourceX, uint32_t EXTI_LineX, EXTITrigger_TypeDef EXTI_Trigger_Edge, uint32_t NVIC_PriorityGroup_x, uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
    RCC_Configuration(GPIOx);
    
    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    NVIC_Configuration(NVIC_PriorityGroup_x, NVIC_IRQChannel, NVIC_IRQChannelPreemptionPriority, NVIC_IRQChannelSubPriority);
    
    GPIOx_Configuration_Init(GPIOx, GPIO_Pin_x, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

    /* Connect EXTIx Line to GPIO_Pin_x */
    if(GPIOA == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSourceX);
    }
    else if(GPIOB == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSourceX);
    }
    else if(GPIOC == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSourceX);
    }
    else if(GPIOD == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSourceX);
    }
    else if(GPIOE == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSourceX);
    }
    else if(GPIOF == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSourceX);
    }
    else if(GPIOG == GPIOx)
    {   
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSourceX);
    }

    bsp_EXTILine_Config(EXTI_LineX, EXTI_Trigger_Edge); 
}



