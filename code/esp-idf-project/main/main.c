#include <stdio.h>
#include "esp_log.h"
#include "freertos\FreeRTOS.h"
#include "freertos\task.h"

#include "moon_lolita_board_v1_support.h"
#include "moon_lolita_ui.h"
#include "mqtt.h"
#include "lvgl.h"

#define TAG "Moon Lolita"

/*
xTaskCreate( 
        system_led_task ,
        "sys led tick",
        4 * 1024 , 
        NULL,
        1,
        &system_led_taskhandle
    );
*/
#include "driver/gpio.h"
static TaskHandle_t system_led_taskhandle = NULL;
static void system_led_task(void * args ){
    gpio_set_direction( GPIO_NUM_48 ,  GPIO_MODE_OUTPUT );
    gpio_set_pull_mode( GPIO_NUM_48 , GPIO_PULLUP_PULLDOWN );
    while(1){
        gpio_set_level( GPIO_NUM_48 , 1 );
        ESP_LOGI(TAG,"LED_ON");
        vTaskDelay( 1000/portTICK_PERIOD_MS );
        gpio_set_level( GPIO_NUM_48 , 0 );
        ESP_LOGI(TAG , "LED_OFF" );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
}

#include "./device/lvgl_support/gt911.h"

void app_main(void)
{   
    ESP_LOGW(TAG,"This version under development.");

    bsp_i2c_init();
    lvgl_support_init();

    home_page_load();

    while( vTaskDelay(1) , 1 );
}
