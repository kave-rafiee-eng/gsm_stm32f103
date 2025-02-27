#ifndef __TEST_H
#define __TEST_H

#include "main.h"

#define TIME_EN_USER_SEND 15
#define COUNTER_EN_USER_SEND 5

void MAIN_communication();
void read_json_advance();

struct GSM
{
	uint16_t device_serial ;
	uint16_t F_send_EN_USER ;
	uint16_t COUNTER_timer_EN_USER ;
	
};

#endif /* __TEST_H */