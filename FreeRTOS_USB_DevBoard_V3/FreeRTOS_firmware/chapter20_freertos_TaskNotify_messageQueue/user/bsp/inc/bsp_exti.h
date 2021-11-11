#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

#include "stm32f10x.h"

void bsp_BothEdge_EXTI_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, uint8_t GPIO_PinSourceX, uint32_t EXTI_LineX, EXTITrigger_TypeDef EXTI_Trigger_Edge, uint32_t NVIC_PriorityGroup_x, uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority);

#endif


