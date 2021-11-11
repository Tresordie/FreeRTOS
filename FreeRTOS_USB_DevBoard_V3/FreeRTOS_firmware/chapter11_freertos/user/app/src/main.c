#include "bsp.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"


/*****************************************************************
  * @brief  ���񴴽����������Ҫ��(��������庯���������ջ��������ƿ�)
  * @param  ��
  * @retval ��
  * @note   ��һ��������ջ����
            �ڶ�����������ƿ�
            ����������������庯��
            ���Ĳ������񴴽��ɹ�����������
            ���岽����ʱ�����񼰿������������ջ����Ӧ��������ƿ�
  ****************************************************************/

// ��һ��������ջ���� 
#define USER_STACK_SIZE                 128
static StackType_t AppTaskCreate_Stack[USER_STACK_SIZE];
static StackType_t LED_Task_Stack[USER_STACK_SIZE];

//�ڶ�����������ƿ�
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;

//����������������庯��
static void AppTaskCreate(void);
static void LED_Task(void *pvParameters);

//���Ĳ������񴴽��ɹ�����������
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

//���岽�����񴴽��ɹ�����������
static StackType_t Idle_Task_Stack[USER_STACK_SIZE];
static StackType_t Timer_Task_Stack[USER_STACK_SIZE];

static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;


/**
  * ʹ���˾�̬�����ڴ棬�������������������û�ʵ�֣�������task.c�ļ���������
  *   ���ҽ��� configSUPPORT_STATIC_ALLOCATION ����궨��Ϊ 1 ��ʱ�����Ч
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
       
	/** ����AppTaskCreate���񣬴����ɹ���AppTaskCreate_Handleָ��������ƿ�AppTaskCreate_TCB */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,               // ������
                                             (const char*)"AppTaskCreate",                // ��������
                                             (uint32_t)USER_STACK_SIZE,                   // ����ջ��С
                                             (void *)NULL,                                // �������Ĵ���
                                             (UBaseType_t)3,                              // ��������ȼ�
                                             (StackType_t*)AppTaskCreate_Stack,           // ����ջָ��
                                             (StaticTask_t*)&AppTaskCreate_TCB);          // ������ƿ�
    if(NULL != AppTaskCreate_Handle)
    {
        vTaskStartScheduler();
    }

    while (1);
}




/*****************************************************************
  * @brief  AppTaskCreate
  * @param  Ϊ�˷���������е����񴴽����������������������
  * @retval ��
  * @note   ��
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
        printf("LED_Task���񴴽��ɹ�!\r\n");
    }
    else
    {
        printf("LED_Task���񴴽�ʧ��!\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle);

    taskEXIT_CRITICAL();
}


/*****************************************************************
  * @brief  LED_Task
  * @param  LED_Task��������
  * @retval ��
  * @note   ��
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
  * @brief  ��ȡ��������������ջ��������ƿ��ڴ�
    @param      *ppxIdleTaskTCBBuffer   : ������ƿ��ڴ�
                *ppxIdleTaskStackBuffer : �����ջ�ڴ�
                *pulIdleTaskStackSize   : �����ջ��С
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
  * @brief  ȡ��ʱ������������ջ��������ƿ��ڴ�
    @param      *ppxTimerTaskTCBBuffer   : ������ƿ��ڴ�
                *ppxTimerTaskStackBuffer : �����ջ�ڴ�
                *pulTimerTaskStackSize   : �����ջ��С
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
*   �� �� ��: PrintfLogo
*   ����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    printf("*********************�ڽ����ӿƼ�FreeRTOS��ֲ****************\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

