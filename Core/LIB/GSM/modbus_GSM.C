#include "main.h"
#include "modbus_gsm.h"

// CPU TIMER
extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];


// MODBUS
extern struct MODBUS_SLAVE modbus_slave;
extern struct MODBUS_RTU modbus;

void modbus_it_uart_manage( char data ){
	
		modbus.buf_rx[modbus.buf_rx_index]=data;
		modbus.buf_rx_index++;
		if( modbus.buf_rx_index > 200 )modbus.buf_rx_index=0;
		
		tbr_g1[tbr_g1_MODBUS_SLAVE_RX].I_time=0;	
	
}

void modbus_slave_manager_recive(){
	
	if( modbus.buf_rx_index > 0 )tbr_g1[tbr_g1_MODBUS_SLAVE_RX].EN=1;
	else tbr_g1[tbr_g1_MODBUS_SLAVE_RX].EN=0;
	
	tbr_g1[tbr_g1_MODBUS_SLAVE_RX].C_set_time=10;
	
	if(tbr_g1[tbr_g1_MODBUS_SLAVE_RX].F_end){
		
		tbr_g1[tbr_g1_MODBUS_SLAVE_RX].F_end=0;
		
		modbus_slave_manage();
		
		memset(modbus.buf_rx,0,UART_BUF_SIZE);
		modbus.buf_rx_index=0;
	}	
	
}