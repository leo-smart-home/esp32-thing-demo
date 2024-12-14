#include "task_mqtt.h"

#include "mqtt_client.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static void mqtt_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_CONNECTED)
    {
        // ESP_LOGI("ESP", "MQTT_EVENT_CONNECTED\n");
        printf("MQTT_EVENT_CONNECTED\n");
        // esp_mqtt_client_subscribe(client, "your topic", 0);
        // printf("sent subscribe successful\n");
    }
    else if (event_id == MQTT_EVENT_DISCONNECTED)
    {
        printf("MQTT_EVENT_DISCONNECTED\n");
    }
    else if (event_id == MQTT_EVENT_SUBSCRIBED)
    {
        printf("MQTT_EVENT_SUBSCRIBED\n");
    }
    else if (event_id == MQTT_EVENT_UNSUBSCRIBED)
    {
        printf("MQTT_EVENT_UNSUBSCRIBED\n");
    }
    else if (event_id == MQTT_EVENT_DATA)
    {
        printf("MQTT_EVENT_DATA\n");
    }
    else if (event_id == MQTT_EVENT_ERROR)
    {
        printf("MQTT_EVENT_ERROR\n");
    }
}

void task_mqtt_init(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://broker.hivemq.com:1883",
        // .broker.address.uri = "mqtt://192.168.88.210:1883",
        .credentials = {
            .username = "",
            .authentication = {
                .password = ""}},
        .network = {.disable_auto_reconnect = false},
        .session = {.keepalive = 60}};
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    esp_err_t err;
    err = esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, (esp_event_handler_t)mqtt_event_handler, client);
    printf("esp_mqtt_client_register_event: %x\n", err);
    err = esp_mqtt_client_start(client);
    printf("esp_mqtt_client_start: %x\n", err);
    printf("MQTT Initialized\n");

    // int msg_id;
    // msg_id = esp_mqtt_client_subscribe(client, "/test_in", 0);
    // printf("sent subscribe successful, msg_id=%d\n", msg_id);

    // // Publish a message to a topic
    // msg_id = esp_mqtt_client_publish(client, "/test_out", "Hello from ESP32", 0, 1, 0);
    // printf("sent publish successful, msg_id=%d\n", msg_id);
}