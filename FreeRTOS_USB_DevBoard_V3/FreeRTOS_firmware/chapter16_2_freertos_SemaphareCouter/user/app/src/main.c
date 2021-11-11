/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"

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
#define USER_STACK_SIZE                 128

//第二步：任务控制块
//使用动态库，此时os使用malloc自动分配空间给任务控制块

//第三步：任务的主体函数
static void AppTaskCreate(void);

//第四步：任务创建成功与否的任务句柄
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/**
  *		消息队列
  */
// 1. 定义一个消息队列句柄
SemaphoreHandle_t SemaphoreCounter_Handle = NULL;

// 2. 定义消息队列的长度及大小

// 3. 消息队列的任务句柄(接收消息任务及发送消息任务)
static TaskHandle_t Semaphore_Take_Task_Handle;
static TaskHandle_t Semaphore_Give_Task_Handle;

// 4. 接收任务, 发送任务函数
static void SemaphoreTakeTask(void *pvParameters);
static void SemaphoreGiveTask(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;      /** 创建信息返回值 */
    
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS二值信号量创建任务实验!\r\n");

    // 创建AppTaskCreate任务, 静态内存分配是使用的是xTaskCreateStatic
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,               // 任务函数
                          (const char*)"AppTaskCreate",                // 任务名称
                          (uint32_t)USER_STACK_SIZE,                   // 任务栈大小
                          (void *)NULL,                                // 任务函数的传参
                          (UBaseType_t)1,                              // 任务的优先级
                          (TaskHandle_t*)&AppTaskCreate_Handle);       // 任务控制块
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

    SemaphoreCounter_Handle = xSemaphoreCreateCounting(5, 5);                   // Init counter and MAX counter
    if(NULL != SemaphoreCounter_Handle){
        printf("SemaphoreCounter_Handle计数信号量创建成功！\r\n");
    }

    xReturn = xTaskCreate((TaskFunction_t)SemaphoreTakeTask,
                          (const char*)"SemaphoreTakeTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&Semaphore_Take_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("SemaphoreTakeTask任务创建成功!\r\n");
		
    }
    else
    {
        printf("SemaphoreTakeTask任务创建失败!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)SemaphoreGiveTask,
                          (const char*)"SemaphoreGiveTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&Semaphore_Give_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("SemaphoreGiveTask任务创建成功!\r\n");
		
    }
    else
    {
        printf("SemaphoreGiveTask任务创建失败!\r\n");
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
static void SemaphoreTakeTask(void* pvParameters)
{
	BaseType_t xReturn = pdTRUE;        /* 创建信息返回值，默认为pdTRUE */

    while(1)
	{
        /** 从SemaphoreBinary_Handle中获取二值信号量, 等待时间为portMAX_DELAY */
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                xReturn = xSemaphoreTake(SemaphoreCounter_Handle, 0);                           /** 无等待方式发送获取信号量 */
                if(xReturn == pdTRUE){
                    printf("KEY1按下, 成功申请到一个信号量!\r\n");
                } else{
                    printf("KEY1按下, 无可用信号量!\r\n");
                }
            }
        }
        vTaskDelay(20);
	}
}


static void SemaphoreGiveTask(void* pvParameters)
{
   BaseType_t xReturn = pdTRUE;

    while(1)
    {
        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xSemaphoreGive(SemaphoreCounter_Handle);
                if(xReturn == pdPASS){
                    printf("KEY2按下, 成功释放一个信号量!\r\n");
                } else{
                    printf("KEY2按下, 无可释放信号量!\r\n");
                }
            }
        }
        vTaskDelay(20);
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
static void PrintLogo(void)
{
    printf("*********************勤进电子科技FreeRTOS移植****************\r\n");
	printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
		   __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

