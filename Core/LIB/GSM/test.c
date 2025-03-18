#include "main.h"
#include "test.h"
#include "stdio.h"
#include "stdlib.h"

struct GSM gsm;
	
// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];
// ESP
extern struct ESP8266 esp;
extern struct ESP8266_MANAGE esp_manage;
extern struct ESP8266_STATUS esp_status;

// SIM
extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;
extern struct SIM800_STATUS sim800_status;

// JSON
extern struct JSON_OUT	 json;
extern struct JSON_PROTOCOL json_protocol;

// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

// STDOUT
extern char UART_STDOUT_SELECT;

extern int time_esp;

extern struct SIM800_STATUS mc60_status;

#define RETRY_INTERVAL 500
#define TIMEOUT_LIMIT 1000

void MAIN_communication(){
	
	if( !HAL_GPIO_ReadPin(SW_EN_GPIO,SW_EN_PIN) ){ 
		for(char i=0; i<5; i++ ){
			LED_ESP(i%2);
			LED_SIM(i%2);   
			LED_ADVANCE(i%2); 	
			osDelay(300);			
		}

		gsm.F_send_EN_USER=1;
		gsm.COUNTER_timer_EN_USER=COUNTER_EN_USER_SEND;
	}
	
	if( gsm.COUNTER_timer_EN_USER > 0 ){
		
			tbrc_s1[tbrc_s1_ESP_EN_USER].EN=1;
			tbrc_s1[tbrc_s1_ESP_EN_USER].AUTO=1;
			tbrc_s1[tbrc_s1_ESP_EN_USER].C_set_time=TIME_EN_USER_SEND;
					
			if( tbrc_s1[tbrc_s1_ESP_EN_USER].F_end ){ tbrc_s1[tbrc_s1_ESP_EN_USER].F_end=0;
					if(esp_status.READY || mc60_status.MQTT_READY ) { gsm.F_send_EN_USER=1;
						gsm.COUNTER_timer_EN_USER--;
					}
			}			
			
	}
	else tbrc_s1[tbrc_s1_ESP_EN_USER].EN=0;
	
	
	MODBUS_ADVANCE_RS(0);

	if( esp.F_data_for_advance  ){  //SEND DATA_JSON ESP TO ADVANCE strlen(esp.BUF_JSON)
		if( esp.BUF_JSON_index>3) modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,esp.BUF_JSON_index,esp.BUF_JSON);
		int time=0;
		
			while (time <= TIMEOUT_LIMIT) {
					tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].I_time = 0;
					tbrc_s1[tbrc_s1_ESP_RANDOM_CONNET].F_end = 0;

					osDelay(1);
					time++;

					if (time == RETRY_INTERVAL) {
							modbus_master_write_register_MULTI(SLAVE_ADD, FC_WRITE_TO_SLAVE_MULTI, 2, strlen(esp.BUF_JSON), esp.BUF_JSON);
					}

					modbus_slave_manager_recive();

					if (modbus_slave.F_new_data) break;
			}
		
		clear_esp_buffer();
		esp.F_data_for_advance=0;
	}
	
	if( sim.F_data_for_advance && esp_status.READY == 0 ){  //SEND DATA_JSON mc60 TO ADVANCE	
		
		tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].I_time=0;
		tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end=0;
		
		if( sim.BUF_JSON_index>3) modbus_master_write_register_MULTI(SLAVE_ADD,FC_WRITE_TO_SLAVE_MULTI,2,strlen(sim.BUF_JSON),sim.BUF_JSON);
		
		int time=0;
		
    while (time <= TIMEOUT_LIMIT) {
        tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].I_time = 0;
        tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end = 0;

        osDelay(1);
        time++;

        if (time == RETRY_INTERVAL) {
            modbus_master_write_register_MULTI(SLAVE_ADD, FC_WRITE_TO_SLAVE_MULTI, 2, strlen(sim.BUF_JSON), sim.BUF_JSON);
        }

        modbus_slave_manager_recive();

        if (modbus_slave.F_new_data) break;
    }
		
		clear_mc60_buffer();
		sim.F_data_for_advance=0;
	}
	
	if( sim.F_data_for_advance && esp_status.READY == 1 ){
		clear_mc60_buffer();
		sim.F_json_get=0;
	}

	
	modbus_slave_manager_recive();
	
	if( modbus_slave.F_new_data ){ modbus_slave.F_new_data=0;
		
			tbrc_s1[tbrc_s1_ADVANCE_CONNECTION_TEST].I_time=0;

			if( esp_status.READY )esp.F_data_for_server=1;
			else if( mc60_status.MQTT_READY )sim.F_data_for_server=1;
			
			reset_json();
			strncpy(json.document, (const char *)modbus_slave.buf, sizeof(json.document) - 1);
			json.document[sizeof(json.document) - 1] = '\0';
		
			if( json_get_data(json.document , "\"serial\":")  == TYPE_STR )gsm.device_serial = atoi(json.str_data);

	}
	
}


