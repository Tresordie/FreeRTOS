#ifndef __BSP_I2C_HW_H__
#define __BSP_I2C_HW_H__

#include "stm32f10x.h"

void bsp_I2C_GPIO_HW_Config(I2C_TypeDef* I2Cx);
void bsp_I2C_MODE_HW_Config(I2C_TypeDef* I2Cx, uint16_t I2Cx_OwnAddressX, uint32_t I2Cx_ClockSpeed);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);

#endif

