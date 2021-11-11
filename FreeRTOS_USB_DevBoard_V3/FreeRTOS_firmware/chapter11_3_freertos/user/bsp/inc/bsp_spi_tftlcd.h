#ifndef __BSP_SPI_TFTLCD__
#define __BSP_SPI_TFTLCD__

#include "stm32f10x.h"

void SPI1_Init(void);			                //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet);                //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);               //SPI总线读写一个字节
u8 SPI1_WriteData(u8 *data,uint32_t size);      //SPI写数据

#endif   /* __BSP_SPI_TFTLCD__ */

