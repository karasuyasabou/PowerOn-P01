#include "bsp_keyboard.h"

void Keyboard_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_Keyboard_ClockPeriph,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = GPIO_IRQ_Pin | GPIO_DA_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO_Keyboard_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_CK_Pin | GPIO_LD_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIO_Keyboard_Port, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIO_Keyboard_Port, GPIO_CK_Pin);
	
	SYSCFG_EXTILineConfig(EXTI_Keyboard_PortSource, EXTI_Keyboard_PinSource);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Keyboard_Line;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = NVIC_Keyboard_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

uint16_t Keyboard_ReadData(void)  //输出被按下的按键号
{
	uint8_t i,data;
	GPIO_SetBits(GPIO_Keyboard_Port, GPIO_LD_Pin);
	delay_us(1);
	GPIO_ResetBits(GPIO_Keyboard_Port, GPIO_LD_Pin);
	for(i=0;i<16;i++)
	{
		delay_us(1);
		if(GPIO_ReadInputDataBit(GPIO_Keyboard_Port, GPIO_DA_Pin))
			data = 16 - i;
		GPIO_SetBits(GPIO_Keyboard_Port, GPIO_CK_Pin);
		delay_us(1);
		GPIO_ResetBits(GPIO_Keyboard_Port, GPIO_CK_Pin);
	}
	return data;
}	

void EXTI15_10_IRQHandler() //任意按键触发中断
{
	uint8_t keyNum = 0; 
	if(EXTI_GetITStatus(EXTI_Keyboard_Line) != RESET)
	{
		LED0 = ~LED0;
		keyNum = Keyboard_ReadData();
		//在此输入中断处理函数
		EXTI_ClearITPendingBit(EXTI_Keyboard_Line);
	}
}
