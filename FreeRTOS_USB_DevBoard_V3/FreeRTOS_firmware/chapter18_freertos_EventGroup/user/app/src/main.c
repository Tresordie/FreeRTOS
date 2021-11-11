/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"


#define USER_STACK_SIZE                                 128
#define KEY1_EVENT                                      (0x01 << 0)
#define KEY2_EVENT                                      (0x01 << 1)

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/* EventGroup of FreeRTOS */
/* 1. event group handle definition */
EventGroupHandle_t EventGroup_Handle = NULL;

/* 2. two tasks(handles) been defined: one is waiting some event group bits. another is set some event group bits */
static TaskHandle_t LED_Task_Handle;                    // LED task handle
static TaskHandle_t KEY_Task_Handle;                    // KEY task handle

static void LED_Task(void *pvParameters);               // LED task to wait event group bit
static void KEY_Task(void *pvParameters);               // KEY task to detect key pressed and then set event group bit



/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();

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


static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    EventGroup_Handle = xEventGroupCreate();                   // Create Event Group
    if(NULL != EventGroup_Handle){
        printf("\r\nEventGroup_Handle created successfully!\r\n\r\n");
    }

    xReturn = xTaskCreate((TaskFunction_t)LED_Task,
                          (const char*)"LED_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&LED_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nLED_Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nLED_Task created failed!\r\n\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)KEY_Task,
                          (const char*)"KEY_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&KEY_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nKEY_Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nKEY_Task created failed!\r\n\r\n");
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
static void LED_Task(void* pvParameters)
{
    // xEventGroupWaitBits return data
    EventBits_t EventBits;

    while(1)
    {
        EventBits = xEventGroupWaitBits(EventGroup_Handle,               // which event group to wait
                                        KEY1_EVENT | KEY2_EVENT,         // which event bits to wait
                                        pdTRUE,                          // xClearOnExit - clear event bits when exit
                                        pdTRUE,                          // xWaitForAllBits - if pdTRUE, wait all event bit been flagged
                                        portMAX_DELAY);                  // timeout delay setting

         printf("%s|%d|EventBits = %d\r\n", __FILE__, __LINE__, EventBits);

         /* LED_Task got required event group flags */
        if((KEY1_EVENT | KEY2_EVENT) == (EventBits & (KEY1_EVENT | KEY2_EVENT))){
            printf("Both Key1 and Key2 been pressed !\r\n\r\n");
        } else{
            printf("Event waited error!\r\n");
        }
    }
}


static void KEY_Task(void* pvParameters)
{
    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN))
            {
                printf("Key1 had been pressed!\r\n");
                xEventGroupSetBits(EventGroup_Handle, KEY1_EVENT);            // which event group and which bits to set
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN))
            {
                printf("Key2 had been pressed!\r\n");
                xEventGroupSetBits(EventGroup_Handle, KEY2_EVENT);            // which event group and which bits to set
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

