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
#define	QUEUE_LEN										4				/* Queue length */
#define	QUEUE_SIZE										4				/* Queue size */

/* 1.定义用于消息队列句柄 */ 
QueueHandle_t       TestQueue_Handle;
SemaphoreHandle_t   SemaphoreBinary_Handle;

static void AppTaskCreate(void);
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/* Task Notify of FreeRTOS */
extern char usart_rx_buff[USART_RX_BUFF_SIZE];

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
    printf("FreeRTOS Interrupt Demo!\r\n");

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

	// 1. TestQueue created in AppTaskCreate(will be used later in tasks to sync) 
	TestQueue_Handle = xQueueCreate((UBaseType_t)QUEUE_LEN, (UBaseType_t)QUEUE_SIZE);
	if(NULL != TestQueue_Handle){
		printf("TestQueue created successfully!\r\n");
	}

	// 2. Semaphore created in AppTaskCreate(will be used later in tasks to sync)
	SemaphoreBinary_Handle = xSemaphoreCreateBinary();
	if(NULL != SemaphoreBinary_Handle){
		printf("SemaphoreBinary created successfully!\r\n");
	}

	// 3. Different tasks created
	xReturn = xTaskCreate((TaskFunction_t)LEDTask,                               // Function
                          (const char*)"LEDTask",                                // Function name
                          (uint32_t)USER_STACK_SIZE,                             // Stack size
                          (void *)NULL,                                          // Function parameter
                          (UBaseType_t)2,                                        // Priority
                          (TaskHandle_t*)&LEDTask_Handle);                       // Task handle pointer
    if(pdPASS == xReturn){
        printf("\r\nLEDTask created successfully!\r\n\r\n");
    }else
    {
        printf("\r\nLEDTask created failed!\r\n\r\n");
    }

	xReturn = xTaskCreate((TaskFunction_t)KEYTask,
						  (const char *)"KEYTask",
						  (uint32_t)USER_STACK_SIZE,
						  (void *)NULL,
						  (UBaseType_t)3,
						  (TaskHandle_t *)&KEYTask_Handle);
	if(pdPASS == xReturn)
    {
        printf("\r\nKEY Task created successfully!\r\n\r\n");
    }
    else
    {
        printf("\r\nKEY Task created failed!\r\n\r\n");
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
static void KEYTask(void* pvParameters)
{
   	BaseType_t xReturn = pdPASS;

    while(1)
    {
       xReturn = xSemaphoreTake(SemaphoreBinary_Handle, portMAX_DELAY);

	   if(pdPASS == xReturn){
		   printf("Received data: %s\r\n", usart_rx_buff);
		   memset(usart_rx_buff, 0, USART_RX_BUFF_SIZE);
		   LED2_TOGGLE;
	   }
    }
}


static void LEDTask(void* pvParameters)
{
	BaseType_t xReturn = pdPASS;
	uint32_t recQueue = 0;

    while(1)
    {
		xReturn = xQueueReceive(TestQueue_Handle, &recQueue, portMAX_DELAY);
		
		if(pdPASS == xReturn){
			printf("KEY%d triggered interrupt!\r\n", recQueue);
		}else{
			printf("Queue message error!\r\n");
		}

		LED1_TOGGLE;
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

