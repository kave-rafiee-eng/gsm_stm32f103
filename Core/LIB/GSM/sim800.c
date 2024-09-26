#include "main.h"
#include "sim800.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];
extern struct cpu_timer_8bit_reset_contorol_Seconds tbrc_s1[def_num_tbrc_s1];


struct ESP8266 sim;
struct ESP8266_MANAGE sim_manage;

struct UART_DATA sim_uart_buffer;

struct SIM800_STATUS sim800_status;

void sim800_led_show(){
	
	if ( sim800_status.MQTT_READY == 0 && ( sim800_status.SIM_CART_INSERT == 1 ) ) HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
		
	if ( sim800_status.SIM_CART_INSERT != 1 ) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,0);
	
	if ( sim800_status.MQTT_READY == 1 && sim800_status.SIM_CART_INSERT == 1  ) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,1);
	
}

void sim800_turn_on_internet(){
	
	sim800_status.MQTT_READY=0;
	sim800_status.SIM_CART_INSERT=0;
	
	sim_send_str("AT\n");
	
	sim_send_str("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\n");

	sim_send_str("AT+SAPBR=3,1,\"APN\",\"mcinet\"\n");
	//sim_send_str("AT+SAPBR=3,1,\"APN\",\"RighTel\"\n");

	sim_send_str("AT+SAPBR=1,1\n");

	sim_send_str("AT+SAPBR=2,1\n");
	
	sim_send_str("AT+cpin?\n");
	sim800_status.SIM_CART_INSERT = wait_to_get_sim("READY",4000); 
}

void sim_send_str(char *str){
	
	clear_sim_buffer();
	UART_PRINT(str,UART_SIM);
	whit_to_responce_sim(4000);
}

char wait_to_get_sim( char *buffer , int time_out ){	
	int time=0;
	while( 1 ){	
			if( strfind((char*)sim_uart_buffer.BUF,buffer) > 0 )return 1;
			osDelay(1);
			time++;
		
			if( strfind((char*)sim_uart_buffer.BUF,"ERROR") > 0 )return 0;
			if( time > time_out )return -1;
	}
}

void whit_to_responce_sim( int time_out ){
		
	int time=0;
	while( tbr_g1[tbr_g1_SIM_UART_GET].F_end == 0 ){
		osDelay(1);
		time++;
		if( time > time_out )break;
	}
	tbr_g1[tbr_g1_SIM_UART_GET].F_end=0;
	tbr_g1[tbr_g1_SIM_UART_GET].EN=0;
	
}


void sim_uart_rx_manager(char data){
	
	if( data == '{' )sim_manage.json_level=1;

		if( sim_manage.json_level == 1 ){	
			sim.BUF_JSON[sim.BUF_JSON_index] = data;
			sim.BUF_JSON_index++;
			if( data == '}' ){ sim_manage.json_level=2;
				
				if( sim.BUF_JSON_index > 3 )sim.F_json_get=1;
				else{
						sim.BUF_JSON_index=0;
						sim_manage.json_level=0;
				}
				
			}
		}
		
		if( sim.BUF_JSON_index >= UART_BUF_SIZE )sim.BUF_JSON_index=0;
		
		tbr_g1[tbr_g1_SIM_UART_GET].EN=1;
		tbr_g1[tbr_g1_SIM_UART_GET].I_time=0;
		tbr_g1[tbr_g1_SIM_UART_GET].C_set_time=5;
		
		sim_uart_buffer.BUF[sim_uart_buffer.BUF_I] = data;
		sim_uart_buffer.BUF_I++;
		
		if( sim_uart_buffer.BUF_I >= UART_BUF_SIZE )sim_uart_buffer.BUF_I=0;
}

void clear_sim_buffer(){
	
	memset(sim_uart_buffer.BUF,0,UART_BUF_SIZE);
	sim_uart_buffer.BUF_I=0;
}

void clear_sim_buffer_json(){
	
	memset(sim.BUF_JSON,0,UART_BUF_SIZE);
	
	sim_manage.json_level=0;
	sim.F_json_get=0;
	sim.BUF_JSON_index=0;
	
}








