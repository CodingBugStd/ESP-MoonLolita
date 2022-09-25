#ifndef _BSP_USB_CDC_H_
#define _BSP_USB_CDC_H_

#include "esp_err.h"

typedef void (*usb_cdc_recieve_cb_t)(uint8_t *buf,size_t len);

esp_err_t bsp_usb_cdc_init();
esp_err_t bsp_usb_cdc_send(uint8_t *buf,size_t len);
esp_err_t bsp_usb_cdc_recieve(uint8_t *buf,size_t len);
esp_err_t bsp_usb_cdc_register_recieve_callback(usb_cdc_recieve_cb_t cb,size_t trigger_len,int timeout_ms);


#endif  //_BSP_USB_CDC_H_

