#ifndef __CRC_H
#define __CRC_H

#include "main.h"



unsigned int CRC_16( uint8_t *buf,int len ){
	
	unsigned int crc = 0XFFFF;
	int pos=0;
	int i=0;
	
	for( pos=0; pos<len; pos++)
	{
		
			crc^=(unsigned int)buf[pos];
			
			for(i=8;i!=0;i--)
			{
					if( (crc & 0x0001)!=0 )
					{	
							crc>>=1;
							crc^=0xA001;	
					}
					else crc>>=1;	
			}
	}	
	return crc;
	
}



#endif