#ifndef _BSP_USB_CDC_H_
#define _BSP_USB_CDC_H_

#include "esp_err.h"

/*

example code:
###################################
#include "moon_lolita_board_v1_support.h"

static void cdc_callback(uint8_t *buf,uint8_t len){
    bsp_usb_cdc_send( buf ,len);
    bsp_usb_cdc_clear_rx_buffer();
}

static void usb_test(){
    bsp_usb_cdc_init();
    bsp_usb_cdc_register_recieve_callback( cdc_callback );
}

*/

typedef void (*user_cdc_recieve_cb_t)(uint8_t *buf,size_t len);

esp_err_t bsp_usb_cdc_init();
esp_err_t bsp_usb_cdc_send(uint8_t *buf,size_t len);
esp_err_t bsp_usb_cdc_clear_rx_buffer();
esp_err_t bsp_usb_cdc_register_recieve_callback(user_cdc_recieve_cb_t cb);

#endif  //_BSP_USB_CDC_H_

