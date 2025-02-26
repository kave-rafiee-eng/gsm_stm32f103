#ifndef __GPIO_H
#define __GPIO_H

#include "main.h"

#define SW_EN_GPIO GPIOB
#define SW_EN_PIN GPIO_PIN_2

#define	LED_ESP(x)     ( HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,x) ) //C13
#define	LED_SIM(x)     ( HAL_GPIO_WritePin(GPIOC,GPIO_PIN_14,x) ) //C14
#define	LED_ADVANCE(x)     ( HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,x) ) //C15

#define	LED_ESP_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13) ) //C13
#define	LED_SIM_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_14) ) //C14
#define	LED_ADVANCE_TOGGLE()     ( HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_15) ) //C15

#define	SIM_ON(x)     ( HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,x) ) //A1
#define	ESP_ON(x)     ( HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,x) ) //B15

#define	SIM_PWR(x)     ( HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,x) ) //A0

void GPIO_INI(void);
char read_gpio(GPIO_TypeDef *GPIOx , unsigned int pin);

#endif


