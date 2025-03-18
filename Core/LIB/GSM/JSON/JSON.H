
#ifndef __JSON_H
#define __JSON_H

#include "main.h"

#define TYPE_STR 1
#define TYPE_WORD 2

void server_protocol_json();

int strfind(char *str,char *substr);
int json_find(char *buffer_uart,char *command);
char json_get_data( char *buffer , char *command );
void reset_json();
void json_get_data_dma();
int replacechar(char *str, char orig, char rep);
void server_protocol_json();

struct JSON_OUT 
{
	char document[UART_BUF_SIZE];
  char str_data[100];
	int data;
};


#endif /* __JSON_H */