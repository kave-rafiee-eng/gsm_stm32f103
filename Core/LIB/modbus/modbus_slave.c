#include "main.h"
#include "modbus.h"

extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

uint16_t slave_start_add =0;
uint8_t slave_data_get =0;
uint16_t slave_crc_get=0;
uint16_t slave_crc_calculate=0;
uint16_t slave_data_count=0;

char modbus_slave_manage(){
	
	if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_WRITE_TO_SLAVE_SINGLE == modbus.buf_rx[1]  ){ //single write
		
		slave_start_add = (modbus.buf_rx[2]<<8) + modbus.buf_rx[3];
		slave_data_get = modbus.buf_rx[4];
		slave_crc_get = (modbus.buf_rx[6]<<8) + modbus.buf_rx[5];
		
		slave_crc_calculate = CRC_16(modbus.buf_rx,5);
		if( slave_crc_calculate == slave_crc_get ){
				modbus_slave.address = slave_start_add;
				modbus_slave.data = slave_data_get;
				return 1;
		}
		else return -1;
	}
	else if( SLAVE_ADD == modbus.buf_rx[0] &&  FC_WRITE_TO_SLAVE_MULTI == modbus.buf_rx[1] ){ //multi write
		
		slave_start_add = (modbus.buf_rx[2]<<8) + modbus.buf_rx[3];
		slave_data_count = (modbus.buf_rx[4]<<8) + modbus.buf_rx[5];
		
		slave_crc_get = (modbus.buf_rx[slave_data_count+7]<<8) + modbus.buf_rx[slave_data_count+6];		
		slave_crc_calculate = CRC_16(modbus.buf_rx,6+slave_data_count);
		
		if( slave_crc_calculate == slave_crc_get ){
			
			memset(modbus_slave.buf,0,UART_BUF_SIZE);
			
			for(int i=0; i<slave_data_count; i++ ){			
					modbus_slave.buf[i] = modbus.buf_rx[i+6];
			}
			modbus_slave.data_count = slave_data_count;
			modbus_slave.address = slave_start_add;
			modbus_slave.F_new_data=1;
		}
		
	}
	else 0;
}

