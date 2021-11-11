/* FreeRTOS head files */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

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
static void QueueReceiveTask(void *pvParameters);
static void QueueSendTask(void *pvParameters);

//���Ĳ������񴴽��ɹ�����������
static TaskHandle_t AppTaskCreate_Handle;

/**---------------------------------------------------------------------------*/
/**
  *		��Ϣ����
  */
// 1. ����һ����Ϣ���о��
QueueHandle_t Test_Queue = NULL;

// 2. ������Ϣ���еĳ��ȼ���С
#define     QUEUE_LEN       4           /* ��������Ϣ�ĸ��� */
#define     QUEUE_SIZE      4          /* �����е�����Ϣ�Ĵ�С */

// 3. ��Ϣ���е�������(������Ϣ���񼰷�����Ϣ����)
static TaskHandle_t Queue_Receive_Task_Handle;
static TaskHandle_t Queue_Send_Task_Handle;
/**---------------------------------------------------------------------------*/

static void PrintfLogo(void);

int main(void)
{
    BaseType_t xReturn = pdPASS;      /** ������Ϣ����ֵ */
    
    bsp_Init();
    PrintfLogo();
    printf("FreeRTOS��̬�ڴ洴������ʵ��!\r\n");

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

    Test_Queue = xQueueCreate((UBaseType_t)QUEUE_LEN,
                              (UBaseType_t)QUEUE_SIZE);
    if(NULL != Test_Queue){
        printf("����Test_Queue��Ϣ���гɹ�!\r\n");
    }else{
        printf("����Test_Queue��Ϣ����ʧ��!\r\n");
    }


    /* LED task create */
    xReturn = xTaskCreate((TaskFunction_t)QueueReceiveTask,
                          (const char*)"QueueReceiveTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&Queue_Receive_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("QueueReceiveTask���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("QueueReceiveTask���񴴽�ʧ��!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)QueueSendTask,
                          (const char*)"QueueSendTask",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&Queue_Send_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("QueueSendTask���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("QueueSendTask���񴴽�ʧ��!\r\n");
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
static void QueueReceiveTask(void* pvParameters)
{
	BaseType_t xReturn = pdTRUE;        /* ������Ϣ����ֵ��Ĭ��ΪpdTRUE */
    uint32_t queue_receive_buffer;      /* ������Ϣ���ջ��� */

    while(1)
	{
        /** ��Test_Queue�����н���һ����Ϣ��queue_receive_buffer, �ȴ�ʱ��ΪportMAX_DELAY */
        xReturn = xQueueReceive(Test_Queue, &queue_receive_buffer, portMAX_DELAY);

        if(pdTRUE == xReturn){
            printf("���յ�������Ϊ��%c\r\n", queue_receive_buffer);
        }else{
            printf("���ݽ��ճ����������Ϊ��%d\r\n", xReturn);
        }
	}
}


static void QueueSendTask(void* pvParameters)
{
   BaseType_t xReturn = pdTRUE;
   uint32_t send_data1 = 'A';
   uint32_t send_data2 = 'B';

    while(1)
    {
        if(KeyScan(KEY1_PORT, KEY1_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY1_PORT, KEY1_PIN)){
                xReturn = xQueueSend(Test_Queue, &send_data1, 0);       /** �޵ȴ���ʽ����send_data1��Test_Queue */
                if(xReturn == pdPASS){
                    printf("��Ϣsend_data1���ͳɹ�!\r\n");
                }
            }
        }

        if(KeyScan(KEY2_PORT, KEY2_PIN)){
            vTaskDelay(100);
            if(KeyScan(KEY2_PORT, KEY2_PIN)){
                xReturn = xQueueSend(Test_Queue, &send_data2, 0);
                if(xReturn == pdPASS){
                    printf("��Ϣsend_data2���ͳɹ�!\r\n");
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
static void PrintfLogo(void)
{
    printf("*********************�ڽ����ӿƼ�FreeRTOS��ֲ****************\r\n");
	printf("************************FreeRTOS��Ϣ����********************\r\n");
	printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
		   __STM32F10X_STDPERIPH_VERSION_SUB1, __STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

