#include <stdio.h>
#include "esp_log.h"
#include "moon_lolita_board_v1_support.h"

#define TAG "Moon Lolita"

void app_main(void)
{   
    bsp_i2c_init();
    bsp_sdcard_init();
    bsp_nvs_init();
    bsp_usb_cdc_init();

    shtc3_wakeup();
    lv_support_init();

    ESP_LOGW(TAG,"This is a version under development.");
}
