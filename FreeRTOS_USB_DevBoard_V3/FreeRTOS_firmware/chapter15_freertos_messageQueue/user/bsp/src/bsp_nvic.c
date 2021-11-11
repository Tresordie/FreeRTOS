#include "bsp_nvic.h"


/**
  * @brief  bsp_InitNVIC
  * @param  None
  * @retval None
  */
void bsp_InitNVIC(uint32_t NVIC_PriorityGroup_x, uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x);

    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


unsigned int RING_FIFO_ENTER_CRITICAL(void) 
{
    unsigned int pri_mask;
    pri_mask = __get_PRIMASK();
    __set_PRIMASK(1);

    return pri_mask;
}


void RING_FIFO_EXIT_CRITICAL(unsigned int pri_mask)
{
    __set_PRIMASK(pri_mask);
}

