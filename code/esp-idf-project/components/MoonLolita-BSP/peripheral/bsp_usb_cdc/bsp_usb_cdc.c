#include "bsp_usb_cdc.h"

esp_err_t bsp_usb_cdc_init(){
    return ESP_OK;
}

esp_err_t bsp_usb_cdc_send(uint8_t *buf,size_t len){
    return ESP_OK;
}

esp_err_t bsp_usb_cdc_recieve(uint8_t *buf,size_t len){
    return ESP_OK;
}

esp_err_t bsp_usb_cdc_register_recieve_callback(usb_cdc_recieve_cb_t cb,size_t trigger_len,int timeout_ms){
    return ESP_OK;
}

