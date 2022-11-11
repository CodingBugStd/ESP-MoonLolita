/*
 * @Author: 没人要的野指针
 * @Date: 2022-10-19 20:50:01
 * @LastEditors: 没人要的野指针 https://github.com/CodingBugStd
 * @LastEditTime: 2022-11-12 00:51:31
 * @Description: 对idf的wifi功能的二次封装
 * Copyright (c) 2022 CodingBugStd, All Rights Reserved. 
 */

#ifndef _BSP_WIFI_H_
#define _BSP_WIFI_H_

#include "esp_err.h"

typedef enum{
    WIFI_UNINIT = -1,       //未初始化
    WIFI_STANDBY  = 0,      //待机
    STA_CONNECTTING = 1,    //连接中
    STA_CONNECTED = 2,      //已连接
    STA_DISCONNECTED = 3,   //未连接
    STA_DISCONNECTTING = 4, //断开连接中
}bsp_wifi_state_t;

esp_err_t bsp_wifi_init();
esp_err_t bsp_wifi_sleep();
esp_err_t bsp_wifi_wakeup();
bsp_wifi_state_t bsp_wifi_get_state();

esp_err_t bsp_wifi_station_connect(char *wifi_name ,char *wifi_password);
esp_err_t bsp_wifi_station_disconnect();

//todo
esp_err_t bsp_wifi_ap_deinit();
esp_err_t bsp_wifi_ap_register_event_callback();
esp_err_t bsp_wifi_ap_get_connected_device_info();

#endif //_BSP_WIFI_H_

