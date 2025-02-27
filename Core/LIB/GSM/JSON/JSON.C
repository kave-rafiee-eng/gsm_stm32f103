#include "main.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "json.h"


struct JSON_OUT	 json;

// Find substring position
int strfind(char *str, char *substr) {
    char *pos = strstr(str, substr);
    return (pos) ? (pos - str) : -1;
}


// Locate a JSON field in the buffer
int json_find(char *buffer_uart, char *command) {
    int pos = strfind(buffer_uart, command);
    return (pos >= 0) ? pos + strlen(command) : -1;
}

// Extract data from JSON buffer
char json_get_data(char *buffer, char *command) {
    memset(json.str_data, 0, sizeof(json.str_data));

    int ad = json_find(buffer, command);
    if (ad == -1) return 0;

    char i = 0;
    if (buffer[ad] == '"') {  
        while (buffer[ad + 1 + i] != ',' && buffer[ad + 1 + i] != '"' && i < sizeof(json.str_data) - 1) {
            json.str_data[i] = buffer[ad + i + 1];
            i++;
        }
        json.str_data[i] = '\0';  // Ensure string termination
        return TYPE_STR;
    } else {
        char num[10] = {0};
        while (buffer[ad + i] != ',' && buffer[ad + i] != '}' && i < sizeof(num) - 1) {
            num[i] = buffer[ad + i];
            i++;
        }
        json.data = atoi(num);
        return TYPE_WORD;
    }
    return 0;
}

// Reset JSON protocol variables
void reset_json() {
    memset(json.document, 0, sizeof(json.document));
}


// Replace a character in a string
int replacechar(char *str, char orig, char rep) {
    char *ix = str;
    int count = 0;
    while ((ix = strchr(ix, orig)) != NULL) {
        *ix++ = rep;
        count++;
    }
    return count;
}




