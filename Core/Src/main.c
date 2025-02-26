#include "main.h"
#include "cmsis_os.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

//Main
void Vtask_advance( void *pvParameters ){ 
	
	 for( ;; ){
		 
		 	tbr_g1[tbr_g1_STATUS_LEDS].EN=1;
			tbr_g1[tbr_g1_STATUS_LEDS].C_set_time=300;
			if(tbr_g1[tbr_g1_STATUS_LEDS].F_end){tbr_g1[tbr_g1_STATUS_LEDS].F_end=0;
					ESP_led_status();
					ADVANCE_led_status();
					MC60_led_status();
			}
			
			advance_manager();
		 
			MAIN_communication();		
			
			//-----------------
			//test_can();
			//test_shasi();
			//---------------
	
			osDelay(1);
			
	 }
 
}

void Vtask_sim( void *pvParameters ){ 
	
	 for( ;; ){	 
		
			//mc60_mqtt_manage();
			osDelay(1);
	 }
}

extern int size;

void Vtask_wifi( void *pvParameters ){ 
		for( ;; ){ 
			ESP_manager();
			osDelay(1);
	 }
}

void StartDefaultTask(void *argument);


int main(void)
{

  HAL_Init();

	Hardware_init();
	Software_init();

	SIM_ON(0);
	
	xTaskCreate(Vtask_wifi,"task_wifi",100,NULL,1,NULL);
	xTaskCreate(Vtask_sim,"task_sim",100,NULL,1,NULL);
	xTaskCreate(Vtask_advance,"task_advance",100,NULL,1,NULL);
	
	vTaskStartScheduler();
	
	MODBUS_ADVANCE_RS(0);
	
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
