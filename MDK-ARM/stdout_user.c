#include "main.h"

extern char uart_select;

int stdout_putchar (int ch) {
	
	if( uart_select == 1 ){
		LL_USART_TransmitData8(USART2, ch);		 
		while(!LL_USART_IsActiveFlag_TXE(USART2));
	}
	
	if( uart_select == 2 ){
		
		LL_USART_TransmitData8(USART1, ch);		 
		while(!LL_USART_IsActiveFlag_TXE(USART1));		
	}

	
  return 1;
}
