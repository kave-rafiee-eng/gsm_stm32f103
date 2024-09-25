
#ifndef __SIM800_H
#define __SIM800_H

#include "main.h"

void sim_send_str(char *str);
void sim800_turn_on_internet();
void clear_sim_buffer();
void sim_uart_rx_manager(char data);
void whit_to_responce_sim( int time_out );
char wait_to_get_sim( char *buffer , int time_out );
void sim_send_str(char *str);
	
#endif /* __SIM800_H */