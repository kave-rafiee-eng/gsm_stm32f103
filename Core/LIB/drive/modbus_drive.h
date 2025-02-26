
#ifndef __MODBUS_DRIVE_H
#define __MODBUS_DRIVE_H

#include "main.h"

char modbus_drive_write_register_single(uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t data);

char modbus_drive_write_multi_register(uint8_t slave_addres , uint16_t start_addres , uint16_t data);
	
void test_drive_modbus();

#endif

