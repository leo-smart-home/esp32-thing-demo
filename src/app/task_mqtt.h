#ifndef MQTT_APP_H
#define MQTT_APP_H

#include <stdint.h>

typedef void (*mqtt_subscribe_handler_t)(const char *data, uint32_t data_len);

void task_mqtt_init(void);
void task_mqtt_publish_message(const char *topic, const char *message);
void task_mqtt_subscribe(const char *topic, mqtt_subscribe_handler_t handler);

#endif // MQTT_APP_H