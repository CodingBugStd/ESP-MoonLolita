/*
 * @Author: 没人要的野指针
 * @Date: 2022-09-25 14:16:11
 * @LastEditors: 没人要的野指针
 * @LastEditTime: 2022-11-06 01:48:25
 * @Description: 对idf的nvs的二次封装，bsp_nvs中默认使用"bsp"名字空间。
 * Copyright (c) 2022 CodingBugStd, All Rights Reserved. 
 */
#ifndef _BSP_NVS_H_
#define _BSP_NVS_H_

#include "esp_err.h"

esp_err_t bsp_nvs_init();
esp_err_t bsp_nvs_deinit();
esp_err_t bsp_nvs_clear();
esp_err_t bsp_nvs_get(char* key,uint8_t *buf,size_t* len);
esp_err_t bsp_nvs_set(char* key,uint8_t *buf, size_t len);
esp_err_t bsp_nvs_delete(char *key);

esp_err_t bsp_nvs_check();

#endif //_BSP_NVS_H_

