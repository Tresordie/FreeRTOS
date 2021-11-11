#include "bsp.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_dma.h"

char usart_rx_buff[USART_RX_BUFF_SIZE];

static SD_Error Status = SD_OK;
static SD_CardInfo SDCardInfo;

static void bsp_led_gpioinit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;   
    RCC_APB2PeriphClockCmd(LEDGP1_RCC|LEDGP2_RCC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED1_PIN|LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDGP1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LED3_PIN|LED4_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDGP2_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(LEDGP1_PORT, LED1_PIN|LED2_PIN);
    GPIO_SetBits(LEDGP2_PORT, LED3_PIN|LED4_PIN);
}


static void bsp_InitUart(void)
{
    GPIO_InitTypeDef 	GPIO_InitStructure;  
    USART_InitTypeDef 	USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_Cmd(USART1, ENABLE);
    USART_ClearITPendingBit(USART1, USART_IT_IDLE|USART_IT_RXNE|USART_IT_TC|USART_IT_TXE);
}


static void USARTx_DMA_Config(void)
{

	DMA_InitTypeDef DMA_InitStructure;

	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)USART_DR_ADDRESS;

	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)usart_rx_buff;

	// ���򣺴��ڴ浽����
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;

	// �����С
	DMA_InitStructure.DMA_BufferSize 			= USART_RX_BUFF_SIZE;

	// �����ַ����
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;

	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;

	// �������ݵ�λ
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;

	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;

	// DMAģʽ��һ�λ���ѭ��ģʽ
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;

	// ���ȼ�����
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;

	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;

	// ����DMAͨ��
	DMA_Init(USART_RX_DMA_CHANNEL, &DMA_InitStructure);

	// ���DMA���б�־
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_ITConfig(USART_RX_DMA_CHANNEL, DMA_IT_TE, ENABLE);

	// ʹ��DMA
	DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);
}


static void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	// �򿪴��������ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate 				= 115200;

	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;

	// ����ֹͣλ
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;

	// ����У��λ
	USART_InitStructure.USART_Parity 				= USART_Parity_No;

	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	// ��ɴ��ڵĳ�ʼ������
	USART_Init(USART1, &USART_InitStructure);

	// �����ж����ȼ�����
	NVIC_Configuration();

	// ���� ���ڿ���IDEL �ж�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	// ��������DMA����
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
}


static void bsp_InitDMAUart(void){
	USARTx_DMA_Config();
	USART_Config();
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;

	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return (ch);
}


/*
static void bsp_InitI2C1(void)
{   
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
    
    // PB6 - I2C1_SCLK, PB7 - I2C1_SDA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 =0x68; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;   
    I2C_InitStructure.I2C_ClockSpeed = 100000;

    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);  
}

static void bsp_InitSPIFlash(void)
{
    SPI_FLASH_Init();
}
*/


void bsp_InitSDcard(void)
{
    /* SD card 4bit SDIO test */
    Status = SD_Init();
    if(Status == SD_OK) 
    {    
        printf("\r\n SD_Init ��ʼ���ɹ� \r\n");
    }
    else
    {
        printf("\r\n SD_Init ��ʼ��ʧ�� \r\n" );
        printf("\r\n ���ص�Status��ֵΪ�� %d \r\n",Status);
    }

    printf( " \r\n CardType is ��%d ", SDCardInfo.CardType);
    printf( " \r\n CardCapacity is ��%d ", SDCardInfo.CardCapacity);
    printf( " \r\n CardBlockSize is ��%d ", SDCardInfo.CardBlockSize);
    printf( " \r\n RCA is ��%d ", SDCardInfo.RCA);
    printf( " \r\n ManufacturerID is ��%d \r\n", SDCardInfo.SD_cid.ManufacturerID);
}


static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;

	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;

	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period = (1000-1);

	// ʱ��Ԥ��Ƶ��Ϊ
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// ʱ�ӷ�Ƶ���� ��������ʱ��û�У����ù�
	//TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

	// ����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
	//TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;

	// �ظ���������ֵ��������ʱ��û�У����ù�
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// ����������жϱ�־λ
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

	// �����������ж�
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// ʹ�ܼ�����
	TIM_Cmd(TIM6, ENABLE);
}


static void bsp_InitTIM(void)
{
	TIM_NVIC_Config();
	TIM_Mode_Config();
}

/*
*********************************************************************************************************
*   �� �� ��: bsp_Init
*   ����˵��: ��ʼ��Ӳ���豸��ֻ��Ҫ����һ�Ρ��ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����
*            ȫ�ֱ�����
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	bsp_led_gpioinit();      			/* ����LED��GPIO�˿� */
	bsp_InitUart();		 				/* ��ʼ���������� */
	bsp_InitTIM();
}




