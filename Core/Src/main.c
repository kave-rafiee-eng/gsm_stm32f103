#include "main.h"
#include "cmsis_os.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

 int fer=700;
 int panel=0;

void Vtask_advance( void *pvParameters ){ 
	
	 for( ;; ){
		 
		 /*modbus_drive_write_register_single(1,6,0x1000,fer);
		 osDelay(100);
		 modbus_drive_write_register_single(1,6,0x2000,panel);
		 osDelay(100);
		 //test_drive_modbus();
		 //modbus_drive_write_register_single(1,6,3,4);
*/
		 
		 //osDelay(10000);
		 
		 	tbr_g1[tbr_g1_LED_BLANK].EN=1;
			tbr_g1[tbr_g1_LED_BLANK].C_set_time=300;
			if(tbr_g1[tbr_g1_LED_BLANK].F_end){tbr_g1[tbr_g1_LED_BLANK].F_end=0;
					esp_led_show();
					advance_led_show();
					mv60_led_show();
			}
			
			advance_manager();
		 
			test_modbus();
		 
	 }
 
}

void Vtask_sim( void *pvParameters ){ 
	
	 for( ;; ){	 
		
			mc60_mqtt_manage();
			osDelay(1);
	 }
}

extern int size;

void Vtask_wifi( void *pvParameters ){ 
		for( ;; ){ 
			esp8266_manager();
			osDelay(1);
	 }
}

void StartDefaultTask(void *argument);


int main(void)
{

  HAL_Init();

	Hardware_init();
	Software_init();

	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,1);
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
SIM_ON(0);
	
	xTaskCreate(Vtask_wifi,"task_wifi",100,NULL,1,NULL);
	xTaskCreate(Vtask_sim,"task_sim",100,NULL,1,NULL);
	xTaskCreate(Vtask_advance,"task_advance",100,NULL,1,NULL);
	
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
