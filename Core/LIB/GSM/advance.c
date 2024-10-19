#include "main.h"
#include "advance.h"

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
extern struct ESP8266 esp;
extern struct ESP8266_MANAGE esp_manage;
extern struct ESP8266_STATUS esp_status;

// ADVANCE
struct ADVANCE  advance;


void advance_led_show(){
	
	if ( advance.READY ) LED_ADVANCE(1);
	else LED_ADVANCE(0);

}

void advance_manager(){
	
/*	if( advance.SERIAL == 0 && advance.ERROR_NOT_RESPONCE == 0 ){
		advance_connection_test();
	}*/
	
	tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].EN=1;
	tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].AUTO=1;
	tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].C_set_time=8;
	
	if( tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].F_end ){ tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].F_end=0;
			advance_connection_test();			
	}
			
}


void advance_connection_test(){
	
	char str[80];
	sprintf(str,"{\"serial\":\"?\",}");
	modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(str),str);
	manage_advance_responce();
	
}


void manage_advance_responce(){
	
	if( wait_to_advance_get_data(2000) == 0 )advance.ERROR_NOT_RESPONCE=1;
	else { advance.ERROR_NOT_RESPONCE=0;
			reset_json();
			strcpy(json.document,modbus_slave.buf,UART_BUF_SIZE);
			
			if( json_get_data(json.document , "\"serial\":") == TYPE_STR ){
					advance.SERIAL=atoi(json.str_data);
					advance.ERROR_NOT_RESPONCE=0;
					advance.READY=1;
			}
			else{
				advance.ERROR_NOT_RESPONCE=1;
			}
	}

	if( advance.ERROR_NOT_RESPONCE )advance.READY=0;	
	
	modbus_slave.F_new_data=0;
	
}

char wait_to_advance_get_data( int time_out ){
		
	int time=0;
	while( 1 ){ 
		modbus_slave_manager_recive();
		osDelay(1);
		if( modbus_slave.F_new_data  )return 1;
		time++;
		if( time > time_out )return 0;
	}
}




