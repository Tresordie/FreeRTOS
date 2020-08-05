/**
 * @file main.c
 * @author simon.yuan (Tresordie@163.com)
 * @brief  FreeRTOS kernel porting
 * @version 0.1
 * @date 2020-08-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "bsp.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/**
 * 函数声明
 */
static void PrintfLogo(void);

/**
 * 任务创建相关的函数
 */
static void AppTaskCreate(void);                        // 任务创建函数
static void vTaskLED1Toggle(void *pvParameters);        // 创建的任务
static void vTaskLED2Toggle(void *pvParameters);        // 创建的任务
static void vTaskLED3Toggle(void *pvParameters);        // 创建的任务
static void vTaskLED4Toggle(void *pvParameters);        // 创建的任务

/**
 * 任务创建后的任务句柄
 */
static TaskHandle_t xHandleTaskLED1 = NULL;             // 每个创建的任务都必须有一个任务句柄
static TaskHandle_t xHandleTaskLED2 = NULL;             // 每个创建的任务都必须有一个任务句柄
static TaskHandle_t xHandleTaskLED3 = NULL;             // 每个创建的任务都必须有一个任务句柄
static TaskHandle_t xHandleTaskLED4 = NULL;             // 每个创建的任务都必须有一个任务句柄


int main(void)
{
    bsp_Init();

    PrintfLogo();

    // create user application
    AppTaskCreate();

    // enable kernenl scheduler
    vTaskStartScheduler();

    while (1);
}



/**
 * @brief 用户创建应用
 * 
 */
static void AppTaskCreate(void)
{
    xTaskCreate( vTaskLED1Toggle,           // 任务函数
                 "vTaskLED1Toggle"          // 任务名称
                 64,                        // 任务栈大小(字为单位)
                 NULL,                      // 任务函数参数
                 1,                         // 任务优先级(数字越大，优先级越高)
                    
                );
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
    printf("*************************************************************\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

