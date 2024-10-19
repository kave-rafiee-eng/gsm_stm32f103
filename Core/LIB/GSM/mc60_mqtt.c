#include "main.h"
#include "mc60_mqtt.h"

// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];

extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;

extern struct UART_DATA sim_uart_buffer;

// ADVANCE
extern struct ADVANCE  advance;

struct SIM800_STATUS mc60_status;

extern struct GSM gsm;

void mc60_mqtt_pub( char *topic , char *data);
void mc60_mqtt_sub(char *topic);
	
void mv60_led_show(){
	
	if ( mc60_status.MQTT_READY == 0 && ( mc60_status.SIM_CART_INSERT == 1 ) ) LED_SIM_TOGGLE();
		
	if ( mc60_status.SIM_CART_INSERT != 1 ) LED_SIM(0);
	
	if ( mc60_status.MQTT_READY == 1 && mc60_status.SIM_CART_INSERT == 1  ) LED_SIM(1);
	
}

char lost_i=3;

void mc60_mqtt_manage(){

	/*SIM_ON(0);
	osDelay(1000);
	SIM_ON(1);
	mc60_status.SIM_CART_INSERT = wait_to_get_sim("SMS Ready",10000); */
	
	
	if( lost_i >= 3 ){ 
		
		while(1){
			
			mc60_status.MQTT_READY=0; mc60_status.SIM_CART_INSERT=0;
			
			SIM_ON(1);
			osDelay(1000);
			SIM_ON(0);
			
			osDelay(2000);
		
			sim_send_str("AT\n");
			if( wait_to_get_sim("OK",3000) == 1 ){  break;	 }	
			
		}

		lost_i=0;
		sim_send_str("at+cfun=1,1\n");
		wait_to_get_sim("SMS",20000); 				
	}

	osDelay(1000);
	
	sim_send_str("AT\n"); 
	osDelay(100);
	sim_send_str("AT\n"); 
	osDelay(100);
	
	sim_send_str("ATE0\n"); osDelay(100);
	
	sim_send_str("AT+CPIN?\n");
	mc60_status.SIM_CART_INSERT = wait_to_get_sim("READY",5000); 
	osDelay(100);
	
	//advance.READY=1;
	if( mc60_status.SIM_CART_INSERT == 1 && advance.READY == 1 ){
			//sim_send_str("AT+QMTCLOSE=0\n");
			//osDelay(1000);
			
			sim_send_str("AT+QMTOPEN=0,\"84.47.232.10\",\"1883\"\n");
			mc60_status.MQTT_READY = wait_to_get_sim("+QMTOPEN",5000);
			osDelay(1000);
			
			if( mc60_status.MQTT_READY ){
				
					sim_send_str("AT+QMTCONN=0,\"clientExample\"\n");
					mc60_status.MQTT_READY = wait_to_get_sim("+QMTCONN",5000);
					osDelay(1000);		
		
					char buf_tx[100];
					sprintf(buf_tx,"server/%d",advance.SERIAL);
				
					mc60_mqtt_sub(buf_tx);				
			}

			int num=0;
			while( mc60_status.MQTT_READY == 1 ){
						

				/*char buf_tx[100];
				sprintf(buf_tx,"rafiee/%d\n",num);
				num++;
				
				//+QMTPUB
				mc60_mqtt_pub("top_test",buf_tx);					
				osDelay(1000);*/
				if( sim.F_data_for_server ){ sim.F_data_for_server=0;	
					mc60_mqtt_pub("gsm",modbus_slave.buf);
					tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].I_time=0;
					osDelay(100);
				}
				
				osDelay(1);
				
				tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].EN=1;
				tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].AUTO=1;
				tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].C_set_time=3;

				if( sim.F_json_get ){ tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].I_time=0;
					tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end=0;
				}
				
				if( tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end ){ tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end=0;
					
						char str[100];
						sprintf(str,"{\"serial\":\"%d\"}",advance.SERIAL);
										
						mc60_mqtt_pub("gsm",str);
				}
				
				if( gsm.F_send_EN_USER ){ gsm.F_send_EN_USER=0;
					
						char str[80];
						sprintf(str,"{\"serial\":\"%d\",\"en_user\":\"1\"}",advance.SERIAL);
						mc60_mqtt_pub("gsm",str);
				}
				
			}	

		
	}
	
	lost_i++;
		
}

void mc60_start_mqtt(){
	
	
}

void mc60_mqtt_pub( char *topic , char *data){
	
	char buf_tx[50];
	sprintf(buf_tx,"AT+QMTPUB=0,0,0,0,\"%s\"\n\r",topic);
	sim_send_str(buf_tx);
	osDelay(100);
	//whit_to_responce_sim(100);
	
	sim_send_str(data);
	if( strfind(data,"}") >= 0 ){}
		else sim_send_str("}");
	UART_PUT_CHAR(26,UART_SIM);
	
	whit_to_responce_sim(4000);
	
	mc60_status.MQTT_READY = wait_to_get_sim("QMTPUB",5000);
	
}

void mc60_mqtt_sub(char *topic){
	
	char buf_tx[100];
	sprintf(buf_tx,"AT+QMTSUB=0,1,\"%s\",2\n",topic);
	sim_send_str(buf_tx);
	osDelay(5);
	
	whit_to_responce_sim(4000);
	
	/*if( strfind(sim_uart_buffer.BUF,"OK") > 0 ){ mc60_status.MQTT_READY=1; }
	if( strfind(sim_uart_buffer.BUF,"ERROR") > 0 ){ mc60_status.MQTT_READY=0; }*/
	
	mc60_status.MQTT_READY = wait_to_get_sim("QMTSUB",5000);
	
}

