#include <stdio.h>
#include "esp_log.h"
#include "moon_lolita_board_v1_support.h"
#include "moon_lolita_ui.h"

#include "lvgl.h"

#include "freertos\FreeRTOS.h"
#include "freertos\task.h"

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

static void btn_cb(lv_event_t * e){
    ESP_LOGI(TAG,"button press");
}

void app_main(void)
{   
    ESP_LOGW(TAG,"This version under development.");

    bsp_i2c_init();
    lvgl_support_init();

    home_page_load();

    //RGB测试
    // vTaskDelay(2000 / portTICK_PERIOD_MS );

    // lv_obj_t *page = lv_obj_create( NULL );
    // lv_obj_set_size( page , 320 , 320 );
    // lv_obj_set_style_bg_color( page , lv_color_make(255, 0, 0) , LV_PART_MAIN );

    // lv_obj_t *page1 = lv_obj_create( NULL );
    // lv_obj_set_size( page1 , 320 , 320 );
    // lv_obj_set_style_bg_color( page1 , lv_color_make(0, 255, 0) , LV_PART_MAIN );

    // lv_obj_t *page2 = lv_obj_create( NULL );
    // lv_obj_set_size( page2 , 320 , 320 );
    // lv_obj_set_style_bg_color( page2 , lv_color_make(0, 0, 255) , LV_PART_MAIN );


    // while( 1 ){
    //     lv_scr_load_anim( page , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
    //     vTaskDelay(1000/portTICK_PERIOD_MS);
    //     lv_scr_load_anim( page1 , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
    //     vTaskDelay(1000/portTICK_PERIOD_MS);
    //     lv_scr_load_anim( page2 , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
    //     vTaskDelay(1000/portTICK_PERIOD_MS);
    // }

    while( vTaskDelay(1) , 1 );
}
