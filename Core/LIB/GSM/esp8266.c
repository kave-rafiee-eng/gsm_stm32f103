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



void esp_led_show(){
	
	if ( esp_status.ERROR_WIFI == 0 && ( esp_status.ERROR_HTTP == 1 ) ) HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_4);
		
	if ( esp_status.ERROR_NOT_RESPONCE || esp_status.ERROR_WIFI  ) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,0);
	
	if ( esp_status.READY  ) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,1);
	
}

void esp8266_manager(){
	
	if( esp_status.READY ){
		
		if( esp.F_data_for_server ){	
			esp_random_connect_to_server();
		}
		
		if( esp.F_data_for_advance == 0 ){
				tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].EN=1;
				tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].AUTO=1;
				
				if( tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].F_end ){ tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].F_end=0;
					
					if( advance.READY )esp_random_connect_to_server();
					else esp8266_connection_test();
					
					tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].C_set_time=2;
				}
		}
		else tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].EN=0;
		
	}
	else{
			esp8266_connection_test();
	}


	
}



void esp_random_connect_to_server(){
	
	gsm.device_serial=100;
	
	if( esp.F_data_for_server ){ esp.F_data_for_server=0;
		char str[80];
		UART_PRINT((char*)modbus_slave.buf,UART_ESP);
		//manage_esp_responce();	
	}
	/*else if( gsm.F_send_EN_USER ){ gsm.F_send_EN_USER=0;
		char str[80];
		sprintf(str,"{\"serial\":\"%d\",\"url\":\"SW_ENABLE=1&\",}",advance.SERIAL);
		UART_PRINT(str,UART_ESP);
		manage_esp_responce();	
	}
	else{
		char str[80];
		sprintf(str,"{\"serial\":\"%d\",}",advance.SERIAL);
		UART_PRINT(str,UART_ESP);
		manage_esp_responce();
	}*/
	
}


void esp8266_connection_test(){
	
	clear_esp_buffer();
	
	char str[80];
	sprintf(str,"{connection_test:\"?\",}");
		
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
				if( strcmp(json.str_data,"http_code") == 0 )esp_status.ERROR_HTTP=1;
				else if( strcmp(json.str_data,"http_begin") == 0 )esp_status.ERROR_HTTP=1;
				else esp_status.ERROR_HTTP=0;
			}
			else{
				esp_status.ERROR_WIFI=0;
				esp_status.ERROR_HTTP=0;
				esp_status.READY=1;
			}
	}

	if( esp_status.ERROR_HTTP || esp_status.ERROR_NOT_RESPONCE ||esp_status.ERROR_WIFI )esp_status.READY=0;
	else { esp_status.READY=1;

		esp.F_data_for_advance=1;
		
		tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].C_set_time=5;
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

/*
void SIM_random_connect_to_server(){
	
	
}

}
*/



