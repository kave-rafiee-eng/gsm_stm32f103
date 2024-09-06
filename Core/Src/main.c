
#include "main.h"
#include "cmsis_os.h"

int i=0;
void Vtask1( void *pvParameters ){ 
	
	 for( ;; ){
		 
		 HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
		 osDelay(100);
		 HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
		 i++;
		 
		/* USART2->DR='a';
		 USART1->DR='b';
		 
		 char str[10]="{asd}";
		 puts(str);*/
		 
		 
	 }
	
}

void Vtask2( void *pvParameters ){ 
	
		for( ;; ){
		 
		 osDelay(10);
			
	 }
	
}

void StartDefaultTask(void *argument);


int main(void)
{


  HAL_Init();

	Hardware_init();
	Software_init();
	
	/*while(1){
		 
		USART1->DR='A';
		HAL_Delay(50);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
  }*/
	
	xTaskCreate(Vtask1,"task1",100,NULL,1,NULL);
	xTaskCreate(Vtask2,"task2",100,NULL,1,NULL);
	
	vTaskStartScheduler();
	


}



void StartDefaultTask(void *argument)
{
  for(;;)
  {
    osDelay(1);
  }
}


















void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
