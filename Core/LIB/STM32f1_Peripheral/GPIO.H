#ifndef __GPIO_H
#define __GPIO_H

#include "main.h"

#define SW_EN_GPIO GPIOB
#define SW_EN_PIN GPIO_PIN_3



void GPIO_INI(void);
char read_gpio(GPIO_TypeDef *GPIOx , unsigned int pin);

#endif


