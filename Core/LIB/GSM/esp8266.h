
#ifndef __ESP8266_H
#define __ESP8266_H

#include "main.h"

void esp_random_connect_to_server();
void esp8266_connection_test();
void manage_esp_responce();
char wait_to_esp_get_json( int time_out );
void esp_uart_rx_manager(char data);
void clear_esp_buffer();
void esp8266_send_en_user();

struct ESP8266_STATUS
{
	char ERROR_NOT_RESPONCE : 1;
	char ERROR_WIFI : 1;
	char ERROR_HTTP : 1;
	char ERROR_MQTT : 1;
	
	char READY : 1;
	

};

struct ESP8266 
{
	char F_json_get : 1;
	
	volatile unsigned char BUF_JSON[UART_BUF_SIZE];
	volatile unsigned char BUF_JSON_index;
	
	char F_data_for_advance : 1;
	char F_data_for_server : 1;

};

struct ESP8266_MANAGE
{
	char json_level ;

};

#endif /* ESP8266 */