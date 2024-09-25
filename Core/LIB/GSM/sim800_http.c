
/*
void http_read(){

	UART_STDOUT_SELECT = UART_SIM;
	char str[100];

	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPINIT");
	puts(str);
	whit_to_responce_sim();
					
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPPARA=\"CID\",1");
	puts(str);
	whit_to_responce_sim();
		
	if( json_get_data(json.document , "\"url\":") == TYPE_STR ){
			clear_sim_uart_buffer();
			sprintf(str,"AT+HTTPPARA=\"URL\",\"www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100&%s\"",json.str_data);
			puts(str);
			whit_to_responce_sim();				
	}
	else{
		
			clear_sim_uart_buffer();
			sprintf(str,"AT+HTTPPARA=\"URL\",\"www.ravis-gsm.ir/GSM_RAVIS/gsm_connection.php?serial=100\"");
			puts(str);
			whit_to_responce_sim();		
	}
							
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPACTION=0");
	puts(str);
	if( wait_to_get_sim("HTTPACTION:",4000) ==  1 ){}
		
		clear_sim_uart_buffer();
		sprintf(str,"AT+HTTPREAD");
		puts(str);
		whit_to_responce_sim();		
	
						
	clear_sim_uart_buffer();
	sprintf(str,"AT+HTTPTERM");
	puts(str);
	whit_to_responce_sim();
	
	
}
*/
