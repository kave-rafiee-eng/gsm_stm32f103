#include "main.h"
#include "Initialization.h"

void Hardware_init( void ){
	
	SystemClock_INI();
	GPIO_INI();
	
	UART1_INI();
	UART2_INI();
	UART3_INI();
	
}


void Software_init( void ){
	
	ini_cpu_timer();
	
}
