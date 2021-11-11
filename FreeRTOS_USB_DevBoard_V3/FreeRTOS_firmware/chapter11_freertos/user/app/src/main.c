#include "bsp.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


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
static StackType_t AppTaskCreate_Stack[USER_STACK_SIZE];
static StackType_t LED_Task_Stack[USER_STACK_SIZE];

//第二步：任务控制块
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;

//第三步：任务的主体函数
static void AppTaskCreate(void);
static void LED_Task(void *pvParameters);

//第四步：任务创建成功与否的任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

//第五步：任务创建成功与否的任务句柄
static StackType_t Idle_Task_Stack[USER_STACK_SIZE];
static StackType_t Timer_Task_Stack[USER_STACK_SIZE];

static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;


/**
  * 使用了静态分配内存，以下这两个函数是由用户实现，函数在task.c文件中有引用
  *   当且仅当 configSUPPORT_STATIC_ALLOCATION 这个宏定义为 1 的时候才有效
  */
void vApplicationGetIdleTaskMemory(StaticTask_t * * ppxIdleTaskTCBBuffer, 
                                                StackType_t * * ppxIdleTaskStackBuffer, 
                                                uint32_t * pulIdleTaskStackSize);

void vApplicationGetTimerTaskMemory(StaticTask_t * * ppxTimerTaskTCBBuffer, 
                                                  StackType_t * * ppxTimerTaskStackBuffer, 
                                                  uint32_t * pulTimerTaskStackSize);

static void PrintfLogo(void);

int main(void)
{
    bsp_Init();
    PrintfLogo();
       
	/** 创建AppTaskCreate任务，创建成功后，AppTaskCreate_Handle指向任务控制块AppTaskCreate_TCB */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,               // 任务函数
                                             (const char*)"AppTaskCreate",                // 任务名称
                                             (uint32_t)USER_STACK_SIZE,                   // 任务栈大小
                                             (void *)NULL,                                // 任务函数的传参
                                             (UBaseType_t)3,                              // 任务的优先级
                                             (StackType_t*)AppTaskCreate_Stack,           // 任务栈指针
                                             (StaticTask_t*)&AppTaskCreate_TCB);          // 任务控制块
    if(NULL != AppTaskCreate_Handle)
    {
        vTaskStartScheduler();
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
    taskENTER_CRITICAL();

    /* LED task create */
    LED_Task_Handle = xTaskCreateStatic((TaskFunction_t)LED_Task,
                                        (const char*)"LED_Task",
                                        (uint32_t)USER_STACK_SIZE,
                                        (void*)NULL,
                                        (UBaseType_t)4,
                                        (StackType_t*)LED_Task_Stack,
                                        (StaticTask_t*)&LED_Task_TCB);

    if(NULL != LED_Task_Handle)
    {
        printf("LED_Task任务创建成功!\r\n");
    }
    else
    {
        printf("LED_Task任务创建失败!\r\n");
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
static void LED_Task(void* parameter)
{
    while(1)
    {
        LED1_ON;
        vTaskDelay(200);
        printf("LED_Task running, LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(200);
        printf("LED_Task running, LED1_OFF\r\n");
    }
}


/**
  **********************************************************************
  * @brief  获取空闲任务的任务堆栈和任务控制块内存
    @param      *ppxIdleTaskTCBBuffer   : 任务控制块内存
                *ppxIdleTaskStackBuffer : 任务堆栈内存
                *pulIdleTaskStackSize   : 任务堆栈大小
  **********************************************************************
  */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
                                                StackType_t **ppxIdleTaskStackBuffer, 
                                                uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB;
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


/**
  **********************************************************************
  * @brief  取定时器任务的任务堆栈和任务控制块内存
    @param      *ppxTimerTaskTCBBuffer   : 任务控制块内存
                *ppxTimerTaskStackBuffer : 任务堆栈内存
                *pulTimerTaskStackSize   : 任务堆栈大小
  **********************************************************************
  */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                                 StackType_t **ppxTimerTaskStackBuffer,
                                                 uint32_t * pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB;
    *ppxTimerTaskStackBuffer = Timer_Task_Stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
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
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

