#include "main.h"
#include "mc60_mqtt.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];

extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;

extern struct UART_DATA sim_uart_buffer;

struct SIM800_STATUS mc60_status;

void mc60_mqtt_pub( char *topic , char *data);
void mc60_mqtt_sub(char *topic);
	
void mv60_led_show(){
	
	if ( mc60_status.MQTT_READY == 0 && ( mc60_status.SIM_CART_INSERT == 1 ) ) LED_SIM_TOGGLE();
		
	if ( mc60_status.SIM_CART_INSERT != 1 ) LED_SIM(0);
	
	if ( mc60_status.MQTT_READY == 1 && mc60_status.SIM_CART_INSERT == 1  ) LED_SIM(1);
	
}

void mc60_mqtt_manage(){

	/*SIM_ON(0);
	osDelay(1000);
	SIM_ON(1);
	mc60_status.SIM_CART_INSERT = wait_to_get_sim("SMS Ready",10000); */
	

	sim_send_str("at+cfun=1,1\n");
	wait_to_get_sim("SMS",20000); 
	
	osDelay(2000);
	
	sim_send_str("AT+cpin?\n");
	mc60_status.SIM_CART_INSERT = wait_to_get_sim("READY",5000); 
	osDelay(100);
	
	if( mc60_status.SIM_CART_INSERT ){
		
			//sim_send_str("AT+QMTCLOSE=0\n");
			//osDelay(1000);
			
			sim_send_str("AT+QMTOPEN=0,\"84.47.232.10\",\"1883\"\n");
			mc60_status.MQTT_READY = wait_to_get_sim("+QMTOPEN",5000);
			osDelay(1000);
			
			if( mc60_status.MQTT_READY ){
				
					sim_send_str("AT+QMTCONN=0,\"clientExample\"\n");
					mc60_status.MQTT_READY = wait_to_get_sim("+QMTCONN",5000);
					osDelay(1000);		
				
					mc60_mqtt_sub("server");				
			}

			int num=0;
			while( mc60_status.MQTT_READY == 1 ){
						
				char buf_tx[100];
				sprintf(buf_tx,"rafiee/%d\n",num);
				num++;
				
				//+QMTPUB
				mc60_mqtt_pub("top_test",buf_tx);					
				osDelay(1000);
				
			}		
	}
		
}

void mc60_start_mqtt(){
	
	
}

void mc60_mqtt_pub( char *topic , char *data){
	
	char buf_tx[100];
	sprintf(buf_tx,"AT+QMTPUB=0,0,0,0,\"%s\"\n",topic);
	sim_send_str(buf_tx);
	osDelay(5);
	
	sim_send_str(data);
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

