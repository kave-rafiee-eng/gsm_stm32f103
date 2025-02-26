
#include "main.h"
#include "ex_serial.H"

volatile	uint8_t	Slave_PacketCounter,Slave_SyncStatus,Slave_PacketLen,Slave_ReceivedByte[50],Slave_GetDataCounter,Slave_DataCount;

volatile 	uint8_t 		PB_Blinker[6],PB_Set,PB_Clear,PB_SetShow,PB_ClearShow;
volatile	uint16_t		Slave_RXDataCNT,Master_RXDataCNT,Slave_GetDataTimer,Master_GetDataTimer,Slave_PacketError,Master_PacketError;

volatile	uint8_t			NumR=0,NumL=0;

volatile	char				Slave_NewPacketF=0,Master_NewPacketF=0,Slave_SendDataToMasterF=0;
volatile	uint8_t			Slave_TX_Buffer[TX_BUFFER_SIZE];

volatile	char				Slave_TxModeF=0,Slave_ReceivingF=0,Master_TxModeF=0,Master_ReceivingF=0;

volatile	uint8_t			SlavePacketCounter,MasterPacketCounter;

volatile 	char				PB[MAX_PB],L_PB[MAX_NOPB],PB_Cancel[MAX_PB],BlinkPBSt=1,Segments[16];

void serial_ex( uint8_t DataIn ){
	
	//	while(SLAVE_RX_DMA_CNT != Slave_RXDataCNT)
	//{
		
		if(DataIn == 0xFF && Slave_SyncStatus == 0) 			{Slave_PacketLen = 0;	
			
			Slave_SyncStatus++;
		
		}
		else if(Slave_SyncStatus == 0)										{Slave_PacketLen = 0;}
		else if(DataIn == 0xFE && Slave_SyncStatus == 1) 	{Slave_PacketLen = 0;	Slave_SyncStatus++;}
		else if(Slave_SyncStatus == 2)
		{
			if(DataIn == SERIAL_EXTENTION_SERIAL_ID) 				{Slave_PacketLen = 0; Slave_SyncStatus++;}
			else {Slave_SyncStatus = 0;} //ID doesn't match
		}
		else if(Slave_SyncStatus == 3) {Slave_PacketLen = DataIn;	Slave_SyncStatus++; Slave_PacketCounter = 0;}
		else if(Slave_SyncStatus < Slave_PacketLen && Slave_SyncStatus > 3)
		{
			Slave_ReceivedByte[Slave_PacketCounter] = DataIn;
			Slave_PacketCounter++;
			Slave_SyncStatus++;
		}
		else
		{
			if(Slave_SyncStatus == Slave_PacketLen && Slave_PacketLen > 0)
			{
				uint8_t	Cheksum = Slave_ReceivedByte[0];
				for(uint16_t iw=1;iw < Slave_PacketLen;iw++) 
				{
					Cheksum ^= Slave_ReceivedByte[iw];
				}
				
				if(DataIn == Cheksum ) 
				{
					Slave_NewPacketF = 1;
					Slave_GetDataCounter ++;
					Slave_GetDataTimer = 0;
				}
			}
			
			if(Slave_NewPacketF == 0) {Slave_PacketError ++;}
			Slave_SyncStatus = 0;
			Slave_PacketLen = 0;
		}
		
		
		if(Slave_NewPacketF)
		{
			
			NumR = Slave_ReceivedByte[0] ;
			NumL = Slave_ReceivedByte[1] ;
			
			PB_Blinker[0] = Slave_ReceivedByte[2];
			PB_Blinker[1] = Slave_ReceivedByte[3];
			PB_Blinker[2] = Slave_ReceivedByte[4];
			PB_Blinker[3] = Slave_ReceivedByte[5];
			PB_Blinker[4] = Slave_ReceivedByte[6];
			PB_Blinker[5] = Slave_ReceivedByte[7];
			
			if(Slave_ReceivedByte[8] == Slave_ReceivedByte[9]) 		PB_Set = Slave_ReceivedByte[8];
			if(Slave_ReceivedByte[10] == Slave_ReceivedByte[11]) 	PB_Clear = Slave_ReceivedByte[10];
			
			//if(PB_Set != NO_AIM) 		{PB_SetShow = PB_Set; PB_SetShowTimer = 200;}
			//if(PB_Clear != NO_AIM) 	{PB_ClearShow = PB_Clear; PB_ClearShowTimer = 200;}
			
			uint8_t Data = Slave_ReceivedByte[12];
			
			/*StandbyF 	= Slave_ReceivedByte[12] & 0x01;
			RunOrStop	= Slave_ReceivedByte[12] & 0x02;
			LandingF 	= Slave_ReceivedByte[12] & 0x04;*/
			
			Slave_NewPacketF = 0;
			
			Slave_SendDataToMasterF = 1;
			
			SlavePacketCounter++;
			
			send_data_to_master();
		}
		
	//}
	
}

