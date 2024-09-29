#ifndef __GPIO_H
#define __GPIO_H

#include "main.h"

#define SW_EN_GPIO GPIOB
#define SW_EN_PIN GPIO_PIN_3

#define	LED_ESP(x)     ( HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,x) ) //B3
#define	LED_SIM(x)     ( HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,x) ) //B4
#define	LED_ADVANCE(x)     ( HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,x) ) //B5

#define	LED_ESP_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_3) ) //B3
#define	LED_SIM_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4) ) //B4
#define	LED_ADVANCE_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5) ) //B5

#define	SIM_ON(x)     ( HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,x) ) //A7
#define	ESP_ON(x)     ( HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,x) ) //B13

void GPIO_INI(void);
char read_gpio(GPIO_TypeDef *GPIOx , unsigned int pin);

#endif


