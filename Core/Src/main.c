#include "main.h"
#include "cmsis_os.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

int time_esp=0;
char init_sim=0;

void Vtask1( void *pvParameters ){ 
	
	 for( ;; ){
		 
			if( init_sim == 0 ){ init_sim=1;
				tset_send_sim();
			}
		 
			
		 
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
			osDelay(200);
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
			
		 	tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].EN=1;
			tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].C_set_time=800;
			if(tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].F_end){tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].F_end=0;
				
				
				if( time_esp == 0 ){ time_esp=1; esp_random_connect_to_server(); }
				else time_esp--;
				
				//if( time_esp == 0 ){ time_esp=2; http_read(); time_esp=2; }
				//else time_esp--;
				
					
			}	
			
			/*tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].EN=1;
			tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].C_set_time=800;
			if(tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].F_end){tbr_g1[tbr_g1_ESP_RANDOM_CONNECT].F_end=0;
				
			}*/
			
	 }
}

void Vtask2( void *pvParameters ){ 
	
		for( ;; ){
		 
		 osDelay(100);
			test_modbus();
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
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
	HAL_Delay(3000);
	
	
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
