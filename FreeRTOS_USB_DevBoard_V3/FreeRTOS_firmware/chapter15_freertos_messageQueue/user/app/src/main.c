/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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
static void QueueReceiveTask(void *pvParameters);
static void QueueSendTask(void *pvParameters);

//第四步：任务创建成功与否的任务句柄
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/**
  *		消息队列
  */
// 1. 定义一个消息队列句柄
QueueHandle_t Test_Queue = NULL;

// 2. 定义消息队列的长度及大小
#define     QUEUE_LEN       4           /* 队列中消息的个数 */
#define     QUEUE_SIZE      4          /* 队列中单个消息的大小 */

// 3. 消息队列的任务函数(接收消息任务及发送消息任务)
static TaskHandle_t Queue_Receive_Task_Handle;
static TaskHandle_t Queue_Send_Task_Handle;
/**---------------------------------------------------------------------------*/

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

    Test_Queue = xQueueCreate((UBaseType_t)QUEUE_LEN,
                              (UBaseType_t)QUEUE_SIZE);
    if(NULL != Test_Queue){
        printf("创建Test_Queue消息队列成功!\r\n");
    }else{
        printf("创建Test_Queue消息队列失败!\r\n");
    }


    /* LED task create */
    xReturn = xTaskCreate((TaskFunction_t)QueueReceiveTask,
                          (const char*)"QueueReceiveTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&Queue_Receive_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("QueueReceiveTask任务创建成功!\r\n");
		
    }
    else
    {
        printf("QueueReceiveTask任务创建失败!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)QueueSendTask,
                          (const char*)"QueueSendTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&Queue_Send_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("QueueSendTask任务创建成功!\r\n");
		
    }
    else
    {
        printf("QueueSendTask任务创建失败!\r\n");
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
static void QueueReceiveTask(void* pvParameters)
{
	BaseType_t xReturn = pdTRUE;        /* 创建信息返回值，默认为pdTRUE */
    uint32_t queue_receive_buffer;      /* 队列消息接收缓冲 */

    while(1)
	{
        /** 从Test_Queue队列中接收一个消息到queue_receive_buffer, 等待时间为portMAX_DELAY */
        xReturn = xQueueReceive(Test_Queue, &queue_receive_buffer, portMAX_DELAY);

        if(pdTRUE == xReturn){
            printf("接收到的数据为：%c\r\n", queue_receive_buffer);
        }else{
            printf("数据接收出错，错误代码为：%d\r\n", xReturn);
        }
	}
}


static void QueueSendTask(void* pvParameters)
{
   BaseType_t xReturn = pdTRUE;
   uint32_t send_data1 = 'A';
   uint32_t send_data2 = 'B';

    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                xReturn = xQueueSend(Test_Queue, &send_data1, 0);       /** 无等待方式发送send_data1到Test_Queue */
                if(xReturn == pdPASS){
                    printf("消息send_data1发送成功!\r\n");
                }
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xQueueSend(Test_Queue, &send_data2, 0);
                if(xReturn == pdPASS){
                    printf("消息send_data2发送成功!\r\n");
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
static void PrintfLogo(void)
{
    printf("*********************勤进电子科技FreeRTOS移植****************\r\n");
	printf("************************FreeRTOS消息队列********************\r\n");
	printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
		   __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

