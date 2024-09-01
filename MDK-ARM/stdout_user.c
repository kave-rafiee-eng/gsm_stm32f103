#include "main.h"

int stdout_putchar (int ch) {
	
 LL_USART_TransmitData8(USART2, ch);		 
 while(!LL_USART_IsActiveFlag_TXE(USART2));
	
  return 1;
}
