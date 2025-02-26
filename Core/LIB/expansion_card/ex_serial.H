#ifndef __EX_SERIAL_H
#define __EX_SERIAL_H

#include "main.h"

#define	SERIAL_EXTENTION_SERIAL_ID			0xBB

#define TX_BUFFER_SIZE	200
#define RX_BUFFER_SIZE	200

#define	NO_AIM					255
#define MAX_NOPB				24
#define MAX_PB					120

void serial_ex( uint8_t DataIn );
void send_data_to_master();

#endif