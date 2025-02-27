#include "main.h"
#include "mc60_mqtt.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//  MODBUS 
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

//  CPU timer 
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];

//  ESP8266  
extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;
extern struct UART_DATA sim_uart_buffer;

//  ADVANCE 
extern struct ADVANCE advance;

// External GSM structure
extern struct GSM gsm;

// MC60 module status structure
struct SIM800_STATUS mc60_status;


// Function to manage the LED status based on SIM and MQTT states
void MC60_led_status(){
	
	if ( mc60_status.MQTT_READY == 0 && ( mc60_status.SIM_CART_INSERT == 1 ) ) LED_SIM_TOGGLE();
		
	if ( mc60_status.SIM_CART_INSERT != 1 ) LED_SIM(0);
	
	if ( mc60_status.MQTT_READY == 1 && mc60_status.SIM_CART_INSERT == 1  ) LED_SIM(1);
	
}

char lost_i=8;
// Function to manage MC60 MQTT connection and SIM status
void mc60_mqtt_manage(){
	
	// Restart module mc60
	if( lost_i >= 8 ){ lost_i=0; 
		
      // Restart SIM module
      SIM_ON(0);
      osDelay(1000);
      SIM_ON(1);
      osDelay(1000);
      SIM_PWR(1);
      osDelay(1000);
      SIM_PWR(0);
			
			mc60_status.MQTT_READY=0; mc60_status.SIM_CART_INSERT=0;
		
      wait_to_get_sim("SMS", 20000);  // Wait for SIM card detection
      osDelay(2000);
	}
	
	// Send initialization AT commands
	sim_send_str("AT\n"); osDelay(100);
	sim_send_str("ATE1\n"); osDelay(100);
	// check sim card intert
	sim_send_str("AT+CPIN?\n");
	mc60_status.SIM_CART_INSERT = wait_to_get_sim("READY",5000); 
	osDelay(100);
	
	//advance.READY=1;
	if( mc60_status.SIM_CART_INSERT == 1 && advance.READY == 1 ){
			//sim_send_str("AT+QMTCLOSE=0\n");
			//osDelay(1000);
			//sim_send_str("AT+QMTOPEN=0,\"84.47.232.10\",\"1883\"\n");
			//sim_send_str("AT+QMTOPEN=0,\"109.125.149.108\",\"1883\"\n\r");
			sim_send_str("AT+QMTOPEN=0,\"ravis-gsm.ir\",\"1883\"\n\r");
			mc60_status.MQTT_READY = wait_to_get_sim("+QMTOPEN",5000);
		
			sim_send_str("AT+QMTOPEN? \n");
			mc60_status.MQTT_READY = wait_to_get_sim("++QMTOPEN: 0,0",5000);

			osDelay(500);
			
			if( mc60_status.MQTT_READY ){
				
					char buf_tx[100];
					sprintf(buf_tx,"AT+QMTCONN=0,\"clientExample%d\"\n",gsm.device_serial);
					sim_send_str(buf_tx);
					mc60_status.MQTT_READY = wait_to_get_sim("+QMTCONN: 0,0,0",5000);
					osDelay(1000);		
		
					sim_send_str("ATE0\n"); osDelay(100);
					osDelay(500);	
				
					sprintf(buf_tx,"server/%d",advance.SERIAL);
					mc60_mqtt_sub(buf_tx);				
			}

			int num=0;
			while( mc60_status.MQTT_READY == 1 ){
						
				// send to mqtt
				if( sim.F_data_for_server ){ sim.F_data_for_server=0;	
					
					char topic[50];
					sprintf(topic,"gsm/%d",advance.SERIAL);
					
					mc60_mqtt_pub(topic,modbus_slave.buf);
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
				
				// test_connection
				if( tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end ){ tbrc_s1[tbrc_s1_MC60_CONECTION_TEST].F_end=0;
					
						char topic[50];
						sprintf(topic,"gsm/%d",advance.SERIAL);
					
						char str[100];
						sprintf(str,"{\"serial\":\"%d\"}",advance.SERIAL);
										
						mc60_mqtt_pub(topic,str);
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
	
	//sim_send_str(data);
	//UART_PRINT(data,UART_SIM);
	
	int size = strlen(data);
	int i=0;
	for(i=0;i<size;i++){
		
		LL_USART_TransmitData8(USART2, data[i]);		 
		while(!LL_USART_IsActiveFlag_TXE(USART2));

	}

	
	//UART_PRINT(data,UART_SIM);
	//whit_to_responce_sim(1000);
	
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

