#ifndef __BSP_NVIC_H__
#define __BSP_NVIC_H__

#include "stm32f10x.h"

unsigned int RING_FIFO_ENTER_CRITICAL(void) ;
void RING_FIFO_EXIT_CRITICAL(unsigned int pri_mask);
void bsp_InitNVIC(uint32_t NVIC_PriorityGroup_x, uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority);

#endif

