#ifndef _BSP_KEYBOARD_H
#define _BSP_KEYBOARD_H

#include "main.h"

#define GPIO_Keyboard_Port GPIOC
#define EXTI_Keyboard_PortSource EXTI_PortSourceGPIOC
#define RCC_Keyboard_ClockPeriph RCC_AHB1Periph_GPIOC
#define GPIO_IRQ_Pin GPIO_Pin_12
#define GPIO_DA_Pin  GPIO_Pin_11
#define GPIO_CK_Pin  GPIO_Pin_10
#define GPIO_LD_Pin  GPIO_Pin_9
#define EXTI_Keyboard_PinSource EXTI_PinSource12
#define EXTI_Keyboard_Line EXTI_Line12
#define NVIC_Keyboard_IRQChannel EXTI15_10_IRQn   //注意还需修改中断处理函数

void Keyboard_Init(void);

#endif
