
#include "main.h"
#include "stm32f1xx_it.h"
#include "FreeRTOS.h"
#include "task.h"


extern struct 	UART_DATA esp_data;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;



void SysTick_Handler(void)
{
	
	timer_basic_manager();
	
  HAL_IncTick();
	
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */

}


void DMA1_Channel2_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart3_tx);

}

void DMA1_Channel3_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart3_rx);

}


void DMA1_Channel4_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart1_tx);

}


void DMA1_Channel5_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart1_rx);

}


void DMA1_Channel6_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart2_rx);

}


void DMA1_Channel7_IRQHandler(void)
{

  HAL_DMA_IRQHandler(&hdma_usart2_tx);

}

char u_data=0;
char u2_data=0;

void USART1_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
	{		
		u_data=LL_USART_ReceiveData8(USART1);
		USART2->DR=u_data;
	}
  HAL_UART_IRQHandler(&huart1);
}

volatile char start_get=0;

void USART2_IRQHandler(void) // esp
{
	if(LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2))
	{		
		u2_data = LL_USART_ReceiveData8(USART2);
		if( u2_data == '{' )start_get=1;

		if( start_get == 1 ){
			
			esp_data.BUF[esp_data.BUF_I] = u2_data;
			esp_data.BUF_I++;
			
			if( u2_data == '}' )start_get=2;
			
		}
		
		if( esp_data.BUF_I >= UART_BUF_SIZE )esp_data.BUF_I=0;

	}
  HAL_UART_IRQHandler(&huart2);

}

void USART3_IRQHandler(void)
{

  HAL_UART_IRQHandler(&huart3);

}


void NMI_Handler(void)
{

   while (1)
  {
  }

}


void HardFault_Handler(void)
{

  while (1)
  {

  }
}


void MemManage_Handler(void)
{

  while (1)
  {

  }
}


void BusFault_Handler(void)
{

  while (1)
  {

  }
}

void UsageFault_Handler(void)
{

  while (1)
  {

  }
}


void DebugMon_Handler(void){

}
