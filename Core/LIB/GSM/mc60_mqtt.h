
#ifndef __MC60_MQTT_H
#define __MC60_MQTT_H

#include "main.h"

void MC60_led_status();
void mc60_mqtt_pub( char *topic , char *data);
void mc60_mqtt_sub(char *topic);

#endif /* __MC60_MQTT_H */