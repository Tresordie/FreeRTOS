#ifndef _BSP_H
#define _BSP_H

/* 开关全局中断的宏 */
#define ENABLE_INT()    __set_PRIMASK(0)    /* 使能全局中断 */
#define DISABLE_INT()   __set_PRIMASK(1)    /* 禁止全局中断 */

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef TRUE
    #define TRUE  1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

//LED port
#define LEDGP1_PORT            GPIOB
#define LEDGP1_RCC             RCC_APB2Periph_GPIOB
#define LED1_PIN               GPIO_Pin_11
#define LED2_PIN               GPIO_Pin_10

#define LEDGP2_PORT            GPIOC
#define LEDGP2_RCC             RCC_APB2Periph_GPIOC
#define LED3_PIN               GPIO_Pin_5
#define LED4_PIN               GPIO_Pin_4

// macro to operate LEDs GPIO
#define digitalHigh(p, i)      {p->BSRR = i;}       // set GPIO level high
#define digitalLow(p, i)       {p->BRR = i;}        // set GPIO level low
#define digitalToggle(p, i)    {p->ODR ^= i;}       // toggle GPIO level 

#define LED1_TOGGLE            digitalToggle(LEDGP1_PORT, LED1_PIN)
#define LED2_TOGGLE            digitalToggle(LEDGP1_PORT, LED2_PIN)
#define LED3_TOGGLE            digitalToggle(LEDGP2_PORT, LED3_PIN)
#define LED4_TOGGLE            digitalToggle(LEDGP2_PORT, LED4_PIN)

#define LED1_ON                digitalLow(LEDGP1_PORT, LED1_PIN)
#define LED1_OFF               digitalHigh(LEDGP1_PORT, LED1_PIN)

#define LED2_ON                digitalLow(LEDGP1_PORT, LED2_PIN)
#define LED2_OFF               digitalHigh(LEDGP1_PORT, LED2_PIN)

#define LED3_ON                digitalLow(LEDGP2_PORT, LED3_PIN)
#define LED3_OFF               digitalHigh(LEDGP2_PORT, LED3_PIN)

#define LED4_ON                digitalLow(LEDGP2_PORT, LED4_PIN)
#define LED4_OFF               digitalHigh(LEDGP2_PORT, LED4_PIN)


#include "bsp_gpio.h"
#include "bsp_nvic.h"
#include "bsp_sw_delay.h"
#include "bsp_i2c_hw.h"

void bsp_Init(void);

#endif

