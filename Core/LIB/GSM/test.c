#include "main.h"
#include "test.h"
#include "stdio.h"
#include "stdlib.h"

struct GSM gsm;
	
// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

// ESP
extern struct ESP8266 esp;
extern struct ESP8266_MANAGE esp_manage;
extern struct ESP8266_STATUS esp_status;

// SIM
extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;

// JSON
extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;

// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

// STDOUT
extern char UART_STDOUT_SELECT;

extern int time_esp;

void test_modbus(){
	
	//if( !HAL_GPIO_ReadPin(SW_EN_GPIO,SW_EN_PIN) )gsm.F_send_EN_USER=1;
	
	MODBUS_ADVANCE_RS(0);

	if( esp.F_data_for_advance  ){ esp.F_data_for_advance=0; //SEND DATA_JSON ESP TO ADVANCE
		
		if( esp.BUF_JSON_index>3) modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(esp.BUF_JSON),esp.BUF_JSON);
		clear_esp_buffer();
	}
	
	/*if( sim.F_json_get ){ sim.F_json_get=0; //SEND DATA_JSON ESP TO ADVANCE
		
		if( sim.BUF_JSON_index>3) modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(sim.BUF_JSON),sim.BUF_JSON);
		clear_sim_buffer_json();
	}*/

	
	modbus_slave_manager_recive();
	
	if( modbus_slave.F_new_data ){ modbus_slave.F_new_data=0;
		
			//sim.F_data_for_server = 1;
		
			if( esp_status.READY )esp.F_data_for_server=1;
		
			//UART_PRINT((char*)modbus_slave.buf,UART_ESP);
			
			//read_json_advance();
			//reset_json();
	}
	
}

/*
void read_json_advance(){
	
	reset_json();
	strcpy(json.document,modbus_slave.buf,UART_BUF_SIZE);
		
	read_protocol_json();
	
	if( json_get_data(json.document , "\"serial\":") == TYPE_STR ){
		gsm.device_serial	= atoi(json.str_data);	
	}
			
}
*/
/*
#define travel_time_addres "general*travel_time"
#define door_control_type_ADDRESS "general*door*control_type"

void test_json(){

			if( json_protocol.data_w1_type > 0 ){
				
					puts("/name = ");
					puts( json_protocol.name_w1 );	
				
					if( json_protocol.data_w1_type == TYPE_STR ){
							puts("/data_str = ");
							puts( json_protocol.data_w1 );	
					}
					else if( json_protocol.data_w1_type == TYPE_WORD ){
							char str[50];
							sprintf(str,"/data_word = %d",json_protocol.data_w1_word);
							puts(str);    
					}
					
					/*if( strcmp(json_protocol.name_w1,travel_time_addres) == 0 ){ // write travel_time_addres	
							if( json_protocol.data_w1_type == TYPE_STR ){
									//puts("/travel_time$ = ");
									Stng[TRAVEL_TIME] = atoi(json_protocol.data_w1);
									timer_eeprom = 500;
							}			
					}
					
					if( strcmp(json_protocol.name_w1,door_control_type_ADDRESS) == 0 ){ // write door_control_type_ADDRESS	
							if( json_protocol.data_w1_type == TYPE_STR ){			
									char i=0;
									for(i=0;i<3;i++){
											if( strcmp( json_protocol.data_w1 , List_DoorControlType[i] ) == 0 ){
												Stng[DOOR_CONTROL_TYPE] = i;
												timer_eeprom = 500;											
											}
									}
							}			
					}			
					
			}

			if( json_protocol.data_r1_type > 0 ){
				
					/*if( strcmp(json_protocol.name_r1,travel_time_addres) == 0 ){ // read travel_time_addres	
						char str[100];
						sprintf(str,"{\"name_w1\":\"%s\",\"data_w1\":%d,}",travel_time_addres,Stng[TRAVEL_TIME]);
						puts(str); 
						time=1;	
		
					}
					
					if( strcmp(json_protocol.name_r1,door_control_type_ADDRESS) == 0 ){ // read door_control_type_ADDRESS	
						char str[100];
						sprintf(str,"{\"name_w1\":\"%s\",\"data_w1\":\"%s\",}",door_control_type_ADDRESS,List_DoorControlType[Stng[DOOR_CONTROL_TYPE]]);
						puts(str); 
						time=1;	
		
					}
									
			}
			else{
				time=100;
			}
		
		}
		


}*/


//HALL_SendData(strlen(HALL_TX_Buffer));	

/* DMA :

//i_get = UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
//extern char 									HALL_TX_Buffer[UART_BUFFER_SIZE];
//extern volatile	char				HALL_RX_Buffer[UART_BUFFER_SIZE];
//json_get_data_dma();
*/
