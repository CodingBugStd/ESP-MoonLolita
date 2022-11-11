/*
 * @Author: 没人要的野指针 https://github.com/CodingBugStd
 * @Date: 2022-10-19 20:50:08
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-12 01:08:55
 * @Description: 
 * Copyright (c) 2022 CodingBugStd, All Rights Reserved. 
 */
#include "bsp_wifi.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define WIFI_ERROR_CHECK( ret , info ) \
    do{ \
        if(ret != ESP_OK){\
            ESP_LOGE(TAG,"--error! > %s -- func:%s  line:%d", info , __FUNCTION__,__LINE__);\
            return ESP_FAIL;\
        }\
    }while(0)

#define TAG "bsp_wifi"

#define BSP_WIFI_TIMEOUT    (10*1000)

//事件标志位
#define STATION_CONNECTED_PASS_EVENT_MASK    (0x01<<0)
#define STATION_CONNECTED_FAIL_EVENT_MASK    (0x01<<1)
#define STATION_CONNECTED_LOSS_EVENT_MASK    (0x01<<2)

//wifi事件组
static EventGroupHandle_t wifi_event_group_handle = NULL;
//wifi设置
wifi_config_t wifi_config;
//wifi状态
bsp_wifi_state_t    bsp_wifi_state = WIFI_UNINIT;

static void bsp_wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG,"connect to the AP fail");
        bsp_wifi_state = STA_DISCONNECTED;
        xEventGroupSetBits(wifi_event_group_handle, STATION_CONNECTED_FAIL_EVENT_MASK);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits( wifi_event_group_handle , STATION_CONNECTED_PASS_EVENT_MASK );
    } else if( event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START){

    } else if( event_base == IP_EVENT && event_id == IP_EVENT_STA_LOST_IP ){
        ESP_LOGW(TAG,"wifi station loss connect!");
        bsp_wifi_state = STA_DISCONNECTED;
        xEventGroupSetBits( wifi_event_group_handle , STATION_CONNECTED_LOSS_EVENT_MASK );
    }
}

bsp_wifi_state_t bsp_wifi_get_state(){
    return bsp_wifi_state;
}

esp_err_t bsp_wifi_station_connect(char *wifi_name ,char *wifi_password){
    esp_netif_create_default_wifi_sta();

    strcpy( (char*)wifi_config.sta.ssid , wifi_name );
    strcpy( (char*)wifi_config.sta.password , wifi_password );
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    WIFI_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) , "mode setting");
    WIFI_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) , "config setting" );
    WIFI_ERROR_CHECK(esp_wifi_start() , "wifi start" );

    //等待wifi连接
    xEventGroupWaitBits( wifi_event_group_handle , 
                         STATION_CONNECTED_PASS_EVENT_MASK | STATION_CONNECTED_FAIL_EVENT_MASK , 
                         pdFALSE ,
                         pdFALSE ,
                        BSP_WIFI_TIMEOUT / portTICK_PERIOD_MS );

    bsp_wifi_state = STA_CONNECTTING;
    uint16_t event = xEventGroupGetBits( wifi_event_group_handle );

    ESP_LOGI(TAG,"event : 0x%02X ", event );
    ESP_LOGI(TAG,"ssid:%s", wifi_config.sta.ssid );

    if( event & STATION_CONNECTED_PASS_EVENT_MASK ){
        ESP_LOGI(TAG,"wifi station connect pass");
        xEventGroupClearBits( wifi_event_group_handle , STATION_CONNECTED_PASS_EVENT_MASK );
        bsp_wifi_state = STA_CONNECTED;
        return ESP_OK;
    }else{
        ESP_LOGI(TAG,"wifi station connect failed");
        xEventGroupClearBits( wifi_event_group_handle , STATION_CONNECTED_FAIL_EVENT_MASK );
        bsp_wifi_state = STA_DISCONNECTED;
        return ESP_FAIL;
    }
}

esp_err_t bsp_wifi_init(){
    //创建wifi事件组
    wifi_event_group_handle = xEventGroupCreate();
    if( !wifi_event_group_handle  ){
        ESP_LOGE(TAG,"wifi_event_handle create error!");
        return ESP_FAIL;
    }

    //初始化nvs
    WIFI_ERROR_CHECK( nvs_flash_init() , "nvs flash init" );
    //初始化netif -> lwip的硬件网络接口
    WIFI_ERROR_CHECK( esp_netif_init() ,  "lwip netif init");
    //建立事件循环，用于处理wifi相关的系统事件
    esp_event_loop_create_default();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    WIFI_ERROR_CHECK( esp_wifi_init(&cfg) , "wifi init");

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    WIFI_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        bsp_wifi_event_handler,    
                                                        NULL,
                                                        &instance_any_id)
                                                        ,"wifi event handler register");
    WIFI_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        bsp_wifi_event_handler,   
                                                        NULL,
                                                        &instance_got_ip)
                                                        ,"ip event handler register");
    
    bsp_wifi_state = WIFI_STANDBY;
    ESP_LOGI(TAG,"wifi init pass.");

    return ESP_OK;
}
