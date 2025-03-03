
#ifndef __MC60_MQTT_H
#define __MC60_MQTT_H

#include "main.h"

#define TIME_MC60_RANDOM_CONNECT 4
#define TIME_MC60_SERVER_CHECK_TX 100
#define TIME_MC60_SERVER_CHECK_RX 350

void MC60_led_status();
void mc60_mqtt_pub( char *topic , char *data);
void mc60_mqtt_sub(char *topic);
void clear_mc60_buffer();

#endif /* __MC60_MQTT_H */