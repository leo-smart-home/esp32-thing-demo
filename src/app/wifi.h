#ifndef WIFI_H
#define WIFI_H

#include "esp_netif.h"
#include "esp_wifi_types.h"
#include "freertos/FreeRTOS.h"


#define WIFI_AP_SSID                "ESP32_AP"
#define WIFI_AP_PASSWORD            "1111"
#define WIFI_AP_CHANNEL             1u
#define WIFI_AP_SSID_HIDDEN         0u
#define WIFI_AP_MAX_CONNECTIONS     5u
#define WIFI_AP_BEACON_INTERVAL     100u
#define WIFI_AP_IP                  "192.168.0.1"
#define WIFI_AP_GATEWAY             "192.168.0.1"
#define WIFI_AP_NETMASK             "255.255.255.0"
#define WIFI_AP_BANDWIDTH           WIFI_BW_HT20
#define WIFI_STA_POWER_SAVE         WIFI_PS_NONE
#define MAX_SSID_LENGTH             32u // IEEE default
#define MAX_PASSWORD_LENGTH         64u // IEEE default
#define MAX_CONNECTION_RETRIES      5u

extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;

typedef enum wifi_app_message
{
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP,
} wifi_app_message_e;

typedef struct wifi_app_queue_message
{
	wifi_app_message_e msg_id;
} wifi_app_queue_message_t;

BaseType_t wifi_app_send_message(wifi_app_message_e msg_id);
void wifi_app_start(void);

#endif // WIFI_H