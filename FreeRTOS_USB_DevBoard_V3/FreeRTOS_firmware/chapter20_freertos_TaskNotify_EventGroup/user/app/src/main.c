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
#define KEY1_EVENT                                      (0x01 << 0)
#define KEY2_EVENT                                      (0x01 << 1)

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;


/**---------------------------------------------------------------------------*/
/* Task Notify of FreeRTOS */
/* 1. task handle definition */
static TaskHandle_t LEDTask_Handle;
static TaskHandle_t KEYTask_Handle;

/* 2. task functions */
static void LEDTask(void *pvParameters);
static void KEYTask(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS TaskNotification to replace EventGroup!\r\n");

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

    xReturn = xTaskCreate((TaskFunction_t)LEDTask,                              // Function
                          (const char*)"LEDTask",                               // Function name
                          (uint32_t)USER_STACK_SIZE,                             // Stack size
                          (void *)NULL,                                          // Function parameter
                          (UBaseType_t)2,                                        // Priority
                          (TaskHandle_t*)&LEDTask_Handle);                      // Task handle pointer
    if(pdPASS == xReturn){
        printf("\r\nLEDTask created successfully!\r\n\r\n");
    }else
    {
        printf("\r\nLEDTask created failed!\r\n\r\n");
    }


    xReturn = xTaskCreate((TaskFunction_t)KEYTask,
                          (const char*)"KEYTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&KEYTask_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nKEYTask created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nKEYTask created failed!\r\n\r\n");
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
static void LEDTask(void* pvParameters)
{
    uint32_t last_notify = 0;
    uint32_t current_notify = 0;
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        xReturn = xTaskNotifyWait(0, ULONG_MAX, &current_notify, portMAX_DELAY);

        if(pdTRUE == xReturn){
            last_notify |= current_notify;
            if((KEY1_EVENT | KEY2_EVENT) == last_notify){
                printf("Both Key1 and Key2 been pressed!\r\n");
                last_notify = 0;
                LED1_TOGGLE;
            }else{
                last_notify = current_notify;
            }
        }

        vTaskDelay(20);
    }
}


static void KEYTask(void* pvParameters)
{
    BaseType_t xReturn = pdPASS;
    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                // use xTaskNotifyGive to replace EventGroup
                xReturn = xTaskNotify(LEDTask_Handle, (uint32_t)KEY1_EVENT, (eNotifyAction)eSetBits);
                if(pdPASS == xReturn){
                    printf("\r\nKey1 pressed, eSetBits with KEY1_EVENT!\r\n\r\n");
                }else{
                    printf("\r\nKey1 pressed, xTaskNotify error!\r\n\r\n");
                }
            }
        }


        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xTaskNotify(LEDTask_Handle, (uint32_t)KEY2_EVENT, (eNotifyAction)eSetBits);
                if(pdPASS == xReturn){
                    printf("\r\nKey2 pressed, eSetBits with KEY2_EVENT!\r\n\r\n");
                }else{
                    printf("\r\nKey2 pressed, xTaskNotify error!\r\n\r\n");
                }
            }
        }
        vTaskDelay(20);
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

