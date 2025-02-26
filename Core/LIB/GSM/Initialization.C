#include "main.h"
#include "Initialization.h"

void Hardware_init( void ){
	
	SystemClock_INI();
	GPIO_INI();
	
	UART1_INI();
	UART2_INI();
	UART3_INI();
	
	MX_CAN_Init();
	
	HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

}


void Software_init( void ){
	
	ini_cpu_timer();
	
}
