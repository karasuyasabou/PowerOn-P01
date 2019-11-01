#include "bsp_i2c.h"

void I2C_Delay()
{
	uint16_t i;
	for (i = 0; i < 6; i++);
}

void I2C_SDA_In()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_SDA_Out()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	
  GPIO_InitStructure.GPIO_Pin = GPIO_I2C_SDA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Start()
{
	I2C_SDA_Out();
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_Delay();
	I2C_SDA_L();
	I2C_Delay();
	I2C_SCL_L();
}

void I2C_Stop()
{
	I2C_SDA_Out();
	I2C_SCL_L();
	I2C_SDA_L();
	I2C_Delay();
	I2C_SCL_H();
	I2C_SDA_H();
	I2C_Delay();
}

uint8_t I2C_WaitAck(void)
{
	uint16_t TimeOut = 5000;
	I2C_SDA_H();
	I2C_SDA_In();
	I2C_Delay();
	I2C_SCL_H();
	I2C_Delay();
	while(I2C_SDA_Read())
	{
		TimeOut--;
		if(TimeOut == 0)
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_SCL_L();
	return 0;
}

void I2C_Ack(void)
{
	I2C_SDA_Out();
	I2C_SDA_H();
	I2C_SCL_H();
	I2C_Delay();
	I2C_SCL_L();
	I2C_Delay();
}

void I2C_NAck(void)
{
	I2C_SDA_Out();
	I2C_SDA_L();
	I2C_SCL_H();
	I2C_Delay();
	I2C_SCL_L();
	I2C_Delay();
}

void I2C_WriteBit(uint8_t Data)
{
	uint8_t i;
	I2C_SDA_Out();
	I2C_SCL_L();
	for (i = 0; i < 8; i++)
	{
		if (Data & 0x80)
		{
			I2C_SDA_H();
		}
		else
		{
			I2C_SDA_L();
		}
		Data<<=1;
		I2C_Delay();
		I2C_SCL_H();
		I2C_Delay();
		I2C_SCL_L();
	}
}

uint8_t I2C_ReadBit(void)
{
	uint8_t i;
	uint8_t Data = 0;
	I2C_SDA_In();
	for (i = 0; i < 8; i++)
	{
		I2C_SCL_L();
		I2C_Delay();
		I2C_SCL_H();
		Data <<= 1;
		if (I2C_SDA_Read())
		{
			Data++;
		}
		I2C_Delay();
	}
	I2C_SCL_L();
	return Data;
}

uint8_t I2C_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Data)
{
	I2C_Start();
	
	I2C_WriteBit(Addr << 1);
	if(I2C_WaitAck())
		return 1;
	
	I2C_WriteBit(Reg);
	if(I2C_WaitAck())
		return 1;
	
	I2C_WriteBit(Data);
	if(I2C_WaitAck())
		return 1;
	
	I2C_Stop();
	return 0;
}

uint8_t I2C_ReadData(uint8_t Addr, uint8_t Reg, uint8_t *pdata, uint8_t count)
{
	uint8_t i;
	I2C_Start();
	
	I2C_WriteBit(Addr << 1);
	if(I2C_WaitAck())
		return 1;
	
	I2C_WriteBit(Reg);
	if(I2C_WaitAck())
		return 1;
	
	I2C_Start();
	I2C_WriteBit((Addr << 1) + 1);
	if(I2C_WaitAck())
		return 1;
	
  for(i=0;i<(count-1);i++)
  {
    *pdata=I2C_ReadBit();
     I2C_NAck();
     pdata++;
  }
  *pdata=I2C_ReadBit();
   I2C_Ack(); 
   I2C_Stop(); 
   return 0;    
}
