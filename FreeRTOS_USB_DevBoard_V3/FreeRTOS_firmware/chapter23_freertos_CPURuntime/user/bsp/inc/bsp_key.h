#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "stm32f10x.h"

//Key board
#define KEY1_PORT              GPIOD
#define KEY1_PIN               GPIO_Pin_8
#define KEY1_GPIO_PinSource    GPIO_PinSource8
#define KEY1_EXTI_Line         EXTI_Line8
#define KEY1_EXTI_IRQn         EXTI9_5_IRQn

#define KEY2_PORT              GPIOD
#define KEY2_PIN               GPIO_Pin_10
#define KEY2_GPIO_PinSource    GPIO_PinSource10
#define KEY2_EXTI_Line         EXTI_Line10
#define KEY2_EXTI_IRQn         EXTI15_10_IRQn

#define KEY3_PORT              GPIOD
#define KEY3_PIN               GPIO_Pin_12
#define KEY3_GPIO_PinSource    GPIO_PinSource12
#define KEY3_EXTI_Line         EXTI_Line12
#define KEY3_EXTI_IRQn         EXTI15_10_IRQn

#define KEY4_PORT              GPIOC
#define KEY4_PIN               GPIO_Pin_6
#define KEY4_GPIO_PinSource    GPIO_PinSource6
#define KEY4_EXTI_Line         EXTI_Line6
#define KEY4_EXTI_IRQn         EXTI9_5_IRQn


#define KEY5_PORT              GPIOD
#define KEY5_PIN               GPIO_Pin_9
#define KEY5_GPIO_PinSource    GPIO_PinSource9
#define KEY5_EXTI_Line         EXTI_Line9
#define KEY5_EXTI_IRQn         EXTI9_5_IRQn

#define KEY6_PORT              GPIOD
#define KEY6_PIN               GPIO_Pin_11
#define KEY6_GPIO_PinSource    GPIO_PinSource11
#define KEY6_EXTI_Line         EXTI_Line11
#define KEY6_EXTI_IRQn         EXTI15_10_IRQn

#define KEY7_PORT              GPIOD
#define KEY7_PIN               GPIO_Pin_15
#define KEY7_GPIO_PinSource    GPIO_PinSource15
#define KEY7_EXTI_Line         EXTI_Line15
#define KEY7_EXTI_IRQn         EXTI15_10_IRQn

#define KEY8_PORT              GPIOC
#define KEY8_PIN               GPIO_Pin_7
#define KEY8_GPIO_PinSource    GPIO_PinSource7
#define KEY8_EXTI_Line         EXTI_Line7
#define KEY8_EXTI_IRQn         EXTI9_5_IRQn


void bsp_InitKeysAsBothEdgeINT(void);
void bsp_InitKeysAsRisingEdgeINT(void);
void bsp_InitKeysAsFallingEdgeINT(void);
void bsp_InitKeysAsInput(void);
uint8_t KeyScan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);

#endif
