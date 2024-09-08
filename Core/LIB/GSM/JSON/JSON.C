#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "json.h"


struct JSON_OUT	 json;
struct JSON_PROTOCOL json_protocol;

int strfind(char *str,char *substr){
	int ad = strstr(str,substr)  - (char *)&str[0];
	if( ad >= 0 )return ad;
	return -1;
}

int json_find(char *buffer_uart,char *command){
	
	if( strfind(buffer_uart,command) >= 0 ){
		int ad = strfind(buffer_uart,(char *)command);
		return ad+strlen((char *)command);
	}
	return -1;
}


char json_get_data( char *buffer , char *command ){
    
		//strcpy( json.str_data , "");
		memset(json.str_data,0,strlen(json.str_data));
	
    int ad =0;
    ad = json_find(buffer,command);
	
		if( ad == 0 )return 0;
    
    char i=0;
    if( buffer[ad] == '"' ){
        while( buffer[ad+1+i] != ',' && buffer[ad+1+i] != '"' ){
    		json.str_data[i] = buffer[ad+i+1];
    		i++;
    	}
        return 1;        
    }
    else{
        char num[10];
        while( buffer[ad+i] != ',' &&  buffer[ad+i] != '}' ){
            
    		num[i] = buffer[ad+i];
    		i++;
					if(i>5)return 0;
    	}
    	json.data = atoi(num);
    	return 2;
    }
}



void read_protocol_json(){
	
	if( json_get_data(json.document , "\"name_w1\":") == TYPE_STR ){
		strcpy( json_protocol.name_w1 , "");
		strcpy( json_protocol.name_w1 , json.str_data);
						
		if( json_get_data(json.document , "\"data_w1\":") == TYPE_STR ){
			strcpy( json_protocol.data_w1 , "");
			strcpy( json_protocol.data_w1 , json.str_data);
			json_protocol.data_w1_type = TYPE_STR;
		}
		else if( json_get_data(json.document , "\"data_w1\":") == TYPE_WORD ){
			json_protocol.data_w1_word = json.data;
			json_protocol.data_w1_type = TYPE_WORD;
		} 		
	}
	
	
	if( json_get_data(json.document , "\"name_r1\":") == TYPE_STR ){
		strcpy( json_protocol.name_r1 , "");
		strcpy( json_protocol.name_r1 , json.str_data);
						
		if( json_get_data(json.document , "\"data_r1\":") == TYPE_STR ){
			strcpy( json_protocol.data_r1 , "");
			strcpy( json_protocol.data_r1 , json.str_data);
			json_protocol.data_r1_type = TYPE_STR;
		}
		else if( json_get_data(json.document , "\"data_r1\":") == TYPE_WORD ){
			json_protocol.data_r1_word = json.data;
			json_protocol.data_r1_type = TYPE_WORD;
		}    
	}

	if( json_get_data(json.document , "\"serial\":") == TYPE_STR ){
		strcpy( json_protocol.serial , "");
		strcpy( json_protocol.serial , json.str_data);					   
	}
		
}

void reset_json(){
	
	json_protocol.data_r1_type=0;
	json_protocol.data_w1_type=0;
	json_protocol.data_r1_word=0;
	json_protocol.data_w1_word=0;
	
	memset( json_protocol.data_r1 ,0,strlen(json_protocol.data_r1));
	memset( json_protocol.data_w1 ,0,strlen(json_protocol.data_w1));
	
	memset( json_protocol.name_w1 ,0,strlen(json_protocol.name_w1));
	memset( json_protocol.name_r1 ,0,strlen(json_protocol.name_r1));
	
	memset(json.document,0,UART_BUF_SIZE);
	
}

void json_get_data_dma(){
	
	/*strcpy( json.document ,"");
	strcpy(json.document,(const char*)HALL_RX_Buffer);
	//puts(json.document);
			
	//strcpy(HALL_RX_Buffer,"");
					
	DMA1_Stream2->CR  = 0; 
	DMA1_Stream2->NDTR = UART_BUFFER_SIZE;
	memset(HALL_RX_Buffer,0,UART_BUFFER_SIZE);
	DMA1_Stream2_Init();*/

	
}

void json_get_document( char *buffer , char *counter  ){
	
	strcpy( json.document ,"");
	strcpy(json.document,(const char*)buffer);
	//puts(json.document);
			
	//strcpy(HALL_RX_Buffer,"");
					
	counter[0] = 0;
	
	memset(buffer,0,UART_BUF_SIZE);
	
}




