#include "main.h"
#include "esp8266.h"


extern struct GSM gsm;

// STDOUT
extern char UART_STDOUT_SELECT;

struct ESP8266 esp;
struct ESP8266_MANAGE esp_manage;

void esp_random_connect_to_server(){
	
	gsm.device_serial=100;
	
	if( gsm.F_send_EN_USER ){ gsm.F_send_EN_USER=0;
			char str[80];
			sprintf(str,"{\"serial\":\"%d\",\"url\":\"SW_ENABLE=1&\",}",gsm.device_serial);
			UART_STDOUT_SELECT = UART_ESP;
			puts(str);			
	}
	else{
			char str[80];
			sprintf(str,"{\"serial\":\"%d\",\"name_w1\":\"-\",\"data_w1\":\"-\",}",gsm.device_serial);
			UART_STDOUT_SELECT = UART_ESP;
			puts(str);	
	}
	
}

void esp_uart_rx_manager(char data){
	
		if( data == '{' )esp_manage.json_level=1;

		if( esp_manage.json_level == 1 ){	
			esp.BUF[esp.BUF_I] = data;
			esp.BUF_I++;
			if( data == '}' ){ esp_manage.json_level=2;
				esp.F_json_get=1;
			}
		}
		
		if( esp.BUF_I >= UART_BUF_SIZE )esp.BUF_I=0;
	
}


void clear_esp_buffer(){
	
	memset(esp.BUF,0,UART_BUF_SIZE);
	esp_manage.json_level=0;
	esp.F_json_get=0;
	esp.BUF_I=0;
	
}