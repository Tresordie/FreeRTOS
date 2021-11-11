#ifndef _BSP_H
#define _BSP_H

/* ����ȫ���жϵĺ� */
#define ENABLE_INT()    __set_PRIMASK(0)    /* ʹ��ȫ���ж� */
#define DISABLE_INT()   __set_PRIMASK(1)    /* ��ֹȫ���ж� */

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


//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO�ڵ�ַӳ��
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO�ڲ���,ֻ�Ե�һ��IO��!
//ȷ��n��ֵС��16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //��� 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //���� 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //��� 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //��� 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //���� 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //��� 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //���� 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //��� 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //��� 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //��� 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����


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
#include "bsp_spi_flash.h"
#include "bsp_spi_tftlcd.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_exti.h"

void bsp_Init(void);

#endif

