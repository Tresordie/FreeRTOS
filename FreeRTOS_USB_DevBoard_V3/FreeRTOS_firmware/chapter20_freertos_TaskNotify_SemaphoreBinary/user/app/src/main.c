/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"
#include "limits.h"

#define USER_STACK_SIZE                                 128

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;


/**---------------------------------------------------------------------------*/
/* Task Notify of FreeRTOS */
/* 1. task handle definition */
static TaskHandle_t SendTask_Handle;
static TaskHandle_t ReceiveTask1_Handle;
static TaskHandle_t ReceiveTask2_Handle;

/* 2. task functions */
static void SendTask(void *pvParameters);
static void ReceiveTask1(void *pvParameters);
static void ReceiveTask2(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS TaskNotification to replace MessageQueue!\r\n");

    /* Task which creates tasks defined */
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,
                          (const char*)"AppTaskCreate",
                          (uint32_t)USER_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)1,
                          (TaskHandle_t*)&AppTaskCreate_Handle);
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


/* Function to create app task */
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;
    taskENTER_CRITICAL();

    xReturn = xTaskCreate((TaskFunction_t)SendTask,                              // Function
                          (const char*)"SendTask",                               // Function name
                          (uint32_t)USER_STACK_SIZE,                             // Stack size
                          (void *)NULL,                                          // Function parameter
                          (UBaseType_t)2,                                        // Priority
                          (TaskHandle_t*)&SendTask_Handle);                      // Task handle pointer
    if(pdPASS == xReturn){
        printf("\r\nSendTask created successfully!\r\n\r\n");
    }else
    {
        printf("\r\nSendTask created failed!\r\n\r\n");
    }


    xReturn = xTaskCreate((TaskFunction_t)ReceiveTask1,
                          (const char*)"ReceiveTask1",
                          (uint32_t)USER_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&ReceiveTask1_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nReceive1Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nReceive1Task created failed!\r\n\r\n");
    }


    xReturn = xTaskCreate((TaskFunction_t)ReceiveTask2,
                          (const char*)"ReceiveTask2",
                          (uint32_t)USER_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t*)&ReceiveTask2_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nReceive2Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nReceive2Task created failed!\r\n\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle);
    
    taskEXIT_CRITICAL();
}


/*****************************************************************
  * @brief  Task
  * @param
  * @retval
  * @note
  ****************************************************************/
static void SendTask(void* pvParameters)
{
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                // use xTaskNotifyGive to replace SemaphoreBinary
                xReturn = xTaskNotifyGive(ReceiveTask1_Handle);
                if(pdPASS == xReturn){
                    printf("\r\nKey1 pressed, Send TaskNotification to ReceiveTask1 successfully!\r\n\r\n");
                }
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xTaskNotifyGive(ReceiveTask2_Handle);
                if(pdPASS == xReturn){
                    printf("\r\nKey2 pressed, Send TaskNotification to ReceiveTask2 successfully!\r\n\r\n");
                }
            }
        }
        vTaskDelay(20);
    }
}


static void ReceiveTask1(void* pvParameters)
{
    while(1)
    {
        // uint32_t ulTaskNotifyTake(BaseType_t xClearCountOnExit, TickType_t xTicksToWait)
        // xClearCountOnExit    设置为pdFALSE时，函数xTaskNotifyTake()退出前，将任务的通知值减 1，可以用来实现计数信号量；
        // 设置为pdTRUE时，函数xTaskNotifyTake()退出前，将任务通知值清零，可以用来实现二值信号量
        // xTicksToWait         超时时间，单位为系统节拍周期。宏pdMS_TO_TICKS用于将毫秒转化为系统节拍数
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Receive Task 1 successfully get Task Notify!\r\n");
        LED1_TOGGLE;
    }
}


static void ReceiveTask2(void* pvParameters)
{
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("Receive Task 2 successfully get Task Notify!\r\n");
        LED2_TOGGLE;
    }
}


static void PrintLogo(void)
{
    printf("********************FreeRTOS EventGroup*********************\r\n");
	printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
		   __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