char tx4=0;
void send_data_to_master(){
	
	
	if(Slave_SendDataToMasterF == 1 && Slave_ReceivingF == 0) //There is 20ms rest time between both packet
	{
		//Slave_TX_AfterLastByteTimer = 0;
		
		for(int i=0;i<50;i++) Slave_TX_Buffer[i] = 0;
		
		Slave_TX_Buffer[0] = 0xFF;
		Slave_TX_Buffer[1] = 0xFE;
		Slave_TX_Buffer[2] = SERIAL_EXTENTION_SERIAL_ID;
		uint8_t PacketLen = 36; //(0xFF to Checksum)
		Slave_TX_Buffer[3] = PacketLen; //Packet Len
			
		//Start Packet
		for(int i=7;i>=0;i--)
		{
			Slave_TX_Buffer[4] = tx4;
			//Slave_TX_Buffer[4] |= PB[i];
			Slave_TX_Buffer[5] |= PB[i + 8];
			Slave_TX_Buffer[6] |= PB[i + 16];
			Slave_TX_Buffer[7] |= PB[i + 24];
			Slave_TX_Buffer[8] |= PB[i + 32];
			Slave_TX_Buffer[9] |= PB[i + 40];
			Slave_TX_Buffer[10]|= PB[i + 48];
			Slave_TX_Buffer[11]|= PB[i + 56];
			Slave_TX_Buffer[12]|= PB[i + 64];
			Slave_TX_Buffer[13]|= PB[i + 72];
			Slave_TX_Buffer[14]|= PB[i + 80];
			Slave_TX_Buffer[15]|= PB[i + 88];
			Slave_TX_Buffer[16]|= PB[i + 96];
			Slave_TX_Buffer[17]|= PB[i + 104];
			Slave_TX_Buffer[18]|= PB[i + 112];
			
			Slave_TX_Buffer[19]|= PB_Cancel[i];
			Slave_TX_Buffer[20]|= PB_Cancel[i + 8];
			Slave_TX_Buffer[21]|= PB_Cancel[i + 16];
			Slave_TX_Buffer[22]|= PB_Cancel[i + 24];
			Slave_TX_Buffer[23]|= PB_Cancel[i + 32];
			Slave_TX_Buffer[24]|= PB_Cancel[i + 40];
			Slave_TX_Buffer[25]|= PB_Cancel[i + 48];
			Slave_TX_Buffer[26]|= PB_Cancel[i + 56];
			Slave_TX_Buffer[27]|= PB_Cancel[i + 64];
			Slave_TX_Buffer[28]|= PB_Cancel[i + 72];
			Slave_TX_Buffer[29]|= PB_Cancel[i + 80];
			Slave_TX_Buffer[30]|= PB_Cancel[i + 88];
			Slave_TX_Buffer[31]|= PB_Cancel[i + 96];
			Slave_TX_Buffer[32]|= PB_Cancel[i + 104];
			Slave_TX_Buffer[33]|= PB_Cancel[i + 112];
			
			if(i > 0)
			{
				for(uint8_t j=4;j<34;j++)
				{
					Slave_TX_Buffer[j] <<=1;
				}
			}
		}
		
		uint8_t Data=0;
		/*if(HwIF1) 	Data |= 0x80;
		if(HwIF1) 	Data |= 0x40;
		if(HwI1) 		Data |= 0x20;
		if(HwI2) 		Data |= 0x10;
		if(HwI3) 		Data |= 0x08;
		if(HwI4) 		Data |= 0x04;
		if(HwI5) 		Data |= 0x02;
		if(HwI68)		Data |= 0x01;*/
		Slave_TX_Buffer[34] = Data;
		
		uint8_t Checksum = Slave_TX_Buffer[4];
		for(uint8_t i=5;i<(Slave_TX_Buffer[3] - 1);i++) {Checksum ^= Slave_TX_Buffer[i];}
		Slave_TX_Buffer[PacketLen - 1] = Checksum;
		Slave_TX_Buffer[PacketLen] = 0;
		
		MODBUS_ADVANCE_RS(1); //Turn off in TIM17 100uS interrupt routine
		
		Slave_TxModeF = 1;
		
		int i=0;
		for(i=0;i<PacketLen+2;i++){
			
			LL_USART_TransmitData8(USART1, Slave_TX_Buffer[i]);		 
			while(!LL_USART_IsActiveFlag_TXE(USART1));		

		}
		
		MODBUS_ADVANCE_RS(0);
		
		Slave_SendDataToMasterF = 0;
	}	
	
}
