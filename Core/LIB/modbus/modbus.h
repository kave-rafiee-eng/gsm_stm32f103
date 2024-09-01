#ifndef __MODBUS_H
#define __MODBUS_H

#include "main.h"

void modbus_master_1ms();
char modbus_time_out_get();
void modbus_master_read_reginter_it();
char modbus_master_read_reginter( uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t world_count , uint16_t *return_data );
void modbus_test();

struct MODBUS_RTU {
	
	uint8_t TIME_OUT;
	
	volatile uint8_t buf_rx[50];
	volatile uint8_t buf_rx_index;
	
	int16_t buf_data[10];
	
};


#endif