#ifndef __BSP_SPI_TFTLCD__
#define __BSP_SPI_TFTLCD__

#include "stm32f10x.h"

void SPI1_Init(void);			                //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet);                //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);               //SPI���߶�дһ���ֽ�
u8 SPI1_WriteData(u8 *data,uint32_t size);      //SPIд����

#endif   /* __BSP_SPI_TFTLCD__ */

