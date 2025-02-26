#include "main.h"
#include "expansion.h"

extern CAN_HandleTypeDef hcan;
	
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

char can_flag=0;
char can_flag_RECIVE=0;

int can_recive;
int can_last_id;

uint8_t TxData[8];
uint8_t RxData[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	can_flag=1;
	can_flag_RECIVE=1;
	
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
	
	can_recive++;
	
	can_last_id = RxHeader.StdId;
	
}

uint8_t data[16];

uint8_t DLC;

uint8_t id[4];

uint32_t TxMailbox;

void test_can(){
	
	TxHeader.IDE = CAN_ID_EXT;
	TxHeader.RTR = CAN_RTR_DATA;
	

	id[0]=0;	id[1]=0;	id[2]=0;	id[3]=2;
	DLC=8;
	
	TxHeader.DLC = DLC;
	//TxHeader.ExtId= (id[3]*4096) +  (id[2]*256) + (id[1]*16)+ id[0] ;
	TxHeader.ExtId= 10;
	/*for( int i=0; i<16; i++){
		data[i] = i;
	}*/
	
	for( int i=0; i<8; i++){
		
		//TxData[i] = (data[i*2+1]*16) + data[i*2];
		TxData[i] = data[i];
	}
	
	
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
	
}
