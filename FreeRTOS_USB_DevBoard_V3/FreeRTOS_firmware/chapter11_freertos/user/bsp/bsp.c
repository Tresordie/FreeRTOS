#include "bsp.h"

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
    GPIO_InitTypeDef GPIO_InitStructure;  
    USART_InitTypeDef USART_InitStructure;
    
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


int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    return (ch);
}


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
    bsp_led_gpioinit();      /* ����LED��GPIO�˿� */
    bsp_InitUart();          /* ��ʼ���������� */
}




