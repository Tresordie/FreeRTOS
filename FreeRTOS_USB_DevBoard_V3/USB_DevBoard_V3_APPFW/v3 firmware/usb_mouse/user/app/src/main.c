#include "bsp.h"
#include "main.h"
#include "usb.h"

#define     FLASH_WriteAddress     0x00000
#define     FLASH_ReadAddress      FLASH_WriteAddress
#define     FLASH_SectorToErase    FLASH_WriteAddress
#define     sFLASH_ID              0xEF4016	 //W25Q32
//#define  sFLASH_ID              0XEF4017    //W25Q128

#define     countof(a)             (sizeof(a) / sizeof(*(a)))
#define     BufferSize             (countof(Tx_Buffer)-1)

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/***
 * SD card
 */
#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      16  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)   //缓冲区大小	 

uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t readbuff[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];


/***
 * SPI Flash
 */
uint8_t Tx_Buffer[] = "Hello, friend!";
uint8_t Rx_Buffer[BufferSize];

__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;


/***
 * RTC
 */
rtc_ds1307_datetime_t rtc_datetime;


/***
 * Function declare
 */
static void PrintfLogo(void);
SD_Error Status = SD_OK;
extern SD_CardInfo SDCardInfo;

void SD_EraseTest(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);


int main(void)
{
    uint16_t D12_ID = 0;
    uint8_t InterruptSource = 0;

    bsp_Init();
    PrintfLogo();
    
    D12GPIOConfiguration();
    Descriptors_Init();
    UsbDisconnect();
    
    printf("\r\n\r\n<<<<<<<<<<<<<<<<<<<<<PDIUSBD12芯片ID读取>>>>>>>>>>>>>>>>>>>\r\n");
    D12_ID = D12ReadID();
    printf("Your PDIUSBD12 Chip ID is: 0x%x\r\n", D12_ID);

    if(0x1012 == D12_ID)
    {
        printf("ID is correct! Congratulations!\r\n");
    }
    else
    {
        printf("ID is incorrect! What a pity!\r\n");
    }
    printf("\r\n\r\n<<<<<<<<<<<<<<<<<<<<<PDIUSBD12芯片ID读取完成>>>>>>>>>>>>>>>>>>>\r\n");
    
    
    UsbConnect();
  
    while (1)
    {    
        if(0 == D12GetINTPin())
        {
            InterruptSource = D12_ReadInterrupt_Register();

            if(InterruptSource & 0x80)
            {
                UsbBusSuspend();
            }

            if(InterruptSource & 0x40)
            {
                UsbBusReset();
            }

            if(InterruptSource & 0x01)
            {
                UsbEp0Out();
            }

            if(InterruptSource & 0x02)
            {
                UsbEp0In();
            }

            if(InterruptSource & 0x04)
            {
                UsbEp1Out();
            }

            if(InterruptSource & 0x08)
            {
                UsbEp1In();
            }

            if(InterruptSource & 0x10)
            {
                UsbEp2Out();
            }

            if(InterruptSource & 0x20)
            {
                UsbEp2In();
            }
        }
    }
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
    printf("*************************勤进电子科技************************\r\n\r\n");
    printf("                STDLib    Version : %d.%d.%d\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN, \
            __STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
    printf("                CMSIS     Version : %X.%02X\r\n", __CM3_CMSIS_VERSION_MAIN, __CM3_CMSIS_VERSION_SUB);
    printf("                Firmware  Version : %X.%02X.%02X\r\n", FW_VERSION, FW_SUB1_VERSION, FW_SUB2_VERSION);
    printf("*************************************************************\r\n");
}



TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

/*
 * 函数名：SD_EraseTest
 * 描述  ：擦除数据测试
 * 输入  ：无
 * 输出  ：无
 */
void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {    
		/* 第一个参数为擦除起始地址，第二个参数为擦除结束地址 */
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
  }

  if (Status == SD_OK)
  {	/* 读取刚刚擦除的区域 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);

    /* 查询传输是否结束 */
    Status = SD_WaitReadOperation(); 
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
 
  if (Status == SD_OK)
  {	/* 把擦除区域读出来对比 */
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(EraseStatus == PASSED)
  	printf("\r\n 擦除测试成功！ " );
 
  else	  
  	printf("\r\n 擦除测试失败！ " );  
}

/*
 * 函数名：SD_SingleBlockTest
 * 描述  ：	单个数据块读写测试
 * 输入  ：无
 * 输出  ：无
 */
void SD_SingleBlockTest(void)
{  
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);//读取数据
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	//比较
  }
  
  if(TransferStatus1 == PASSED)
    printf("\r\n 单块读写测试成功！" );
 
  else  
  	printf("\r\n 单块读写测试失败！ " );  
}

/*
 * 函数名：SD_MultiBlockTest
 * 描述  ：	多数据块读写测试
 * 输入  ：无
 * 输出  ：无
 */
void SD_MultiBlockTest(void)
{
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)	  
  	printf("\r\n 多块读写测试成功！ " );

  else 
  	printf("\r\n 多块读写测试失败！ " );  
}


/*
 * 函数名：Fill_Buffer
 * 描述  ：在缓冲区中填写数据
 * 输入  ：-pBuffer 要填充的缓冲区
 *         -BufferLength 要填充的大小
 *         -Offset 填在缓冲区的第一个值
 * 输出  ：无 
 */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/*
 * 函数名：eBuffercmp
 * 描述  ：检查缓冲区的数据是否为0
 * 输入  ：-pBuffer 要比较的缓冲区
 *         -BufferLength 缓冲区长度        
 * 输出  ：PASSED 缓冲区的数据全为0
 *         FAILED 缓冲区的数据至少有一个不为0 
 */
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))//擦除后是0xff或0x00
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

