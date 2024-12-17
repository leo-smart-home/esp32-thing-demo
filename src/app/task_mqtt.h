#ifndef MQTT_APP_H
#define MQTT_APP_H

#include <stdint.h>

typedef void (*subscriber_callback)(const char *string, uint32_t string_len);

typedef struct
{
    const char *mqtt_topic;
    subscriber_callback callback;
} subscriber_to_callback_map;

typedef struct
{
    uint8_t number_of_subscribers;
    subscriber_to_callback_map map[20]; // NOTE: Be careful
} mqtt_subs_config;

void task_mqtt_init(mqtt_subs_config subs_config);
void task_mqtt_publish_message(const char *topic, const char *message);

#endif // MQTT_APP_H