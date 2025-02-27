#include "main.h"
#include "esp8266.h"

// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

// JSON
extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];

// STDOUT
extern char UART_STDOUT_SELECT;

// GSM
extern struct GSM gsm;

// ESP
extern struct ADVANCE  advance;

// ESP
struct ESP8266 esp;
struct ESP8266_MANAGE esp_manage;
struct ESP8266_STATUS esp_status;

extern struct GSM gsm;

void ESP_led_status(){
	
	if ( esp_status.ERROR_WIFI == 0 && ( esp_status.ERROR_MQTT == 1 ) ) LED_ESP_TOGGLE();
		
	if ( esp_status.ERROR_NOT_RESPONCE || esp_status.ERROR_WIFI  ) LED_ESP(0);
	
	if ( esp_status.READY  ) LED_ESP(1);
	
}

void ESP_manager(){
	
	if(esp.F_reset == 0 )ESP_ON(0); // tuen on ESP
	else{
		//restarting ESP
		esp.F_reset=0;
		ESP_ON(1);
		reset_esp_status();
		osDelay(2000);
		ESP_ON(0);
	}
	//---------
	//esp_status.READY=1;
	//advance.READY=1;
	//---------
	if( esp_status.READY ){
		
		if( gsm.F_send_EN_USER ){ gsm.F_send_EN_USER=0;
			esp8266_send_en_user();
			osDelay(1000);
		}
			if( esp.F_data_for_server ){	
				esp_random_connect_to_server();
				tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].I_time=0;
			}
			
			if( esp.F_data_for_advance == 0 ){

				if( esp.F_json_get ){ tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].I_time=0;
					manage_esp_responce();	
				}	
			}
			
			tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].EN=1;
			tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].AUTO=1;
			tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].C_set_time= TIME_ESP_RANDOM_CONNECT;
			
			if( tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].F_end ){ tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].F_end=0;
					esp8266_connection_test();	
			}
			
			//---------------------------- SERVER TSET
			tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_TX].EN=1;
			tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_TX].AUTO=1;
			tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_TX].C_set_time=TIME_ESP_SERVER_CHECK_TX;
			
			if( tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_TX].F_end ){ tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_TX].F_end=0;
					esp8266_server_check_TX();	
			}			
			
	}
	else if( advance.READY ){
			esp8266_connection_test();
			osDelay(1000);
	}
	
	
	//---------------------------- SERVER TSET reset
	tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].EN=1;
	tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].AUTO=1;
	tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].C_set_time=TIME_ESP_SERVER_CHECK_RX;
			
	if( tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].F_end ){ tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].F_end=0;
			esp.F_reset=1;	
	}		

}

void reset_esp_status(){
	
	esp_status.ERROR_HTTP=0;
	esp_status.ERROR_MQTT=0;
	esp_status.ERROR_NOT_RESPONCE=0;
	esp_status.ERROR_WIFI=0;
	esp_status.READY=0;
}

void esp_random_connect_to_server(){
	
	gsm.device_serial=100;
	
	if( esp.F_data_for_server ){ esp.F_data_for_server=0;
		char str[80];
		UART_PRINT((char*)modbus_slave.buf,UART_ESP);
		//manage_esp_responce();	
	}
	
}


void esp8266_server_check_TX(){
	
	clear_esp_buffer();
	
	char str[80];
	sprintf(str,"{\"serial\":\"%d\",\"TEST\":\"1\"}",advance.SERIAL);
		
	UART_PRINT(str,UART_ESP);
	manage_esp_responce();

}

void esp8266_send_en_user(){
	
	clear_esp_buffer();
	
	char str[80];
	sprintf(str,"{\"serial\":\"%d\",\"en_user\":\"1\"}",advance.SERIAL);
		
	UART_PRINT(str,UART_ESP);
	manage_esp_responce();

}

void esp8266_connection_test(){
	
	clear_esp_buffer();
	
	char str[80];
	sprintf(str,"{\"serial\":\"%d\"}",advance.SERIAL);
		
	UART_PRINT(str,UART_ESP);
	manage_esp_responce();

}


void manage_esp_responce(){
	
	if( wait_to_esp_get_json(8000) == 0 )esp_status.ERROR_NOT_RESPONCE=1;
	else { esp_status.ERROR_NOT_RESPONCE=0;
			reset_json();
			strcpy(json.document,esp.BUF_JSON,UART_BUF_SIZE);
			
			if( json_get_data(json.document , "\"error\":") == TYPE_STR ){
				if( strcmp(json.str_data,"wifi") == 0 )esp_status.ERROR_WIFI=1;
				else esp_status.ERROR_WIFI=0;
				/*if( strcmp(json.str_data,"http_code") == 0 )esp_status.ERROR_HTTP=1;
				else if( strcmp(json.str_data,"http_begin") == 0 )esp_status.ERROR_HTTP=1;
				else esp_status.ERROR_HTTP=0;*/
				if( strcmp(json.str_data,"mqtt") == 0 )esp_status.ERROR_MQTT=1;
				
				clear_esp_buffer();
			}
			else{
				esp_status.ERROR_WIFI=0;
				//esp_status.ERROR_HTTP=0;
				esp_status.ERROR_MQTT=0;
				esp_status.READY=1;
			}
		
			
	}

	if( esp_status.ERROR_MQTT || esp_status.ERROR_NOT_RESPONCE ||esp_status.ERROR_WIFI ){ esp_status.READY=0;
		
	}
	else { esp_status.READY=1;

			if( json_get_data(json.document , "\"@empty\":") == TYPE_STR ){
				clear_esp_buffer();
			}
			else if( json_get_data(json.document , "\"SERVER_TEST\":")  > 0 ){
				esp.F_reset=0;
				tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].I_time=0;
				tbrc_s1[tbrc_s1_ESP_SERVER_CHECK_RX].F_end=0;
				clear_esp_buffer();
			}
			else{
				esp.F_data_for_advance=1;
			}
			
	}	
		
}

char wait_to_esp_get_json( int time_out ){
		
	int time=0;
	while( 1 ){ osDelay(1);
		if( esp.F_json_get )return 1;
		time++;
		if( time > time_out )return 0;
	}
}

void esp_uart_rx_manager(char data){
	
		if( data == '{' )esp_manage.json_level=1;

		if( esp_manage.json_level == 1 ){	
			esp.BUF_JSON[esp.BUF_JSON_index] = data;
			esp.BUF_JSON_index++;
			if( data == '}' ){ esp_manage.json_level=2;
				esp.F_json_get=1;
			}
		}
		
		if( esp.BUF_JSON_index >= UART_BUF_SIZE )clear_esp_buffer();
}

void clear_esp_buffer(){
	
	memset(esp.BUF_JSON,0,UART_BUF_SIZE);
	esp_manage.json_level=0;
	esp.F_json_get=0;
	esp.BUF_JSON_index=0;
	
}

//------------------------------------------ SIM

char wait_to_get( char *buffer ,char *sub_str , int time_out ){
		
	int time=0;
	while( 1 ){
		if( strfind(buffer,sub_str) > 0 )return 1;
		osDelay(1);
		time++;
		if( time > time_out )return 0;
	}
}



