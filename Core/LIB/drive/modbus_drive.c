#include "main.h"
#include "modbus_drive.h"

		 /*modbus_drive_write_register_single(1,6,0x1000,fer);
		 osDelay(100);
		 modbus_drive_write_register_single(1,6,0x2000,panel);
		 osDelay(100);
		 //test_drive_modbus();
		 //modbus_drive_write_register_single(1,6,3,4);
*/

unsigned int speed=200;
unsigned int command=0;
char bit=0;

int qma_fer=1000;
int qma_com=0;
void test_drve_yaskawa(){
	
	MODBUS_ADVANCE_RS(0);
	//modbus_drive_write_register_single(1,0x08,0x100,100);
	//modbus_drive_write_register_single(1,0x06,0xE104,100);
//	modbus_drive_write_register_single(1,0x03,0x01,1);
	//modbus_drive_write_register_single(1,0x06,0x02,speed);
	
	//test_drive_modbus();
	
	//while(1){
	//	osDelay(100);
	//}
	
		//modbus_drive_write_register_single(1,0x06,0x0009,test_data);
		//modbus_drive_write_register_single(1,0x06,0x1000,qma_fer);	
//osDelay(100);		
		//modbus_drive_write_register_single(1,0x06,0x2000,qma_com);		
		
		//osDelay(100);
	//}
	
	
	//bit =  1 - bit;
	
	command=1;
	
	for( speed=500; speed<5000; speed+=15 ){
	
		modbus_drive_write_multi_register(1,1,2);
		osDelay(10);
	}
	
	command=1;
	
	for( speed=5000; speed>500; speed-=15 ){
	
		modbus_drive_write_multi_register(1,1,2);
		osDelay(10);
	}
	
	command=2;
	
	for( speed=500; speed<5000; speed+=15 ){
	
		modbus_drive_write_multi_register(1,1,2);
		osDelay(10);
	}
	
	command=2;
	
	for( speed=5000; speed>500; speed-=15 ){
	
		modbus_drive_write_multi_register(1,1,2);
		osDelay(10);
	}
	
	command=0;
	modbus_drive_write_multi_register(1,1,2);
	osDelay(3000);
	
	
}

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


char modbus_drive_write_multi_register(uint8_t slave_addres , uint16_t start_addres , uint16_t data){
	
	uint8_t tx_buf[20];  uint16_t tx_crc=0;
	
	slave_addres=1;
	start_addres = 0x01;
	data = 2;
	
	tx_buf[0] = slave_addres;
	tx_buf[1] = 0x10;	
	tx_buf[2] = start_addres >> 8 ; //start_addres H	
	tx_buf[3] = start_addres  ; //start_addres L		
	tx_buf[4] = data >> 8 ; //data H	
	tx_buf[5] = data  ; //data L	
	
	tx_buf[6] = 4  ; //data L	
	
	tx_buf[7] = command >> 8; 
	tx_buf[8] = command  ; 
	
	tx_buf[9] = speed >> 8  ; 
	tx_buf[10] = speed  ;

	tx_crc = CRC_16(tx_buf,11);
	
	tx_buf[11] = tx_crc ; //CRC H
	tx_buf[12] = tx_crc >> 8 ; //CRC L	
	
	char i=0;
	for(i=0;i<14;i++){
		MODBUS_ADVANCE_RS(1);
		UART_PUT_CHAR(tx_buf[i],UART_RS485);
		MODBUS_ADVANCE_RS(0);
	}
}

void test_drive_modbus(){
	
	//const char buf_tx[] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x0F, 0xC9, 0xCE};
	
	const char buf_tx[] = {0x01 ,0x10 ,0x00 ,0x01 ,0x00 ,0x02 ,0x04 ,0x00 ,0x01 , 0x27 ,0x10 , 0x79 , 0x9F };
	
	int size=sizeof(buf_tx);
		
	MODBUS_ADVANCE_RS(1);
	
	for(int i=0; i<size; i++)UART_PUT_CHAR(buf_tx[i],UART_RS485);
	
	MODBUS_ADVANCE_RS(0);
		

}

//01 06 00 00 00 0F C9 CE