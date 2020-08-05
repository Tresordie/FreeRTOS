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
 * ��������
 */
static void PrintfLogo(void);

/**
 * ���񴴽���صĺ���
 */
static void AppTaskCreate(void);                        // ���񴴽�����
static void vTaskLED1Toggle(void *pvParameters);        // ����������
static void vTaskLED2Toggle(void *pvParameters);        // ����������
static void vTaskLED3Toggle(void *pvParameters);        // ����������
static void vTaskLED4Toggle(void *pvParameters);        // ����������

/**
 * ���񴴽����������
 */
static TaskHandle_t xHandleTaskLED1 = NULL;             // ÿ�����������񶼱�����һ��������
static TaskHandle_t xHandleTaskLED2 = NULL;             // ÿ�����������񶼱�����һ��������
static TaskHandle_t xHandleTaskLED3 = NULL;             // ÿ�����������񶼱�����һ��������
static TaskHandle_t xHandleTaskLED4 = NULL;             // ÿ�����������񶼱�����һ��������


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
 * @brief �û�����Ӧ��
 * 
 */
static void AppTaskCreate(void)
{
    xTaskCreate( vTaskLED1Toggle,           // ������
                 "vTaskLED1Toggle"          // ��������
                 64,                        // ����ջ��С(��Ϊ��λ)
                 NULL,                      // ����������
                 1,                         // �������ȼ�(����Խ�����ȼ�Խ��)
                    
                );
}












/*
*********************************************************************************************************
*   �� �� ��: PrintfLogo
*   ����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*   ��    �Σ���
*   �� �� ֵ: ��
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

