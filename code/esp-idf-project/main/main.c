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

#include "./device/lvgl_support/gt911.h"

void app_main(void)
{   
    ESP_LOGW(TAG,"This version under development.");

    bsp_i2c_init();
    lvgl_support_init();

    xTaskCreate( 
        system_led_task ,
        "sys led tick",
        2 * 1024 , 
        NULL,
        1,
        &system_led_taskhandle
    );

    Software_Reset(1);
    vTaskDelay(10);
    Software_Reset(0);
    vTaskDelay(1);

    while(1){
        gt911_Scanf();
        ESP_LOGI( TAG , "%d %d" , User_Touch.Touch_XY->X_Point , User_Touch.Touch_XY->Y_Point );
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS );

    lv_obj_t *page = lv_obj_create( NULL );
    lv_obj_set_size( page , 320 , 320 );
    lv_obj_set_style_bg_color( page , lv_color_make(255, 0, 0) , LV_PART_MAIN );

    lv_obj_t *page1 = lv_obj_create( NULL );
    lv_obj_set_size( page1 , 320 , 320 );
    lv_obj_set_style_bg_color( page1 , lv_color_make(0, 255, 0) , LV_PART_MAIN );

    lv_obj_t *page2 = lv_obj_create( NULL );
    lv_obj_set_size( page2 , 320 , 320 );
    lv_obj_set_style_bg_color( page2 , lv_color_make(0, 0, 255) , LV_PART_MAIN );


    while( 1 ){
        lv_scr_load_anim( page , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
        vTaskDelay(1000/portTICK_PERIOD_MS);
        lv_scr_load_anim( page1 , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
        vTaskDelay(1000/portTICK_PERIOD_MS);
        lv_scr_load_anim( page2 , LV_SCR_LOAD_ANIM_OVER_RIGHT , 500 , 0 , false );
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }

    while( vTaskDelay(1) , 1 );
}
