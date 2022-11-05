/*
 * @Author: 没人要的野指针
 * @Date: 2022-10-19 20:50:01
 * @LastEditors: 没人要的野指针
 * @LastEditTime: 2022-11-06 01:20:11
 * @Description: 对idf的wifi功能的二次封装
 * Copyright (c) 2022 CodingBugStd, All Rights Reserved. 
 */

#ifndef _BSP_WIFI_H_
#define _BSP_WIFI_H_

#include "esp_err.h"

esp_err_t bsp_wifi_station_connect(char *wifi_name , char *wifi_password);
esp_err_t bsp_wifi_station_disconnect();

esp_err_t bsp_wifi_ap_init(char *wifi_name , char *wifi_password);
esp_err_t bsp_wifi_ap_deinit();
esp_err_t bsp_wifi_ap_register_event_callback();
esp_err_t bsp_wifi_ap_get_connected_device_info();

#endif //_BSP_WIFI_H_

