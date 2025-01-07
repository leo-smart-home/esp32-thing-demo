#include "task_mqtt.h"

#include "mqtt_client.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "rgb_led.h"

static void event_handler(void *event_client, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void init_subscribers(void);

esp_mqtt_client_handle_t g_mqtt_client;
mqtt_subs_config g_subs_config;

void task_mqtt_init(mqtt_subs_config config)
{
    g_subs_config = config;
    const esp_mqtt_client_config_t client_config =
        {
            .broker.address.uri = "mqtt://192.168.88.210:1883",
            .credentials.username = "",
            .credentials.authentication.password = "",
            .network.disable_auto_reconnect = false,
            .session.keepalive = 20,
        };
    g_mqtt_client = esp_mqtt_client_init(&client_config);

    esp_mqtt_client_register_event(g_mqtt_client, ESP_EVENT_ANY_ID, (esp_event_handler_t)event_handler, g_mqtt_client);
    esp_mqtt_client_start(g_mqtt_client);
    printf("MQTT Initialized\n");
}

void task_mqtt_publish_message(const char *topic, const char *message)
{
    esp_mqtt_client_publish(g_mqtt_client, topic, message, 0, 1, 0);
}

/**
 * NOTE:
 * User should initialize subscriber in the `event_handler`, otherwise `MQTT_EVENT_SUBSCRIBED` is never called
 * Best place to do it, is when MQTT client connected
 */
static void init_subscribers(void)
{
    for (uint8_t i = 0; i < g_subs_config.number_of_subscribers; i++)
    {
        esp_mqtt_client_subscribe_single(g_mqtt_client, g_subs_config.map[i].mqtt_topic, 1u);
        printf("Subscribed to %s\n", g_subs_config.map[i].mqtt_topic);
    }
}

static void event_handler(void *event_client, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_CONNECTED)
    {
        printf("[MQTT_EVENT_CONNECTED]\n");
        init_subscribers();
        RGB_LED_SET_COLOR_GREEN();
    }
    else if (event_id == MQTT_EVENT_DISCONNECTED)
    {
        printf("[MQTT_EVENT_DISCONNECTED]\n");
        RGB_LED_SET_COLOR_RED();
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
        // printf("[MQTT_EVENT_DATA] %.*s: %.*s\r\n", event->topic_len, event->topic, event->data_len, event->data);
        for (uint8_t i = 0; i < g_subs_config.number_of_subscribers; i++)
        {
            if (strncmp(event->topic, g_subs_config.map[i].mqtt_topic, event->topic_len) == 0u)
            {
                g_subs_config.map[i].callback(event->data, event->data_len);
                break;
            }
        }
    }
    else if (event_id == MQTT_EVENT_ERROR)
    {
        printf("[MQTT_EVENT_ERROR]\n");
    }
    else if (event_id == MQTT_USER_EVENT)
    {
        printf("[MQTT_USER_EVENT]\n");
    }
}