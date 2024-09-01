
#include "main.h"
#include "stm32f1xx_it.h"
#include "FreeRTOS.h"
#include "task.h"

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


void USART1_IRQHandler(void)
{

  HAL_UART_IRQHandler(&huart1);

}


void USART2_IRQHandler(void)
{

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
