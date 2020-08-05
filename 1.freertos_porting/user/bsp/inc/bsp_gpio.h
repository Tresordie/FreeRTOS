#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

#include "stm32f10x.h"

void bsp_InitRCC(GPIO_TypeDef* GPIOx);
void bsp_InitGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x, GPIOMode_TypeDef GPIO_Mode, GPIOSpeed_TypeDef GPIO_Speed);
void bsp_setIO_OutputHigh(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
void bsp_setIO_OutputLow(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);
void JTAG_GPIO_DISABLE(void);

#endif

