#include "main.h"

int stdout_putchar (int ch) {
	
 LL_USART_TransmitData8(USART1, ch);		 
 while(!LL_USART_IsActiveFlag_TXE(USART1));
	
  return 1;
}
