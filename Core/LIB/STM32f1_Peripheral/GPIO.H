#ifndef __GPIO_H
#define __GPIO_H

#include "main.h"


void GPIO_INI(void);
char read_gpio(GPIO_TypeDef *GPIOx , unsigned int pin);

#endif


