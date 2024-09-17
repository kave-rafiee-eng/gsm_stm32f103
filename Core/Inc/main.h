
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif



#include "stm32f1xx_hal.h"

#define USE_FULL_LL_DRIVER 1

/* Includes ------------------------------------------------------------------*/ //CMSIS
#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_usart.h"

/* Includes ------------------------------------------------------------------*/ //STM32f1_Peripheral
#include "../LIB/STM32f1_Peripheral/GPIO.h"
#include "../LIB/STM32f1_Peripheral/Clock.h"
#include "../LIB/STM32f1_Peripheral/USART.h"
#include "../LIB/STM32f1_Peripheral/CAN.h"
#include "../LIB/STM32f1_Peripheral/TIMER.h"
#include "../LIB/STM32f1_Peripheral/DMA.h"

/* Includes ------------------------------------------------------------------*/ //GSM
#include "../LIB/GSM/Initialization.H"
#include "../LIB/GSM/test.H"
#include "../LIB/GSM/ESP8266.H"
#include "../LIB/GSM/modbus_gsm.H"
#include "../LIB/GSM/advance.h"

/* Includes ------------------------------------------------------------------*/ //JSON
#include "../LIB/GSM/JSON/json.H"

/* Includes ------------------------------------------------------------------*/ //MODBUS
#include "../LIB/MODBUS/CRC.h"
#include "../LIB/MODBUS/modbus.h"

/* Includes ------------------------------------------------------------------*/ //CPU_TIMER
#include "../LIB/SOFT_WARE_TIMER/CPU_TIMER.H"


/* Includes ------------------------------------------------------------------*/ //STANSARD LIB
#include <stdint.h>
#include <stdio.h>    

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
