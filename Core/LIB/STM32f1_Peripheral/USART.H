#ifndef __USART_H
#define __USART_H

#include "main.h"
	
#define UART_SIM 3
#define UART_ESP 2
#define UART_RS485 1

#define UART_BUF_SIZE 200

#define	MODBUS_ADVANCE_RS(x)     ( HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,x) ) //A8


struct 	UART_DATA
{
		volatile unsigned char BUF[UART_BUF_SIZE];
		volatile unsigned char BUF_I ;
};
	
void UART2_INI(void);
void UART1_INI(void);
void UART3_INI(void);
char UART_PRINT(char *str , char uart_select );
char UART_PUT_CHAR(char data , char uart_select );
	
#endif