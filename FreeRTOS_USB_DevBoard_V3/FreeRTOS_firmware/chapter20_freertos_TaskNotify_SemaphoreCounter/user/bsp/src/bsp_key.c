#include "bsp_key.h"
#include "bsp_gpio.h"
#include "bsp_exti.h"

void bsp_InitKeysAsBothEdgeINT(void)
{
	bsp_BothEdge_EXTI_Init(KEY1_PORT, KEY1_PIN, KEY1_GPIO_PinSource, KEY1_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY1_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY2_PORT, KEY2_PIN, KEY2_GPIO_PinSource, KEY2_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY2_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY3_PORT, KEY3_PIN, KEY3_GPIO_PinSource, KEY3_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY3_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY4_PORT, KEY4_PIN, KEY4_GPIO_PinSource, KEY4_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY4_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY5_PORT, KEY5_PIN, KEY5_GPIO_PinSource, KEY5_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY5_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY6_PORT, KEY6_PIN, KEY6_GPIO_PinSource, KEY6_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY6_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY7_PORT, KEY7_PIN, KEY7_GPIO_PinSource, KEY7_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY7_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY8_PORT, KEY8_PIN, KEY8_GPIO_PinSource, KEY8_EXTI_Line, EXTI_Trigger_Rising_Falling, NVIC_PriorityGroup_4, 
    KEY8_EXTI_IRQn, 2, 0);
}


void bsp_InitKeysAsRisingEdgeINT(void)
{
	bsp_BothEdge_EXTI_Init(KEY1_PORT, KEY1_PIN, KEY1_GPIO_PinSource, KEY1_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY1_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY2_PORT, KEY2_PIN, KEY2_GPIO_PinSource, KEY2_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY2_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY3_PORT, KEY3_PIN, KEY3_GPIO_PinSource, KEY3_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY3_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY4_PORT, KEY4_PIN, KEY4_GPIO_PinSource, KEY4_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY4_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY5_PORT, KEY5_PIN, KEY5_GPIO_PinSource, KEY5_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY5_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY6_PORT, KEY6_PIN, KEY6_GPIO_PinSource, KEY6_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY6_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY7_PORT, KEY7_PIN, KEY7_GPIO_PinSource, KEY7_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY7_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY8_PORT, KEY8_PIN, KEY8_GPIO_PinSource, KEY8_EXTI_Line, EXTI_Trigger_Rising, NVIC_PriorityGroup_4, 
    KEY8_EXTI_IRQn, 2, 0);
}


void bsp_InitKeysAsFallingEdgeINT(void)
{
	bsp_BothEdge_EXTI_Init(KEY1_PORT, KEY1_PIN, KEY1_GPIO_PinSource, KEY1_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY1_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY2_PORT, KEY2_PIN, KEY2_GPIO_PinSource, KEY2_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY2_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY3_PORT, KEY3_PIN, KEY3_GPIO_PinSource, KEY3_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY3_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY4_PORT, KEY4_PIN, KEY4_GPIO_PinSource, KEY4_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY4_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY5_PORT, KEY5_PIN, KEY5_GPIO_PinSource, KEY5_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY5_EXTI_IRQn, 1, 0);

    bsp_BothEdge_EXTI_Init(KEY6_PORT, KEY6_PIN, KEY6_GPIO_PinSource, KEY6_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY6_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY7_PORT, KEY7_PIN, KEY7_GPIO_PinSource, KEY7_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY7_EXTI_IRQn, 2, 0);
    
    bsp_BothEdge_EXTI_Init(KEY8_PORT, KEY8_PIN, KEY8_GPIO_PinSource, KEY8_EXTI_Line, EXTI_Trigger_Falling, NVIC_PriorityGroup_4, 
    KEY8_EXTI_IRQn, 2, 0);
}


void bsp_InitKeysAsInput(void)
{
	GPIOx_Configuration_Init(KEY1_PORT, KEY1_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY2_PORT, KEY2_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY3_PORT, KEY3_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY4_PORT, KEY4_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY5_PORT, KEY5_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY6_PORT, KEY6_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY7_PORT, KEY7_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);

	GPIOx_Configuration_Init(KEY8_PORT, KEY8_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz);
}


uint8_t KeyScan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	uint8_t ret = 0;

	if(!GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x))
	{
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x));			/** wait key released */
		ret = 1;
	}
	else
	{
		/* code */
		ret = 0;
	}

	return ret;	
}

