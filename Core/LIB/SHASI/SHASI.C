#include "main.h"
#include "shasi.h"

extern char can_flag_RECIVE;

extern struct cpu_timer_basic_10bit_auto_reset tbr_g1[def_num_tbr_g1];

ModbusFrame txFrame;
ModbusFrame RXframe;

extern struct MODBUS_RTU modbus;

extern CAN_HandleTypeDef hcan;
	
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;

extern uint32_t TxMailbox;
extern uint8_t TxData[8];
extern uint8_t RxData[8];

void test_shasi(){
	
	
	if( modbus.buf_rx_index > 0 )tbr_g1[tbr_g1_MODBUS_SLAVE_RX].EN=1;
	else tbr_g1[tbr_g1_MODBUS_SLAVE_RX].EN=0;
	
	tbr_g1[tbr_g1_MODBUS_SLAVE_RX].C_set_time=1;
	
	if(tbr_g1[tbr_g1_MODBUS_SLAVE_RX].F_end){tbr_g1[tbr_g1_MODBUS_SLAVE_RX].F_end=0;
		
	  RXframe = Modbus_ReceiveFrame( (uint8_t*)modbus.buf_rx, modbus.buf_rx_index);
		
		if (RXframe.isValid) {
		
				TxHeader.IDE = CAN_ID_EXT;
				TxHeader.RTR = CAN_RTR_DATA;
				TxHeader.DLC = 8;
				TxHeader.ExtId= RXframe.deviceAddress;		
				
				HAL_CAN_AddTxMessage(&hcan, &TxHeader, RXframe.data, &TxMailbox);
							
		}
		
		//memset(modbus.buf_rx,0,UART_BUF_SIZE);
		modbus.buf_rx_index=0;
	}	
	
	
	if(  can_flag_RECIVE ){ can_flag_RECIVE=0;
		
		txFrame.dataLength = 8;
		txFrame.functionCode = 0x10;
		txFrame.deviceAddress = RxHeader.ExtId;
		
		txFrame.data[0] = RxData[0]; //set
		txFrame.data[1] = RxData[1]; //cansel
		
		Modbus_SendFrame(txFrame.deviceAddress, txFrame.functionCode, txFrame.data, txFrame.dataLength);
					
	}

	
}






