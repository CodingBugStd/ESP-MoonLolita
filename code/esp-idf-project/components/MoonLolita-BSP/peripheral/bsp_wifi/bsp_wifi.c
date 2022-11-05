#include "bsp_wifi.h"

#include "esp_wifi.h"
#include "lwip/err.h"
#include "lwip/sys.h"

#include "esp_log.h"

#define TAG "bsp_wifi"

typedef struct{
    enum{
        WIFI_UNDEFINE = 0,
        WIFI_STA_MODE = 1,
        WIFI_AP_MODE = 2,
    }wifi_mode;
    union
    {
        enum{
            WIFI_CONNECTED = 0,
            WIFI_UNCONNECTED = 1,
        }wifi_station_state;
        enum{
            DEVICE_CONNECTED = 0,
            DEVICE_UNCONNECTED = 1,
        }wifi_ap_state;
    }wifi_state;
}bsp_wifi_state_t;

static esp_err_t bsp_wifi_init(){
    //wifi需要nvs支持
    //没有使用bsp_nvs_init(),减少耦合
    //bsp_nvs_init()与nvs_flash_init()不冲突
    if( nvs_flash_init() != ESP_OK ){
        ESP_LOGE(TAG,"nvs init error!");
        return ESP_FAIL;
    }

    //netif -> lwip中用来描述一个硬件网络接口
    //硬件接口层
    if( esp_netif_init() != ESP_OK ){
        ESP_LOGE(TAG,"netif init error!");
        return ESP_FAIL;
    }

    //wifi硬件初始化
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    if( esp_wifi_init(&cfg) != ESP_OK ){
        ESP_LOGE(TAG,"wifi init error!");
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t bsp_wifi_station_init(){
    esp_netif_create_default_wifi_sta();
    return ESP_OK;
}

