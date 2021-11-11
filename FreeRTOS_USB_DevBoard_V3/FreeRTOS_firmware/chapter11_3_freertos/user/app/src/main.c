#include "bsp.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


/*****************************************************************
  * @brief  任务创建：任务的三要素(任务的主体函数，任务的栈，任务控制块)
  * @param  无
  * @retval 无
  * @note   第一步：任务栈创建
            第二步：任务控制块
            第三步：任务的主体函数
            第四步：任务创建成功与否的任务句柄
            第五步：定时器任务及空闲任务的任务栈及相应的任务控制块
  ****************************************************************/

// 第一步：任务栈创建 
#define USER_STACK_SIZE                 512

//第二步：任务控制块


//第三步：任务的主体函数
static void AppTaskCreate(void);
static void LED1_Task(void *pvParameters);
static void LED2_Task(void *pvParameters);
static void LED3_Task(void *pvParameters);
static void LED4_Task(void *pvParameters);

//第四步：任务创建成功与否的任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED1_Task_Handle;
static TaskHandle_t LED2_Task_Handle;
static TaskHandle_t LED3_Task_Handle;
static TaskHandle_t LED4_Task_Handle;

//第五步：任务创建成功与否的任务句柄


static void PrintfLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;      /** 创建信息返回值 */
    
    bsp_Init();
    PrintfLogo();
    printf("FreeRTOS动态内存创建任务实验!\r\n");

    // 创建AppTaskCreate任务, 静态内存分配是使用的是xTaskCreateStatic
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,               // 任务函数
                          (const char*)"AppTaskCreate",                // 任务名称
                          (uint32_t)USER_STACK_SIZE,                   // 任务栈大小
                          (void *)NULL,                                // 任务函数的传参
                          (UBaseType_t)1,                              // 任务的优先级
                          (TaskHandle_t*)&AppTaskCreate_Handle);          // 任务控制块
    if(pdPASS == xReturn)
    {
        vTaskStartScheduler();
    }
    else
    {
		return -1;
    }

    while (1);
}




/*****************************************************************
  * @brief  AppTaskCreate
  * @param  为了方便管理，所有的任务创建函数都放在这个函数里面
  * @retval 无
  * @note   无
  ****************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    /* LED task create */
    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,
                          (const char*)"LED1_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)5,
                          (TaskHandle_t*)&LED1_Task_Handle);    
    if(pdPASS == xReturn)
    {
        printf("LED1_Task任务创建成功!\r\n");
		
    }
    else
    {
        printf("LED1_Task任务创建失败!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)LED2_Task,
                          (const char*)"LED2_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t*)&LED2_Task_Handle);   
    if(pdPASS == xReturn)
    {
        printf("LED2_Task任务创建成功!\r\n");
		
    }
    else
    {
        printf("LED2_Task任务创建失败!\r\n");
    }
    
    
    xReturn = xTaskCreate((TaskFunction_t)LED3_Task,
                          (const char*)"LED3_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&LED3_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("LED3_Task任务创建成功!\r\n");
		
    }
    else
    {
        printf("LED3_Task任务创建失败!\r\n");
    }
    
        
    xReturn = xTaskCreate((TaskFunction_t)LED4_Task,
                          (const char*)"LED4_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&LED4_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("LED4_Task任务创建成功!\r\n");
		
    }
    else
    {
        printf("LED4_Task任务创建失败!\r\n");
    }

    
    vTaskDelete(AppTaskCreate_Handle);
    
    taskEXIT_CRITICAL();
}


/*****************************************************************
  * @brief  LED_Task
  * @param  LED_Task任务主体
  * @retval 无
  * @note   无
  ****************************************************************/
static void LED1_Task(void* parameter)
{
    while(1)
    {
        LED1_ON;
        vTaskDelay(200);
        printf("LED1_Task running, LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(200);
        printf("LED1_Task running, LED1_OFF\r\n");
    }
}


static void LED2_Task(void* parameter)
{
    while(1)
    {
        LED2_ON;
        vTaskDelay(500);
        printf("LED2_Task running, LED2_ON\r\n");

        LED2_OFF;
        vTaskDelay(500);
        printf("LED2_Task running, LED2_OFF\r\n");
    }
}


static void LED3_Task(void* parameter)
{
    while(1)
    {
        LED3_ON;
        vTaskDelay(300);
        printf("LED3_Task running, LED3_ON\r\n");

        LED3_OFF;
        vTaskDelay(300);
        printf("LED3_Task running, LED3_OFF\r\n");
    }

}


static void LED4_Task(void* parameter)
{
    while(1)
    {
        LED4_ON;
        vTaskDelay(600);
        printf("LED4_Task running, LED4_ON\r\n");

        LED4_OFF;
        vTaskDelay(600);
        printf("LED4_Task running, LED4_OFF\r\n");
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
    printf("*********************勤进电子科技FreeRTOS移植****************\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

