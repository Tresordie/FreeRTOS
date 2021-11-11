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
#define USER_STACK_SIZE                 512

//�ڶ�����������ƿ�


//����������������庯��
static void AppTaskCreate(void);
static void LED1_Task(void *pvParameters);
static void LED2_Task(void *pvParameters);
static void LED3_Task(void *pvParameters);
static void LED4_Task(void *pvParameters);

//���Ĳ������񴴽��ɹ�����������
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED1_Task_Handle;
static TaskHandle_t LED2_Task_Handle;
static TaskHandle_t LED3_Task_Handle;
static TaskHandle_t LED4_Task_Handle;

//���岽�����񴴽��ɹ�����������


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
                          (TaskHandle_t*)&AppTaskCreate_Handle);          // ������ƿ�
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

    /* LED task create */
    xReturn = xTaskCreate((TaskFunction_t)LED1_Task,
                          (const char*)"LED1_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)5,
                          (TaskHandle_t*)&LED1_Task_Handle);    
    if(pdPASS == xReturn)
    {
        printf("LED1_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("LED1_Task���񴴽�ʧ��!\r\n");
    }
    

    xReturn = xTaskCreate((TaskFunction_t)LED2_Task,
                          (const char*)"LED2_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)4,
                          (TaskHandle_t*)&LED2_Task_Handle);   
    if(pdPASS == xReturn)
    {
        printf("LED2_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("LED2_Task���񴴽�ʧ��!\r\n");
    }
    
    
    xReturn = xTaskCreate((TaskFunction_t)LED3_Task,
                          (const char*)"LED3_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)3,
                          (TaskHandle_t*)&LED3_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("LED3_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("LED3_Task���񴴽�ʧ��!\r\n");
    }
    
        
    xReturn = xTaskCreate((TaskFunction_t)LED4_Task,
                          (const char*)"LED4_Task",
                          (uint32_t)USER_STACK_SIZE,
                          (void*)NULL,
                          (UBaseType_t)2,
                          (TaskHandle_t*)&LED4_Task_Handle);
    if(pdPASS == xReturn)
    {
        printf("LED4_Task���񴴽��ɹ�!\r\n");
		
    }
    else
    {
        printf("LED4_Task���񴴽�ʧ��!\r\n");
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
static void LED1_Task(void* parameter)
{
    while(1)
    {
        LED1_ON;
        vTaskDelay(200);
        printf("LED1_Task running, LED1_ON\r\n");

        LED1_OFF;
        vTaskDelay(200);
        printf("LED1_Task running, LED1_OFF\r\n");
    }
}


static void LED2_Task(void* parameter)
{
    while(1)
    {
        LED2_ON;
        vTaskDelay(500);
        printf("LED2_Task running, LED2_ON\r\n");

        LED2_OFF;
        vTaskDelay(500);
        printf("LED2_Task running, LED2_OFF\r\n");
    }
}


static void LED3_Task(void* parameter)
{
    while(1)
    {
        LED3_ON;
        vTaskDelay(300);
        printf("LED3_Task running, LED3_ON\r\n");

        LED3_OFF;
        vTaskDelay(300);
        printf("LED3_Task running, LED3_OFF\r\n");
    }

}


static void LED4_Task(void* parameter)
{
    while(1)
    {
        LED4_ON;
        vTaskDelay(600);
        printf("LED4_Task running, LED4_ON\r\n");

        LED4_OFF;
        vTaskDelay(600);
        printf("LED4_Task running, LED4_OFF\r\n");
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
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}

