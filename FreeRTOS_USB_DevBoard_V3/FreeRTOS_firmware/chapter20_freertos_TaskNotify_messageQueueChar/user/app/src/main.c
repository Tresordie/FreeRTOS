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
    char send1[] = "This is string 1";
    char send2[] = "This is string 2";
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
      /**
        * @brief xTaskNotify(TaskHandle_t xTaskToNotify, uint32_t ulValue, eNotifyAction eAction)
        * @param eNoAction = 0 -- Notify task but NOT update ulValue
        *        eSetBits      -- Set bits of ulValue
        *        eIncrement    -- Increase ulValue
        *        eSetValueWithOverwrite -- overwrite current ulValue
        *        eSetValueWithoutOverwrite -- NO overwrite current ulValue
        * @return pdFAIL -- when eAction been set as eSetValueWithoutOverWrite, if task notification happened but failed to update ulValue
        *         pdPASS -- when other situation, pdPASS will be returned.
        * */
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                // xTaskNotify send some task a ulValue NOT some pointer
                // 这里需要注意：
                // 1. send1表示的是数组的首元素的地址即数组的首地址，&send1表示的整个数组的地址
                // 2. 如果xTaskNotify发送的通知值为send1, 那表示发送的是首元素地址；
                //    如果xTaskNotify发送的通知值为&send1, 那表示发送的是整个数组的数组地址
                xReturn = xTaskNotify(ReceiveTask1_Handle, (uint32_t)send1, eSetValueWithOverwrite);
                if(pdPASS == xReturn){
                    printf("\r\nKey1 pressed, Send TaskNotification to ReceiveTask1 successfully!\r\n\r\n");
                }
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xTaskNotify(ReceiveTask2_Handle, (uint32_t)&send2, eSetValueWithOverwrite);
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
    char *ReceiveChar;
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        /**
          * @brief xTaskNotifyWait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pilNotificationValue, TickType_t xTicksToWait)
          * @param ulBitsToClearOnEntry -- If clear any bits of task notification when enter function
          * @param ulBitsToClearOnExit  -- If clear any bits of task notification when exit function
          * @param *pulNotificationValue - save task notification value
          * @param xTicksToWait         -- time to block
          * */

        // xTaskNotifyWait wait some pointer point to ulValue NOT an ulValue
        xReturn = xTaskNotifyWait(0x00, ULONG_MAX, (uint32_t *)&ReceiveChar, portMAX_DELAY);
        if(pdTRUE == xReturn){
            printf("%s\r\n", ReceiveChar);
        }
        LED1_TOGGLE;
    }
}


static void ReceiveTask2(void* pvParameters)
{
    char *ReceiveChar;
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        /**
          * @brief xTaskNotifyWait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pilNotificationValue, TickType_t xTicksToWait)
          * @param ulBitsToClearOnEntry -- If clear any bits of task notification when enter function
          * @param ulBitsToClearOnExit  -- If clear any bits of task notification when exit function
          * @param *pulNotificationValue - save task notification value
          * @param xTicksToWait         -- time to block
          * */

        // use ReceiverChar to save string address from SendTask
        xReturn = xTaskNotifyWait(0x00, ULONG_MAX, (uint32_t *)&ReceiveChar, portMAX_DELAY);
        if(pdTRUE == xReturn){
            printf("%s\r\n", ReceiveChar);
        }
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

