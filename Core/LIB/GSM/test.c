#include "main.h"
#include "test.h"
#include "stdio.h"

#define travel_time_addres "general*travel_time"
#define door_control_type_ADDRESS "general*door*control_type"

extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

//extern char List_DoorControlType[][8];
	
//extern volatile	uint8_t			Stng[SETTING_ARRAY_SIZE];
//extern volatile  uint8_t   	EEPROMSaveFlag;

extern struct 	UART_DATA esp_data;

extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;

char flag_test=0;
int i_get=0;
char last_get=0;

int time=0;
int timer_eeprom=0;

char modbus_start=0;

uint16_t modbus_read[20];

char uart_select=0;

extern volatile char start_get;

void test_modbus(){
	
	MODBUS_ADVANCE_RS(1);
	modbus_start=1;
	if( modbus_start == 1 ){ modbus_start=0;
		//modbus_master_write_register_SINGLE(SLAVE_ADD,FC_WRITE_TO_SLAVE_SINGLE,2,7);
		
			if( start_get == 2 ){ start_get=0;
				uart_select=2;
				//uint8_t data[]={"{\"name_w1\":\"general*travel_time\",\"data_w1\":\"10\",}"};
				modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(esp_data.BUF),esp_data.BUF);
				
			}
		//modbus_master_read_reginter(01,01,01,01,modbus_read);
	}

	if( esp_data.BUF_I > 0 ){
			//modbus_slave_manage();
			esp_data.BUF_I=0;
	}
	
}

void send_to_esp2(){
	
	char str[100];
	sprintf(str,"{\"name_w1\":\"-\",\"data_w1\":\"-\",}");
	uart_select = 1;
	puts(str);
	
}

void test_json(){
	
	tbr_g1[tbr_g1_TEST].EN=1;
	tbr_g1[tbr_g1_TEST].C_set_time=10;
	
	if( tbr_g1[tbr_g1_TEST].F_end ){ tbr_g1[tbr_g1_TEST].F_end=0;
		
		//puts("a");
		
		last_get=0;
		int i=0;
		for(i=0; i<UART_BUF_SIZE; i++ ){
			if( esp_data.BUF[i] == '}' ){
				last_get = '}';
				break;
			}
		}
		
		if( last_get == '}' ){
			
			json_get_document(esp_data.BUF,&esp_data.BUF_I);
			
			read_protocol_json();
	
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
					}	*/		
					
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
		
					}*/
									
			}
			else{
				time=100;
			}
			
			reset_json();
			
		}
		
		if(time>0)time--;
			
	}
	
	
	if( time == 0 ){ time=500;
		char str[100];
		sprintf(str,"{\"name_w1\":\"-\",\"data_w1\":\"-\",}");
		puts(str); 					
	}

}

void gsm_init(){
	
	ini_cpu_timer();
	
}



//HALL_SendData(strlen(HALL_TX_Buffer));	


/* DMA :

//i_get = UART_BUFFER_SIZE - DMA1_Stream2->NDTR;
//extern char 									HALL_TX_Buffer[UART_BUFFER_SIZE];
//extern volatile	char				HALL_RX_Buffer[UART_BUFFER_SIZE];
//json_get_data_dma();
*/
