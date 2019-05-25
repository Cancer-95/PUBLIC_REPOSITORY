#ifndef __LED_H
#define __LED_H

#include "system.h"

#define Blue_On  	 GPIO_ResetBits(GPIOC,GPIO_Pin_13)
#define Blue_Off  	 GPIO_SetBits(GPIOC,GPIO_Pin_13)

#define Orange_On			 GPIO_ResetBits(GPIOC,GPIO_Pin_14)
#define Orange_Off	     GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define Visual_On		 GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define Visual_Off	 GPIO_ResetBits(GPIOA,GPIO_Pin_8)

void Led_Init(void);
#endif

