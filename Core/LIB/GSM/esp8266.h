
#ifndef __ESP8266_H
#define __ESP8266_H

#include "main.h"

void esp_uart_rx_manager(char data);
void clear_esp_buffer();
void esp_random_connect_to_server();

struct ESP8266 
{
	char F_json_get : 1;
	
	volatile unsigned char BUF[UART_BUF_SIZE];
	volatile unsigned char BUF_I ;

};

struct ESP8266_MANAGE
{
	char json_level ;

};

#endif /* ESP8266 */