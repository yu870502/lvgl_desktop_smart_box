#include "esp_wifi.h"

#include "esp_netif.h"
#include "esp_log.h"

#include "ui.h"
#include "update_ui.h"

#include "dbg_test.h"

static const char *TAG = "network_info";
static int curRSSI = 0;
static char curName[33] = {0};
char curCC[3] = {0};

int read_network_info(void)
{
    esp_err_t ret = ESP_OK;
    wifi_ap_record_t ap_info;
    static uint8_t single = 0;

    memset((void *)&ap_info, 0, sizeof(ap_info));
    if(ESP_OK != (ret = esp_wifi_sta_get_ap_info(&ap_info))){
        ESP_LOGW(TAG, "esp_wifi_sta_get_ap_info error (%s)", esp_err_to_name(ret));
        return -1;
    }

    // ESP_LOGW(TAG, "ap ssid = %s", ap_info.ssid);
    // ESP_LOGW(TAG, "ap info country code = %s", ap_info.country.cc);
    if(!single){
        memcpy(curName, ap_info.ssid, sizeof(curName));
        memcpy(curCC, ap_info.country.cc, sizeof(curCC));
        update_ui_safe(wifi_name_label, update_wifi_name, curName, sizeof(curName));
        single = 1;
    }  

    // ESP_LOGW(TAG, "ap info rssi = %d", ap_info.rssi);
    if(curRSSI != ap_info.rssi){
        curRSSI = ap_info.rssi;
        update_ui_safe(wifi_rssi_label, update_wifi_rssi, &curRSSI, sizeof(curRSSI));
    }

    return 0;
}

void moni_network_info_task(void *a)
{
    for(;;){
        read_network_info();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int start_network_info_update(void)
{
    xTaskCreate(moni_network_info_task, "moni_network_info", 4096, NULL, 1, NULL);
    return 0;
}