/*
struct ESP8266 sim;
struct ESP8266_MANAGE sim_manage;

struct UART_DATA sim_uart_buffer;

void sim_uart_rx_manager(char data){
	
		if( data == '{' )sim_manage.json_level=1;

		if( sim_manage.json_level == 1 ){	
			sim.BUF[sim.BUF_I] = data;
			sim.BUF_I++;
			if( data == '}' ){ sim_manage.json_level=2;
				
				if( sim.BUF_I > 3 )sim.F_json_get=1;
				else{
						sim.BUF_I=0;
						sim_manage.json_level=0;
				}
				
			}
		}
		
		if( sim.BUF_I >= UART_BUF_SIZE )sim.BUF_I=0;
		
		tbr_g1[tbr_g1_SIM_UART_GET].EN=1;
		tbr_g1[tbr_g1_SIM_UART_GET].I_time=0;
		tbr_g1[tbr_g1_SIM_UART_GET].C_set_time=5;
		
		sim_uart_buffer.BUF[sim_uart_buffer.BUF_I] = data;
		sim_uart_buffer.BUF_I++;
}

void clear_sim_buffer(){
	memset(sim.BUF,0,UART_BUF_SIZE);
	memset(sim_uart_buffer.BUF,0,UART_BUF_SIZE);
	esp_manage.json_level=0;
	sim.F_json_get=0;
	sim.BUF_I=0;
	sim_uart_buffer.BUF_I=0;
}

void clear_sim_uart_buffer(){

	memset(sim_uart_buffer.BUF,0,UART_BUF_SIZE);
	sim_uart_buffer.BUF_I=0;
}

void whit_to_responce_sim(){
		
	int time=0;
	
	while( tbr_g1[tbr_g1_SIM_UART_GET].F_end == 0 ){
		osDelay(1);
		time++;
		if( time > 1000 )break;
	}
	tbr_g1[tbr_g1_SIM_UART_GET].F_end=0;
	tbr_g1[tbr_g1_SIM_UART_GET].EN=0;
	
}

char wait_to_get_sim( char *buffer , int time_out ){
		
	int time=0;
	
	while( 1 ){
		
			if( strfind(sim_uart_buffer.BUF,buffer) > 0 )return 1;
			osDelay(1);

			time++;
			if( time > time_out )return 0;
		
	}
	

}


void http_read(){

	UART_STDOUT_SELECT = UART_SIM;
	char str[100];

	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPINIT");
	puts(str);
	whit_to_responce_sim();
					
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPPARA=\"CID\",1");
	puts(str);
	whit_to_responce_sim();
		
	if( json_get_data(json.document , "\"url\":") == TYPE_STR ){
			clear_sim_uart_buffer();
			sprintf(str,"AT+HTTPPARA=\"URL\",\"www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100&%s\"",json.str_data);
			puts(str);
			whit_to_responce_sim();				
	}
	else{
		
			clear_sim_uart_buffer();
			sprintf(str,"AT+HTTPPARA=\"URL\",\"www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100\"");
			puts(str);
			whit_to_responce_sim();		
	}
							
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPACTION=0");
	puts(str);
	if( wait_to_get_sim("HTTPACTION:",4000) ==  1 ){}
		
		clear_sim_uart_buffer();
		sprintf(str,"AT+HTTPREAD");
		puts(str);
		whit_to_responce_sim();		
	
						
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPTERM");
	puts(str);
	whit_to_responce_sim();
	
	
}

void tset_send_sim(){
	
	UART_STDOUT_SELECT = UART_SIM;
	char str[100];
	
	/*clear_sim_uart_buffer();
	sprintf(str,"AT");
	puts(str);
	whit_to_responce_sim();*/
	

	/*
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(500);
	
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=3,1,\"APN\",\"RighTel\"");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(500);
	
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=1,1");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(500);
			
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=2,1");
	puts(str);
	whit_to_responce_sim();
	

	osDelay(2000);				
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=0,1");
	puts(str);
	whit_to_responce_sim();
	
	
}

/*

AT+HTTPPARA="URL","https://www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100&"



//Get Product name
AT


//Command to read signal strength
AT+CSQ


//Get device phone number
AT+CIMI

//Get the device serial number
AT+GSN

//TYPE GPRS CONNECTIO
AT+SAPBR=3,1,"Contype","GPRS"

//	mtnirancell	mcinet	RighTel
AT+SAPBR=3,1,"APN","RighTel"

//ENABLE GPRS
AT+SAPBR=1,1

//GET IP
AT+SAPBR=2,1

// ENABLE HTTP
AT+HTTPINIT

AT+HTTPPARA="CID",1

AT+HTTPPARA="URL","www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100&"

// START HTTP
AT+HTTPACTION=0

// READ HTTP
AT+HTTPREAD

// END HTTP
AT+HTTPTERM


// GPRS OFF
AT+SAPBR=0,1

*/

char wait_to_get( char *buffer ,char *sub_str , int time_out ){
		
	int time=0;
	while( 1 ){
		if( strfind(buffer,sub_str) > 0 )return 1;
		osDelay(1);
		time++;
		if( time > time_out )return 0;
	}
}

