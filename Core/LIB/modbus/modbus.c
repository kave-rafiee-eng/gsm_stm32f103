#include "main.h"
#include "modbus.h"

struct MODBUS_RTU modbus;
struct MODBUS_SLAVE modbus_slave;

extern struct 	UART_DATA esp_data;
 uint16_t tx_crc=0;
char modbus_master_write_register_MULTI( uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t world_count , uint8_t *data ){
	
	uint8_t tx_buf[world_count+20]; 
	int i=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = world_count >> 8 ; //world_count H	
	tx_buf[5] = world_count  ; //world_count L	

	for( i=0; i<world_count; i++ ){
		
		tx_buf[6+i] = data[i];
	}
	
	tx_crc = CRC_16(tx_buf,6+world_count);
	
	tx_buf[6+world_count] = tx_crc ; //CRC H
	tx_buf[7+world_count] = tx_crc >> 8 ; //CRC L	
	
	for(i=0;i<world_count+8;i++){
		MODBUS_ADVANCE_RS(1);
			stdout_putchar(tx_buf[i]);
		MODBUS_ADVANCE_RS(0);
	}	
	
}

char modbus_master_write_register_SINGLE(uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint8_t data){
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = data  ; //data L	

	tx_crc = CRC_16(tx_buf,5);
	
	tx_buf[5] = tx_crc ; //CRC H
	tx_buf[6] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<7;i++){
		MODBUS_ADVANCE_RS(1);
			stdout_putchar(tx_buf[i]);
		MODBUS_ADVANCE_RS(0);
	}
}




uint16_t crc_rx_check; uint8_t crc_rx_check_h;  uint8_t crc_rx_check_l;

char modbus_master_read_reginter( uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t world_count , uint16_t *return_data ){

	esp_data.BUF_I=0;
	//------------------------------------------------ SEND
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = world_count >> 8 ; //world_count H	
	tx_buf[5] = world_count  ; //world_count L	

	tx_crc = CRC_16(tx_buf,6);
	
	tx_buf[6] = tx_crc ; //CRC H
	tx_buf[7] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<8;i++){
		stdout_putchar(tx_buf[i]);
	}
	
	//------------------------------------------------ RECIVE
	
	if( modbus_time_out_get() ){}
		else return 1;
	
	HAL_Delay(10);
		
	for( char i=0; i<esp_data.BUF_I; i++ ){
		modbus.buf_rx[i]= esp_data.BUF[i];
	}
		// send :01 02 03 04
	// 01 02 01
	if( slave_addres == modbus.buf_rx[0] &&  fc_code == modbus.buf_rx[1] && modbus.buf_rx[2] == (world_count*2) ){
		
		  
		
		crc_rx_check = CRC_16( modbus.buf_rx , modbus.buf_rx[2]+3 );
		
		crc_rx_check_h = crc_rx_check>>8;
		crc_rx_check_l = crc_rx_check;

		if( modbus.buf_rx[modbus.buf_rx[2]+3] == crc_rx_check_l && modbus.buf_rx[modbus.buf_rx[2]+4] == crc_rx_check_h ){
			
			char i=0;
			for(i=0; i<world_count; i++ ){
					
				//*(return_data+i) = ( modbus.buf_rx[3+(i*2)] << 8 | modbus.buf_rx[4+(i*2)] );
				return_data[i] = ( modbus.buf_rx[3+(i*2)] << 8 | modbus.buf_rx[4+(i*2)] );
				
			}
			return 0;
		}	
	}
	return 2;	
}


void modbus_master_1ms(){
	
	if( modbus.TIME_OUT > 0 )modbus.TIME_OUT ++;
	
}

void modbus_master_read_reginter_it(){
	
	modbus.buf_rx[ modbus.buf_rx_index ] = LL_USART_ReceiveData8(USART2);	
	
	modbus.buf_rx_index ++;
	
}

char modbus_time_out_get(){
	
	/*modbus.buf_rx_index=0;  modbus.TIME_OUT=1;
	
	while( modbus.TIME_OUT > 0 ){	
			if( modbus.buf_rx_index > 0 )return 1;
			if( modbus.TIME_OUT > 100 ) return 0;
	}*/
	
	while( esp_data.BUF_I == 0 ){
		HAL_Delay(10);
	}
	
}



char start=0;
char error=0;
/*
void modbus_test(){
	
	if( start == 1 ){
		
		error = modbus_master_read_reginter(1,4,1,5,modbus.buf_data);
		//modbus_write_register_single();
	}
	
	start=0;
	
}*/

/*
char modbus_slave_manage( char buf_data){
	
	if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_READ_WRITE_TO_SLAVE_SINGLE == modbus.buf_rx[1]  ){
		
		slave_start_add = (modbus.buf_rx[2]<<8) + modbus.buf_rx[3];
		slave_data_get = (modbus.buf_rx[4]<<8) + modbus.buf_rx[5];
		slave_crc_get = (modbus.buf_rx[7]<<8) + modbus.buf_rx[6];
		
		slave_crc_calculate = CRC_16(modbus.buf_rx,6);
		if( slave_crc_calculate == slave_crc_get ){
				modbus_slave.address = slave_start_add;
				modbus_slave.data = slave_data_get;
				return 1;
		}
		else return -1;
	}
	else if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_READ_FROM_SLAVE == modbus.buf_rx[1] ){
		
	}
	else 0;
}
*/