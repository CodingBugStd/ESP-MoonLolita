#include <stdio.h>
#include "esp_log.h"
#include "moon_lolita_board_v1_support.h"
#include "moon_lolita_ui.h"

#define TAG "Moon Lolita"

void app_main(void)
{   
    bsp_i2c_init();
    bsp_sdcard_init();
    bsp_nvs_init();
    bsp_usb_cdc_init();
 
    shtc3_wakeup();
    lvgl_support_init();

    home_page_load();

    ESP_LOGW(TAG,"This version under development.");
}
