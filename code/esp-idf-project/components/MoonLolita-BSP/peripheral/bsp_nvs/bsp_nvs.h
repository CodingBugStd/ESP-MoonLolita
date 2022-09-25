#ifndef _BSP_NVS_H_
#define _BSP_NVS_H_

#include "esp_err.h"

esp_err_t bsp_nvs_init();
esp_err_t bsp_nvs_clear();
esp_err_t bsp_nvs_get(char* key,uint8_t *buf,size_t len);
esp_err_t bsp_nvs_set(char* key,uint8_t *buf , size_t len);

#endif //_BSP_NVS_H_

