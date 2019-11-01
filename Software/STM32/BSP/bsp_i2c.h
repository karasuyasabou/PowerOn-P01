#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "main.h"

#define I2CTimeout 0xFFFF
#define I2C_SCL_H()      GPIO_SetBits(GPIOB, GPIO_I2C_SCL_Pin)
#define I2C_SCL_L()      GPIO_ResetBits(GPIOB, GPIO_I2C_SCL_Pin)
#define I2C_SDA_H()      GPIO_SetBits(GPIOB, GPIO_I2C_SDA_Pin)
#define I2C_SDA_L()      GPIO_ResetBits(GPIOB, GPIO_I2C_SDA_Pin)
#define I2C_SDA_Read()   GPIO_ReadInputDataBit(GPIOB, GPIO_I2C_SDA_Pin)
#define I2C_SCL_Read()   GPIO_ReadInputDataBit(GPIOB, GPIO_I2C_SCL_Pin)


uint8_t I2C_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Data);
uint8_t I2C_ReadData(uint8_t Addr, uint8_t Reg, uint8_t *pdata, uint8_t count);

#endif
