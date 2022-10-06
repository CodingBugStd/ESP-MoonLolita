#include <stdio.h>
#include "esp_log.h"
#include "moon_lolita_board_v1_support.h"
#include "moon_lolita_ui.h"

#include "freertos\FreeRTOS.h"
#include "freertos\task.h"

#define TAG "Moon Lolita"

#include "driver/gpio.h"
static TaskHandle_t system_led_taskhandle = NULL;
static void system_led_task(void * args ){
    gpio_set_direction( GPIO_NUM_48 ,  GPIO_MODE_OUTPUT );
    gpio_set_pull_mode( GPIO_NUM_48 , GPIO_PULLUP_PULLDOWN );
    while(1){
        gpio_set_level( GPIO_NUM_48 , 1 );
        vTaskDelay( 100/portTICK_PERIOD_MS );
        gpio_set_level( GPIO_NUM_48 , 0 );
        vTaskDelay( 100/portTICK_PERIOD_MS );
    }
}

void app_main(void)
{   
    //bsp_i2c_init();
    //bsp_sdcard_init();
    //bsp_ledc_init();
    //bsp_nvs_init();
    //bsp_usb_cdc_init();
 
    //shtc3_wakeup();
    //lvgl_support_init();

    //home_page_load();

    xTaskCreate( 
        system_led_task , 
        "sys led tick",
        1 * 1024 , 
        NULL,
        1,
        &system_led_taskhandle
    );

    ESP_LOGW(TAG,"This version under development.");
}
