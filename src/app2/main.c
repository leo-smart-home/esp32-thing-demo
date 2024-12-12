#include <stdio.h>             //for basic printf commands
#include <string.h>            //for handling strings
#include "freertos/FreeRTOS.h" //for delay,mutexs,semphrs rtos operations
#include "esp_system.h"        //esp_init funtions esp_err_t
#include "esp_wifi.h"          //esp_wifi_init functions and wifi operations
#include "esp_log.h"           //for showing logs
#include "esp_event.h"         //for wifi event
#include "nvs_flash.h"         //non volatile storage
#include "lwip/err.h"          //light weight ip packets error handling
#include "lwip/sys.h"          //system applications for light weight ip apps
#include "driver/uart.h"

// !===============================================================================================

// #include "protocol_examples_common.h" //important for running different protocols in code
#include "mqtt_client.h"       //provides important functions to connect with MQTT
#include "esp_event.h"         //managing events of mqtt
#include "nvs_flash.h"         //storing mqtt and wifi configs and settings
#include "freertos/FreeRTOS.h" //it is important too if you want to run mqtt task independently and provides threads funtionality
#include "freertos/task.h"     //MQTT communication often involves asynchronous operations, and FreeRTOS helps handle those tasks effectively
#include "esp_log.h"           //log out put, do not use printf everywhere

static void mqtt_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{ // here esp_mqtt_event_handle_t is a struct which receieves struct event from mqtt app start funtion
    if (event_id == MQTT_EVENT_CONNECTED)
    {
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

static void mqtt_initialize(void)
{
    const esp_mqtt_client_config_t mqtt_cfg = {};
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    esp_err_t err;
    err = esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, (esp_event_handler_t)mqtt_event_handler, client);
    printf("esp_mqtt_client_register_event: %x\n", err);
    err = esp_mqtt_client_set_uri(client, "mqtt://test.mosquitto.org:1883");
    printf("esp_mqtt_client_set_uri: %x\n", err);
    err = esp_mqtt_client_start(client);
    printf("esp_mqtt_client_start: %x\n", err);
    printf("MQTT Initialized\n");


    int msg_id;
    msg_id = esp_mqtt_client_subscribe(client, "/leonid/hello/test", 0);
    printf("sent subscribe successful, msg_id=%d\n", msg_id);

    // Publish a message to a topic
    msg_id = esp_mqtt_client_publish(client, "/eleonid/hello/test1", "Hello from ESP32", 0, 1, 0);
    printf("sent publish successful, msg_id=%d\n", msg_id);
}

// !===============================================================================================

static void init_uart()
{
    uart_config_t uart_config =
        {
            .baud_rate = 9600u,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);
}

int _write(int file, char *ptr_data, int len)
{
    (void)file; // Not used
    return uart_write_bytes(UART_NUM_0, (char *)ptr_data, (size_t)len);
}

const char *ssid = "STVWIFI_VBK9";
const char *pass = "sTv572706";
int retry_num = 0;
static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
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
        printf("WiFi lost connection\n");
        if (retry_num < 5)
        {
            esp_wifi_connect();
            retry_num++;
            printf("Retrying to Connect...\n");
        }
    }
    else if (event_id == IP_EVENT_STA_GOT_IP)
    {
        printf("Wifi got IP...\n\n");
    }
}

void wifi_connection()
{
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation); //
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);
    wifi_config_t wifi_configuration = {
        .sta = {
            .ssid = "",
            .password = "",

        }

    };
    strcpy((char *)wifi_configuration.sta.ssid, ssid);
    strcpy((char *)wifi_configuration.sta.password, pass);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_connect();
    printf("wifi_init_softap finished. SSID:%s  password:%s", ssid, pass);
}

void app_main(void)
{
    init_uart();
    nvs_flash_init();
    wifi_connection();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    mqtt_initialize();
}