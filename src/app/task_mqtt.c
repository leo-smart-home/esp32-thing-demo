#include "task_mqtt.h"

#include "mqtt_client.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

esp_mqtt_client_handle_t mqtt_client;

// typedef struct
// {
//     const char *topic;
//     mqtt_subscribe_handler_t handler;
// } mqtt_sub_topic_to_handler_s;

static void mqtt_event_handler(void *event_client, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_CONNECTED)
    {
        printf("[MQTT_EVENT_CONNECTED]\n");
        esp_mqtt_client_subscribe_single((esp_mqtt_client_handle_t)event_client, "/test", 0);
        // esp_mqtt_client_subscribe(client, "your topic", 0);
        printf("Subscribed to /test\n");
    }
    else if (event_id == MQTT_EVENT_DISCONNECTED)
    {
        printf("[MQTT_EVENT_DISCONNECTED]\n");
    }
    else if (event_id == MQTT_EVENT_SUBSCRIBED)
    {
        printf("[MQTT_EVENT_SUBSCRIBED]\n");
    }
    else if (event_id == MQTT_EVENT_UNSUBSCRIBED)
    {
        printf("[MQTT_EVENT_UNSUBSCRIBED]\n");
    }
    else if (event_id == MQTT_EVENT_DATA)
    {
        esp_mqtt_event_t *event = (esp_mqtt_event_t *)event_data;
        printf("[MQTT_EVENT_DATA] %.*s: %.*s\r\n", event->topic_len, event->topic, event->data_len, event->data);
    }
    else if (event_id == MQTT_EVENT_ERROR)
    {
        printf("[MQTT_EVENT_ERROR]\n");
    }
}

void task_mqtt_publish_message(const char *topic, const char *message)
{
    esp_mqtt_client_publish(mqtt_client, topic, message, 0, 1, 0);
}

void task_mqtt_subscribe(const char *topic, mqtt_subscribe_handler_t handler)
{
    esp_mqtt_client_subscribe_single(mqtt_client, topic, 0);
    printf("Subscribed to %s\n", topic);
    (void)handler; // TODO: Map topic to handler; MB use topic ID
}

void task_mqtt_init(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        // .broker.address.uri = "mqtt://broker.hivemq.com:1883",
        .broker.address.uri = "mqtt://192.168.88.210:1883",
        .credentials.username = "",
        .credentials.authentication.password = "",
        .network.disable_auto_reconnect = false,
        .session.keepalive = 60,
    };
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_err_t err;
    err = esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, (esp_event_handler_t)mqtt_event_handler, mqtt_client);
    printf("esp_mqtt_client_register_event: %x\n", err);
    err = esp_mqtt_client_start(mqtt_client);
    printf("esp_mqtt_client_start: %x\n", err);
    printf("MQTT Initialized\n");
}