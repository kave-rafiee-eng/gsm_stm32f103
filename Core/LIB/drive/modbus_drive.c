#include "main.h"
#include "modbus_drive.h"


char modbus_drive_write_register_single(uint8_t slave_addres , uint8_t fc_code , uint16_t start_addres , uint16_t data){
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = fc_code;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = data >> 8 ; //data H	
	tx_buf[5] = data  ; //data L	

	tx_crc = CRC_16(tx_buf,6);
	
	tx_buf[6] = tx_crc ; //CRC H
	tx_buf[7] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<9;i++){
		MODBUS_ADVANCE_RS(1);
		UART_PUT_CHAR(tx_buf[i],UART_RS485);
		MODBUS_ADVANCE_RS(0);
	}
}

void test_drive_modbus(){
	
	//const char buf_tx[] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x0F, 0xC9, 0xCE};
	
	const char buf_tx[] = {0x01 ,0x06 ,0x00 ,0x02 ,0x00 ,0x01 ,0xE9 ,0xCA};
	
	int size=sizeof(buf_tx);
		
	MODBUS_ADVANCE_RS(1);
	
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_RS485);
	
	MODBUS_ADVANCE_RS(0);
		

}

//01 06 00 00 00 0F C9 CE