#include "main.h"
#include "mqtt_tcp.h"

void mqtt_tcp_packet_pub( char *topic , char *data );
void mqtt_tcp_packet_sub( char *topic  );
void mqtt_tcp_start();
void mqtt_ping();

// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

char server_connect=0;

extern struct UART_DATA sim_uart_buffer;

extern struct ESP8266 sim;
extern struct ESP8266_MANAGE sim_manage;

void SIM_MQTT(){
	

	if( server_connect == 0 ){
		
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,0);
			
			osDelay(200);
		
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,1);
		
			osDelay(200);
			sim_send_str("ATE0\n");
			osDelay(8000);
			
			//whit_to_responce_sim(20000);
		
	}
	
	sim800_turn_on_internet();
	mqtt_tcp_start();
	
	
	if( server_connect ){ 
		
		sim_send_str("ATE0\n");
		mqtt_tcp_packet_pub("gsm","start");
		osDelay(500);
		mqtt_tcp_packet_sub("server");	
		
		osDelay(500);

	}
	
	int i=0;
	while( server_connect == 1 ){
		
				/*char str[50];
				sprintf(str,"data%d",i);
				mqtt_tcp_packet_pub("server",str);
				
				osDelay(500);*/
		
				osDelay(1);

				i++;
		
				if( i > 3000 ){ i=0; mqtt_ping(); }
				
				if( sim.F_data_for_server ){ sim.F_data_for_server=0;
					i=0;
					
					mqtt_tcp_packet_pub("gsm",modbus_slave.buf);
					//mqtt_tcp_packet_pub("server","data");
					
				}
						
	}
	
	
	//sim_send_str("AT+SAPBR=0,1\n");

}

void mqtt_ping(){
	
	sim_send_str("AT+CIPSEND\n");
	osDelay(100);
	
	const char buf_tx[] = {0xC0,0x00};
	
	int size=sizeof(buf_tx);
		
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_SIM);
	UART_PUT_CHAR(26,UART_SIM);
		
	whit_to_responce_sim(4000);	
	
	if( strfind(sim_uart_buffer.BUF,"OK") > 0 ){ server_connect=1; }
	if( strfind(sim_uart_buffer.BUF,"ERROR") > 0 ){ server_connect=0; }
}

void mqtt_tcp_start(){
	
	osDelay(1000);

	sim_send_str("AT+CIPSTART=\"TCP\",\"5.198.176.233\",\"1883\"\n");
	
	server_connect = wait_to_get_sim("CONNECT",10000);

	
	osDelay(1000);
	
	mqtt_tcp_packet_connect();
	
	osDelay(1000);	
}

void mqtt_tcp_packet_connect(){
	
	sim_send_str("AT+CIPSEND\n");
	osDelay(100);
	
	const char buf_tx[] = {0x10 ,0x2C ,0x00 ,0x06 ,0x4D ,0x51 ,0x49 ,0x73 ,0x64 ,0x70 ,0x03 ,0xC2 ,0x00 ,0x3C ,0x00 ,0x06 ,0x41 ,0x42 ,0x43 ,0x44 ,0x45 ,0x46 ,0x00 ,0x08 ,0x64 ,0x78 ,0x78 ,0x6B ,0x67 ,0x6B ,0x70 ,0x70 ,0x00 ,0x0C ,0x71 ,0x41 ,0x55 ,0x5A ,0x42 ,0x64 ,0x61 ,0x53 ,0x49 ,0x55 ,0x4C ,0x78};
	
	int size=sizeof(buf_tx);
		
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_SIM);
	UART_PUT_CHAR(26,UART_SIM);
		
	whit_to_responce_sim(4000);
		
	
}

//-----------------****************************************

void mqtt_tcp_packet_pub( char *topic , char *data ){
	
	sim_send_str("AT+CIPSEND\n");
	osDelay(100);
	
//	const char buf_tx[] = {0x30 ,0x13 ,0x00 ,0x08 ,0x76 ,0x61 ,0x6C ,0x65 ,0x74 ,0x72 ,0x6F ,0x6E ,0x68 ,0x65 ,0x6C ,0x6C ,0x6F ,0x72 ,0x61 ,0x76 ,0x69};
	
	unsigned char buf_tx[200] = {0x30 ,0x13 ,0x00 };
	
	buf_tx[3] = strlen(topic);
	
	for( int i=0; i<strlen(topic); i++ ){
		buf_tx[i+4] = topic[i];
	}
	
	for( int i=0; i<strlen(data); i++ ){
		buf_tx[i+4+strlen(topic)] = data[i];
	}
		
	int size=strlen(topic)+ strlen(data) +4;
		
	//int size=sizeof(buf_tx);
	
	buf_tx[1] = size-2;
		
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_SIM);
	UART_PUT_CHAR(26,UART_SIM);
	
	whit_to_responce_sim(4000);
	
	if( strfind(sim_uart_buffer.BUF,"OK") > 0 ){ server_connect=1; }
	if( strfind(sim_uart_buffer.BUF,"ERROR") > 0 ){ server_connect=0; }
	
}


void mqtt_tcp_packet_sub( char *topic  ){
	
	sim_send_str("AT+CIPSEND\n");
	osDelay(100);
	
	//const char buf_tx[] = {0x82, 0x0d, 0x00, 0x01, 0x00, 0x08, 0x76, 0x61, 0x6c, 0x65, 0x74, 0x72, 0x6f, 0x6e, 0x00};

	char buf_tx[200] = {0x82, 0x0d, 0x00, 0x01, 0x00 };
	
	buf_tx[5] = strlen(topic);
	
	for( int i=0; i<strlen(topic); i++ ){
		buf_tx[i+6] = topic[i];
	}
	
	buf_tx[strlen(topic)+7] = 0;
	int size=strlen(topic)+7;
		
	buf_tx[1] = size-2;
	
	//int size=sizeof(buf_tx);
		
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_SIM);
	UART_PUT_CHAR(26,UART_SIM);
	
	whit_to_responce_sim(4000);
	
}