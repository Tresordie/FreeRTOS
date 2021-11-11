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

	// 开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// 设置DMA源地址：串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)USART_DR_ADDRESS;

	// 内存地址(要传输的变量的指针)
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (uint32_t)usart_rx_buff;

	// 方向：从内存到外设
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralSRC;

	// 传输大小
	DMA_InitStructure.DMA_BufferSize 			= USART_RX_BUFF_SIZE;

	// 外设地址不增
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;

	// 内存地址自增
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;

	// 外设数据单位
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;

	// 内存数据单位
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;

	// DMA模式，一次或者循环模式
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;

	// 优先级：中
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;

	// 禁止内存到内存的传输
	DMA_InitStructure.DMA_M2M 					= DMA_M2M_Disable;

	// 配置DMA通道
	DMA_Init(USART_RX_DMA_CHANNEL, &DMA_InitStructure);

	// 清除DMA所有标志
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_ITConfig(USART_RX_DMA_CHANNEL, DMA_IT_TE, ENABLE);

	// 使能DMA
	DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);
}


static void USART_Config(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	// 打开串口外设的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate 				= 115200;

	// 配置 针数据字长
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;

	// 配置停止位
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;

	// 配置校验位
	USART_InitStructure.USART_Parity 				= USART_Parity_No;

	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;

	// 完成串口的初始化配置
	USART_Init(USART1, &USART_InitStructure);

	// 串口中断优先级配置
	NVIC_Configuration();

	// 开启 串口空闲IDEL 中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	// 开启串口DMA接收
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	// 使能串口
	USART_Cmd(USART1, ENABLE);
}


static void bsp_InitDMAUart(void){
	USARTx_DMA_Config();
	USART_Config();
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;

	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* 初始化配置NVIC */
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
        printf("\r\n SD_Init 初始化成功 \r\n");
    }
    else
    {
        printf("\r\n SD_Init 初始化失败 \r\n" );
        printf("\r\n 返回的Status的值为： %d \r\n",Status);
    }

    printf( " \r\n CardType is ：%d ", SDCardInfo.CardType);
    printf( " \r\n CardCapacity is ：%d ", SDCardInfo.CardCapacity);
    printf( " \r\n CardBlockSize is ：%d ", SDCardInfo.CardBlockSize);
    printf( " \r\n RCA is ：%d ", SDCardInfo.RCA);
    printf( " \r\n ManufacturerID is ：%d \r\n", SDCardInfo.SD_cid.ManufacturerID);
}


static void TIM_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* 配置USART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;

	/* 抢断优先级*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;

	/* 子优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	// 开启定时器时钟,即内部时钟CK_INT=72M
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period = (1000-1);

	// 时钟预分频数为
	TIM_TimeBaseStructure.TIM_Prescaler = 71;

	// 时钟分频因子 ，基本定时器没有，不用管
	//TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;

	// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
	//TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;

	// 重复计数器的值，基本定时器没有，不用管
	//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;

	// 初始化定时器
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	// 清除计数器中断标志位
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);

	// 开启计数器中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

	// 使能计数器
	TIM_Cmd(TIM6, ENABLE);
}


static void bsp_InitTIM(void)
{
	TIM_NVIC_Config();
	TIM_Mode_Config();
}

/*
*********************************************************************************************************
*   函 数 名: bsp_Init
*   功能说明: 初始化硬件设备。只需要调用一次。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。
*            全局变量。
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{   
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	bsp_led_gpioinit();      			/* 配置LED的GPIO端口 */
	bsp_InitUart();		 				/* 初始化串口驱动 */
	bsp_InitTIM();
}




