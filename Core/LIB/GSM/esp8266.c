#include "main.h"
#include "esp8266.h"

// JSON
extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;


// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

extern struct GSM gsm;

// STDOUT
extern char UART_STDOUT_SELECT;

struct ESP8266 esp;
struct ESP8266_MANAGE esp_manage;


void SIM_random_connect_to_server(){
	
	
}

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

//------------------------------------------ SIM


struct ESP8266 sim;
struct ESP8266_MANAGE sim_manage;

struct UART_DATA sim_uart_buffer;

void sim_uart_rx_manager(char data){
	
		if( data == '{' )sim_manage.json_level=1;

		if( sim_manage.json_level == 1 ){	
			sim.BUF[sim.BUF_I] = data;
			sim.BUF_I++;
			if( data == '}' ){ sim_manage.json_level=2;
				sim.F_json_get=1;
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
			sprintf(str,"AT+HTTPPARA=\"URL\",\"www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100&%d\"");
			puts(str);
			whit_to_responce_sim();		
	}
							
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPACTION=0");
	puts(str);
	if( wait_to_get_sim("HTTPACTION:",4000) ==  1 ){
		
		clear_sim_uart_buffer();
		sprintf(str,"AT+HTTPREAD");
		puts(str);
		whit_to_responce_sim();		
	}
						
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPTERM");
	puts(str);
	whit_to_responce_sim();
	
	
}

void tset_send_sim(){
	
	UART_STDOUT_SELECT = UART_SIM;
	char str[100];
	
	clear_sim_uart_buffer();
	sprintf(str,"AT");
	puts(str);
	whit_to_responce_sim();
	
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(2000);
	
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=3,1,\"APN\",\"mcinet\"");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(2000);
	
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=1,1");
	puts(str);
	whit_to_responce_sim();
	
	osDelay(2000);
			
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=2,1");
	puts(str);
	whit_to_responce_sim();
	
/*
	osDelay(2000);				
	clear_sim_uart_buffer();
	sprintf(str,"AT+SAPBR=0,1");
	puts(str);
	whit_to_responce_sim();
	*/
	
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
