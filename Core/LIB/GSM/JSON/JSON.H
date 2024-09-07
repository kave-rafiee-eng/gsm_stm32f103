
#ifndef __JSON_H
#define __JSON_H

#include "main.h"

#define TYPE_STR 1
#define TYPE_WORD 2

int strfind(char *str,char *substr);
int json_find(char *buffer_uart,char *command);
char json_get_data( char *buffer , char *command );
void json_get_data_dma();
void read_protocol_json();
void reset_json();
void json_get_document( char *buffer , char *counter  );

struct JSON_OUT 
{
	char document[UART_BUF_SIZE];
  char str_data[100];
	int data;
};

struct JSON_PROTOCOL 
{
	char name_w1[50];
  char data_w1[50];
	int data_w1_word;
	
	char data_w1_type : 3;
	
	char name_r1[50];
  char data_r1[50];
	int data_r1_word;
	
	char data_r1_type : 3;
	
};

#endif /* __JSON_H */