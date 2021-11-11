#include "bsp.h"
#include "main.h"
#include "usb_lib.h"

uint8_t PrevXferComplete;

static void PrintfLogo(void);
static void D12_BoardKeyINT_Config(void);

int main(void)
{
    bsp_Init();
    D12_BoardKeyINT_Config();
    PrintfLogo();   
    
    USB_Disconnect();
    bsp_sw_delay_ms(1500);
    USB_Connect();    
    
    /* hw_config.c */
    Set_System();
    USB_Interrupts_Config();
    Set_USBClock();
    
    /* usb_init.c */
    USB_Init();
    
    while (1)
    {    
        LED1_TOGGLE;
        bsp_sw_delay_ms(200);
        LED2_TOGGLE;
        bsp_sw_delay_ms(200);
        LED3_TOGGLE;
        bsp_sw_delay_ms(200);
        LED4_TOGGLE;
        bsp_sw_delay_ms(200);  
    }
}


/*
*********************************************************************************************************
*   函 数 名: PrintfLogo
*   功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    printf("*************************勤进电子科技************************\r\n");
    printf("                     STM32 USB Device                        \r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}


// 按键的GPIO初始化
static void D12_BoardKeyINT_Config(void)
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









