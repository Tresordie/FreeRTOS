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
// 1. ����һ����Ϣ���о��
SemaphoreHandle_t SemaphoreCounter_Handle = NULL;

// 2. ������Ϣ���еĳ��ȼ���С

// 3. ��Ϣ���е�������(������Ϣ���񼰷�����Ϣ����)
static TaskHandle_t Semaphore_Take_Task_Handle;
static TaskHandle_t Semaphore_Give_Task_Handle;

// 4. ��������, ����������
static void SemaphoreTakeTask(void *pvParameters);
static void SemaphoreGiveTask(void *pvParameters);

/**---------------------------------------------------------------------------*/

static void PrintLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;      /** ������Ϣ����ֵ */
    
    bsp_Init();
    PrintLogo();
    printf("FreeRTOS��ֵ�ź�����������ʵ��!\r\n");

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

    SemaphoreCounter_Handle = xSemaphoreCreateCounting(5, 5);                   // Init counter and MAX counter
    if(NULL != SemaphoreCounter_Handle){
        printf("SemaphoreCounter_Handle�����ź��������ɹ���\r\n");
    }

    xReturn = xTaskCreate((TaskFunction_t)SemaphoreTakeTask,
                          (const char*)"SemaphoreTakeTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&Semaphore_Take_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("SemaphoreTakeTask���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("SemaphoreTakeTask���񴴽�ʧ��!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)SemaphoreGiveTask,
                          (const char*)"SemaphoreGiveTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&Semaphore_Give_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("SemaphoreGiveTask���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("SemaphoreGiveTask���񴴽�ʧ��!\r\n");
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
static void SemaphoreTakeTask(void* pvParameters)
{
	BaseType_t xReturn = pdTRUE;        /* ������Ϣ����ֵ��Ĭ��ΪpdTRUE */

    while(1)
	{
        /** ��SemaphoreBinary_Handle�л�ȡ��ֵ�ź���, �ȴ�ʱ��ΪportMAX_DELAY */
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                xReturn = xSemaphoreTake(SemaphoreCounter_Handle, 0);                           /** �޵ȴ���ʽ���ͻ�ȡ�ź��� */
                if(xReturn == pdTRUE){
                    printf("KEY1����, �ɹ����뵽һ���ź���!\r\n");
                } else{
                    printf("KEY1����, �޿����ź���!\r\n");
                }
            }
        }
        vTaskDelay(20);
	}
}


static void SemaphoreGiveTask(void* pvParameters)
{
   BaseType_t xReturn = pdTRUE;

    while(1)
    {
        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xSemaphoreGive(SemaphoreCounter_Handle);
                if(xReturn == pdPASS){
                    printf("KEY2����, �ɹ��ͷ�һ���ź���!\r\n");
                } else{
                    printf("KEY2����, �޿��ͷ��ź���!\r\n");
                }
            }
        }
        vTaskDelay(20);
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

