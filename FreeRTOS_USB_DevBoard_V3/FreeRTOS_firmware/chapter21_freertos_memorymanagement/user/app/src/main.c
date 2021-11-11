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

uint8_t *ptr_mem = NULL;

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/* Task Notify of FreeRTOS */
/* 1. task handle definition */
static TaskHandle_t LEDTask_Handle;
static TaskHandle_t TestTask_Handle;

/* 2. task functions */
static void LEDTask(void *pvParameters);
static void TestTask(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS MemoryManagement!\r\n");

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

    xReturn = xTaskCreate((TaskFunction_t)LEDTask,                               // Function
                          (const char*)"LEDTask",                                // Function name
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


    xReturn = xTaskCreate((TaskFunction_t)TestTask,
                          (const char*)"TestTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void *)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&TestTask_Handle);
    if(pdPASS == xReturn)
    {
        printf("\r\nTest Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nTest Task created failed!\r\n\r\n");
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
static void TestTask(void* pvParameters)
{
    uint32_t h_memsize = 0;

    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)) {
                if(NULL == ptr_mem) {
                    h_memsize = xPortGetFreeHeapSize();
                    printf("Current HeapSize in system is %d bytes,Start to malloc!\r\n", h_memsize);
                    ptr_mem = pvPortMalloc(1024);
                    if(NULL != ptr_mem) {
                        printf("Malloc successfully!\r\n");
                        printf("Memory address applied start from %#x\r\n", (int)ptr_mem);      // %#x, 16进制格式输出，输出带0x

                        h_memsize = xPortGetFreeHeapSize();
                        printf("Current HeapSize in system is %d bytes!\r\n", h_memsize);
                        sprintf((char *)ptr_mem, "Current system TickCount = %d\r\n", xTaskGetTickCount());
                        printf("Data had been written is %s\r\n", (char *)ptr_mem);
                    } else {
                        printf("Apply after released with press key2!\r\n");
                    }
                }
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)) {
                if(NULL == ptr_mem) {
                        printf("Please press key1 to release firstly!\r\n");
                    } else {
                        printf("Memory released!\r\n");
                        vPortFree(ptr_mem);
                        ptr_mem = NULL;

                        h_memsize = xPortGetFreeHeapSize();
                        printf("Current HeapSize in system is %d bytes!\r\n", h_memsize);
                    }
                }
            }
    }
}


static void LEDTask(void* pvParameters)
{
    while(1)
    {
        LED1_TOGGLE;
        vTaskDelay(1000);
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

