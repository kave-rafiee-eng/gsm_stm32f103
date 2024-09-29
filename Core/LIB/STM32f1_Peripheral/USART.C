#include "main.h"
#include "USART.H"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
	
void UART1_INI(void)
{
	
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

	LL_USART_EnableIT_RXNE(USART1);

	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
  NVIC_EnableIRQ(USART1_IRQn);
	
}

void UART2_INI(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
	
	LL_USART_EnableIT_RXNE(USART2);

	NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
  NVIC_EnableIRQ(USART2_IRQn);
	
}

void UART3_INI(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
	
	LL_USART_EnableIT_RXNE(USART3);

	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),1, 1));
  NVIC_EnableIRQ(USART3_IRQn);
	
}

int size=0;

char UART_PRINT(char *str , char uart_select ){

	size = strlen(str);
	int i=0;
	for(i=0;i<size;i++){
		
			if( uart_select == 3 ){
				LL_USART_TransmitData8(USART3, str[i]);		 
				while(!LL_USART_IsActiveFlag_TXE(USART3));
			}
			
			if( uart_select == 2 ){
				LL_USART_TransmitData8(USART2, str[i]);		 
				while(!LL_USART_IsActiveFlag_TXE(USART2));
			}
			
			if( uart_select == 1 ){
				
				LL_USART_TransmitData8(USART1, str[i]);		 
				while(!LL_USART_IsActiveFlag_TXE(USART1));		
			}		
	}
	return 1;
	
}


char UART_PUT_CHAR(char data , char uart_select ){
	
		if( uart_select == 3 ){
			LL_USART_TransmitData8(USART3, data);		 
			while(!LL_USART_IsActiveFlag_TXE(USART3));
		}
			
		if( uart_select == 2 ){
			LL_USART_TransmitData8(USART2, data);		 
			while(!LL_USART_IsActiveFlag_TXE(USART2));
		}
			
		if( uart_select == 1 ){
				
			LL_USART_TransmitData8(USART1, data);		 
			while(!LL_USART_IsActiveFlag_TXE(USART1));		
		}		

}