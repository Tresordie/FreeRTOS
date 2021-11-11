/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* bsp head files */
#include "bsp.h"
#include "main.h"

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

//�ڶ�����������ƿ�
//ʹ�ö�̬�⣬��ʱosʹ��malloc�Զ�����ռ��������ƿ�

//����������������庯��
static void AppTaskCreate(void);

//���Ĳ������񴴽��ɹ�����������
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/**
  *		��Ϣ����
  */
// 1. ����һ����Ϣ���о��,���������ź���ʱʹ��
SemaphoreHandle_t MutexSemaphore_Handle = NULL;

// 2. ������Ϣ���еĳ��ȼ���С

// 3. ��Ϣ���е�������(������Ϣ���񼰷�����Ϣ����)
static TaskHandle_t LowPriorityTask_Handle;
static TaskHandle_t MidPriorityTask_Handle;
static TaskHandle_t HighPriorityTask_Handle;

// 4. ��������, ����������
static void LowPriority_Task(void *pvParameters);
static void MidPriority_Task(void *pvParameters);
static void HighPriority_Task(void *pvParameters);


/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;      /** ������Ϣ����ֵ */
    
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS�����ź�����������ʵ��!\r\n");

    // ����AppTaskCreate����, ��̬�ڴ������ʹ�õ���xTaskCreateStatic
    xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,               // ������
                          (const char*)"AppTaskCreate",                // ��������
                          (uint32_t)USER_STACK_SIZE,                   // ����ջ��С
                          (void *)NULL,                                // �������Ĵ���
                          (UBaseType_t)1,                              // ��������ȼ�
                          (TaskHandle_t*)&AppTaskCreate_Handle);       // ������ƿ�
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

/*****************************************************************
  * @brief  AppTaskCreate
  * @param  Ϊ�˷���������е����񴴽����������������������
  * @retval ��
  * @note   ��
  ****************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;

    taskENTER_CRITICAL();

    MutexSemaphore_Handle = xSemaphoreCreateMutex();                          // Mutex semaphore seems like semaphore binary
    if(NULL != MutexSemaphore_Handle){
        printf("MutexSemaphore_Handle�����ź��������ɹ���\r\n");
    }

    xReturn = xTaskCreate((TaskFunction_t)LowPriority_Task,
                          (const char*)"LowPriority_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&LowPriorityTask_Handle);
    if(pdPASS == xReturn)
    {
        printf("LowPriority_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("LowPriority_Task���񴴽�ʧ��!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)MidPriority_Task,
                          (const char*)"MidPriority_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&MidPriorityTask_Handle);
    if(pdPASS == xReturn)
    {
        printf("MidPriority_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("MidPriority_Task���񴴽�ʧ��!\r\n");
    }


    xReturn = xTaskCreate((TaskFunction_t)HighPriority_Task,
                          (const char*)"HighPriority_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t*)&HighPriorityTask_Handle);
    if(pdPASS == xReturn)
    {
        printf("HighPriority_Task���񴴽��ɹ�!\r\n");

    }
    else
    {
        printf("HighPriority_Task���񴴽�ʧ��!\r\n");
    }

    vTaskDelete(AppTaskCreate_Handle);
    
    taskEXIT_CRITICAL();
}


/*****************************************************************
  * @brief  Task
  * @param  Task��������
  * @retval ��
  * @note   ��
  ****************************************************************/
static void LowPriority_Task(void* pvParameters)
{
    static uint32_t i = 0;
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        printf("LowPriority_Task��ȡ������!\r\n\r\n");

        xReturn = xSemaphoreTake(MutexSemaphore_Handle, portMAX_DELAY);     /** ����ʱ��������ʽ��ȡ�����ź��� */
        if(pdTRUE == xReturn){
            printf("LowPriority_Task running!\r\n\r\n");
        }

        for(i = 0; i < 2000000; i++){
            taskYIELD();
        }

        printf("LowPriority_Task�ͷŻ�����!\r\n\r\n");
        xReturn = xSemaphoreGive(MutexSemaphore_Handle);        // �ͷŻ�����
        LED1_TOGGLE;
        vTaskDelay(1000);
    }
}


static void MidPriority_Task(void* pvParameters)
{
    while(1)
    {
        printf("MidPriority_Task running!\r\n\r\n");
        vTaskDelay(1000);
    }
}


static void HighPriority_Task(void* pvParameters)
{
    BaseType_t xReturn = pdTRUE;

    while(1)
    {
        printf("HighPriority_Task��ȡ������!\r\n\r\n");

        xReturn = xSemaphoreTake(MutexSemaphore_Handle, portMAX_DELAY);     /** ����ʱ��������ʽ��ȡ�����ź��� */
        if(pdTRUE == xReturn){
            printf("HighPriority_Task running!\r\n\r\n");
        }
        LED1_TOGGLE;

        printf("HighPriority_Task�ͷŻ�����!\r\n\r\n");
        xReturn = xSemaphoreGive(MutexSemaphore_Handle);        // �ͷŻ�����

        vTaskDelay(1000);
    }
}







/*
*********************************************************************************************************
*   �� �� ��: PrintfLogo
*   ����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintLogo(void)
{
    printf("*********************�ڽ����ӿƼ�FreeRTOS��ֲ****************\r\n");
	printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
		   __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

