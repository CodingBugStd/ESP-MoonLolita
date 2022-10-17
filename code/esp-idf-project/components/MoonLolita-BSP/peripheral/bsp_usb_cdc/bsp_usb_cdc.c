#include "bsp_usb_cdc.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"

#include "esp_log.h"

#define TAG "bsp_usb_cdc"

#define CDC_BUFFER_LEN             512
#define CDC_BUFFER_PROTECT_LEN     64

static user_cdc_recieve_cb_t user_rx_callback = NULL;

static uint8_t user_rx_buf[CDC_BUFFER_LEN + CDC_BUFFER_PROTECT_LEN ];
static uint16_t rx_len = 0;

static void rx_callback(int itf, cdcacm_event_t *event){
    /* initialization */
    size_t rx_size = 0;

    //将接收到的数据追加到缓冲区后面
    tinyusb_cdcacm_read(itf, user_rx_buf + rx_len , CONFIG_TINYUSB_CDC_RX_BUFSIZE, &rx_size );
    rx_len += rx_size;

    if(rx_len > CDC_BUFFER_LEN){
        ESP_LOGI(TAG,"Rx buffer is full!!");
        rx_len = 0;
    }
    if( user_rx_callback ){
        user_rx_callback( user_rx_buf , rx_len );
    }
}

esp_err_t bsp_usb_cdc_init(){
    tinyusb_config_t tusb_cfg = {0}; // the configuration using default values
    esp_err_t ret = ESP_FAIL;

    ret = tinyusb_driver_install(&tusb_cfg);
    if( ret != ESP_OK ){
        ESP_LOGE(TAG,"driver install error!");
        return ret;
    }

    tinyusb_config_cdcacm_t amc_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = &rx_callback,
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL
    };

    ret = tusb_cdc_acm_init(&amc_cfg);
    if( ret != ESP_OK ){
        ESP_LOGE(TAG,"cdc init error!");
        return ret;
    }

    return ret;
}

esp_err_t bsp_usb_cdc_send(uint8_t *buf,size_t len){
    tinyusb_cdcacm_write_queue( TINYUSB_CDC_ACM_0 , buf , len );
    return tinyusb_cdcacm_write_flush( TINYUSB_CDC_ACM_0 , 0 );     //非阻塞式
}

esp_err_t bsp_usb_cdc_clear_rx_buffer(){
    rx_len = 0;
    return ESP_OK;
}

esp_err_t bsp_usb_cdc_register_recieve_callback(user_cdc_recieve_cb_t cb){
    user_rx_callback = cb;
    return ESP_OK;
}

