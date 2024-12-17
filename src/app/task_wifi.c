#include "task_wifi.h"

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "driver/uart.h"

#define WIFI_SECRETS_NAMESPACE "wifi_storage"

static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
static void read_secret(const char *key, char *out_value, size_t max_len);
static void write_secret(const char *key, const char *value);

void task_wifi_init()
{
    char ssid[32u];
    char pass[32u];
    read_secret("ssid", ssid, 32u);
    read_secret("pass", pass, 32u);

    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); //
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, event_handler, NULL);

    wifi_config_t wifi_configuration =
    {
        .sta.ssid = "",
        .sta.password = "",
    };
    strcpy((char *)wifi_configuration.sta.ssid, ssid);
    strcpy((char *)wifi_configuration.sta.password, pass);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();
    printf("WiFi init finished; SSID:%s  password:%s\n", ssid, pass);
}

static void event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    static uint8_t retry_num = 0u;
    if (event_id == WIFI_EVENT_STA_START)
    {
        printf("WIFI CONNECTING....\n");
    }
    else if (event_id == WIFI_EVENT_STA_CONNECTED)
    {
        printf("WiFi CONNECTED\n");
    }
    else if (event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        printf("WiFi LOST CONNECTION\n");
        if (retry_num < 5u)
        {
            esp_wifi_connect();
            retry_num++;
            printf("WiFI TRYING TO RECONNECT...\n");
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        printf("WIFI GOT IP\n\n");
    }
}

static void read_secret(const char *key, char *out_value, size_t max_len)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(WIFI_SECRETS_NAMESPACE, NVS_READONLY, &handle);
    if (err == ESP_OK)
    {
        nvs_get_str(handle, key, out_value, &max_len);
        nvs_close(handle);
    }
}

static void write_secret(const char *key, const char *value)
{
    nvs_handle_t handle;
    esp_err_t err = nvs_open(WIFI_SECRETS_NAMESPACE, NVS_READWRITE, &handle);
    if (err == ESP_OK)
    {
        nvs_set_str(handle, key, value);
        nvs_commit(handle);
        nvs_close(handle);
    }
}
