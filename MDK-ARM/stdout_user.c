#include "main.h"

char UART_STDOUT_SELECT=0;

extern char uart_select;

int stdout_putchar (int ch) {
	
		
	if( UART_STDOUT_SELECT == 3 ){
		LL_USART_TransmitData8(USART3, ch);		 
		while(!LL_USART_IsActiveFlag_TXE(USART3));
	}
	
	if( UART_STDOUT_SELECT == 2 ){
		LL_USART_TransmitData8(USART2, ch);		 
		while(!LL_USART_IsActiveFlag_TXE(USART2));
	}
	
	if( UART_STDOUT_SELECT == 1 ){
		
		LL_USART_TransmitData8(USART1, ch);		 
		while(!LL_USART_IsActiveFlag_TXE(USART1));		
	}

  return 1;
}
