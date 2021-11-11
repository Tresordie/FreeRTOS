/**
  * 1. 按键中断方式发送消息
  * 2. USART接收中断后释放信号量
  */

/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"
#include "limits.h"


#define USER_STACK_SIZE                                 512


static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/* 1. task handle definition */
static TaskHandle_t LED1Task_Handle;
static TaskHandle_t LED2Task_Handle;
static TaskHandle_t CPURunTime_Handle;

/* 2. task functions */
static void LED1Task(void *pvParameters);
static void LED2Task(void *pvParameters);
static void CPURunTime(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS CPU run time Demo!\r\n");

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

	// Different tasks created
	xReturn = xTaskCreate((TaskFunction_t)LED1Task,			// Function
						  (const char *)"LED1Task",			// Function name
						  (uint32_t)USER_STACK_SIZE,		// Stack size
						  (void *)NULL,						// Function parameter
						  (UBaseType_t)3,					// Priority
						  (TaskHandle_t *)&LED1Task_Handle); // Task handle pointer
	if(pdPASS == xReturn){
        printf("\r\nLED1Task created successfully!\r\n\r\n");
    }else
    {
        printf("\r\nLED1Task created failed!\r\n\r\n");
    }

	xReturn = xTaskCreate((TaskFunction_t)LED2Task,
						  (const char *)"LED2Task",
						  (uint32_t)USER_STACK_SIZE,
						  (void *)NULL,
						  (UBaseType_t)4,
						  (TaskHandle_t *)&LED2Task_Handle);
	if(pdPASS == xReturn)
    {
		printf("\r\nLED2Task created successfully!\r\n\r\n");
	}
    else
    {
		printf("\r\nLED2Task created failed!\r\n\r\n");
	}

	xReturn = xTaskCreate((TaskFunction_t)CPURunTime,
						  (const char *)"CPURunTime",
						  (uint32_t)USER_STACK_SIZE,
						  (void *)NULL,
						  (UBaseType_t)2,
						  (TaskHandle_t *)&CPURunTime_Handle);
	if (pdPASS == xReturn)
	{
		printf("\r\nCPURunTime task created successfully!\r\n\r\n");
	}
	else
	{
		printf("\r\nCPURunTime task created failed!\r\n\r\n");
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
static void CPURunTime(void *pvParameters)
{
   	uint8_t CPU_RunInfo[400];

    while(1)
    {
		memset(CPU_RunInfo, 0, 400);
		vTaskList((char *)&CPU_RunInfo[0]);
        printf("---------------------------------------------\r\n");
        printf("TaskName     Status   Prio   Remain  SeqNo\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n");

		memset(CPU_RunInfo, 0, 400);
		vTaskGetRunTimeStats((char *)&CPU_RunInfo[0]);
        printf("---------------------------------------------\r\n");    
        printf("TaskName       TaskRunTime      CPUUsage\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n\n");
		vTaskDelay(1000); /* 延时500个tick */
	}
}


static void LED1Task(void* pvParameters)
{
    while(1)
    {
		LED1_ON;
		vTaskDelay(500);
		printf("LED1 ON!\r\n");
		
		LED1_OFF;
		vTaskDelay(500);
		printf("LED1 OFF!\r\n");
    }
}


static void LED2Task(void *pvParameters)
{
	while (1)
	{
		LED2_ON;
		vTaskDelay(300);
		printf("LED2 ON!\r\n");

		LED2_OFF;
		vTaskDelay(300);
		printf("LED2 OFF!\r\n");
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

