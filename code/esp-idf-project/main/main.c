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

#include "string.h"
static TaskHandle_t usb_taskhandle = NULL;
static void usb_task(void* args){
    while(1){
        vTaskDelay(500/portTICK_PERIOD_MS);
        bsp_usb_cdc_send( (uint8_t*)args , strlen((char*)args) );
    }
}

void app_main(void)
{   
    //bsp_i2c_init();
    //bsp_sdcard_init();
    //bsp_ledc_init();
    //bsp_nvs_init();
    bsp_usb_cdc_init();
 
    //shtc3_wakeup();
    //lvgl_support_init();

    //home_page_load();

    
    xTaskCreate(
        usb_task,
        "usb",
        1 * 1024,
        "HelloWorld!\r\n",
        12,
        &usb_taskhandle
    );

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
