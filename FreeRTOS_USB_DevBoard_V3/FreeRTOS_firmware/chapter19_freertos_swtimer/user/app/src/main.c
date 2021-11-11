/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"

#define USER_STACK_SIZE                                 128

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/* Soft timer of FreeRTOS */
/* 1. soft timer handle definition */
TimerHandle_t SoftTimer1_Handle = NULL;
TimerHandle_t SoftTimer2_Handle = NULL;

/* 2. two soft timers callback functions */
static uint32_t SoftTimer1_CB_Counter = 0;
static uint32_t SoftTimer2_CB_Counter = 0;

static void SoftTimer1_Callback(void *pvParameters);
static void SoftTimer2_Callback(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS Soft timer!\r\n");

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
    taskENTER_CRITICAL();

    SoftTimer1_Handle = xTimerCreate((const char*)"AutoReloadTimer",                    // Timer name
                                     (TickType_t)1000,                                  // timer period
                                     (UBaseType_t)pdTRUE,                               // period mode
                                     (void *)1,                                         // timer ID
                                     (TimerCallbackFunction_t)SoftTimer1_Callback);     // callback when timer overflow
    if(NULL != SoftTimer1_Handle){
        printf("\r\nSoftTimer1_Handle created successfully!\r\n\r\n");
        xTimerStart(SoftTimer1_Handle,                                                  // start soft timer
                    0);                                                                 // time out period
    }else
    {
        printf("\r\nSoftTimer1_Handle created failed!\r\n\r\n");
    }


    SoftTimer2_Handle = xTimerCreate((const char*)"OneShotTimer",
                                    (TickType_t)5000,
                                    (UBaseType_t)pdFALSE,
                                    (void*)2,
                                    (TimerCallbackFunction_t)SoftTimer2_Callback);
    if(NULL != SoftTimer2_Handle)
    {
        printf("\r\nSoftTimer2_Handle created successfully!\r\n\r\n");
        xTimerStart(SoftTimer2_Handle,                                                  // start soft timer
                    0);
    }
    else
    {
        printf("\r\nSoftTimer2_Handle created failed!\r\n\r\n");
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
static void SoftTimer1_Callback(void* pvParameters)
{
    TickType_t TickNum1 = 0;

    SoftTimer1_CB_Counter++;
    TickNum1 = xTaskGetTickCount();
    LED1_TOGGLE;
    printf("\r\nSoftTimer1_Callback had been run %d times!\r\n", SoftTimer1_CB_Counter);
    printf("System Tick counter = %d\r\n\r\n", TickNum1);
}


static void SoftTimer2_Callback(void* pvParameters)
{
    TickType_t TickNum2 = 0;

    SoftTimer2_CB_Counter++;
    TickNum2 = xTaskGetTickCount();
    LED2_TOGGLE;
    printf("\r\nSoftTimer2_Callback had been run %d times!\r\n", SoftTimer2_CB_Counter);
    printf("System Tick counter = %d\r\n\r\n", TickNum2);
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

