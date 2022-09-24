#ifndef _BSP_USB_CDC_H_
#define _BSP_USB_CDC_H_

#include "esp_err.h"

typedef void (*usb_cdc_recieve_cb_t)(uint8_t *buf,size_t len);

#endif  //_BSP_USB_CDC_H_

