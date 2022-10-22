#include <stdio.h>
#include "esp_log.h"
#include "moon_lolita_board_v1_support.h"
#include "moon_lolita_ui.h"

#include "lvgl.h"

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
        ESP_LOGI(TAG,"LED_ON");
        vTaskDelay( 1000/portTICK_PERIOD_MS );
        gpio_set_level( GPIO_NUM_48 , 0 );
        ESP_LOGI(TAG , "LED_OFF" );
        vTaskDelay( 1000/portTICK_PERIOD_MS );
    }
}

void app_main(void)
{   
    ESP_LOGW(TAG,"This version under development.");

    lvgl_support_init();

    lv_obj_t *page = lv_obj_create( NULL );

    lv_obj_set_size( page , 240 , 320 );
    lv_obj_set_style_bg_color( page , lv_palette_main(LV_PALETTE_BLUE) , LV_PART_MAIN );
    lv_scr_load( page );

    xTaskCreate( 
        system_led_task ,
        "sys led tick",
        2 * 1024 , 
        NULL,
        1,
        &system_led_taskhandle
    );

    while( vTaskDelay(1) , 1 );
}
