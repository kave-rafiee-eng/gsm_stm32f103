
#ifndef __ADVANCE_H
#define __ADVANCE_H

#include "main.h"

void ADVANCE_led_status();

void advance_connection_test();
char wait_to_advance_get_data( int time_out );
void manage_advance_responce();
void advance_manager();

struct ADVANCE 
{
	char READY : 1;
	char ERROR_NOT_RESPONCE : 1;
	int SERIAL ;

};

#endif /* __ADVANCE_H */