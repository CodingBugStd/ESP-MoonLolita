#include "lv_support.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "ST7789.h"

static esp_timer_handle_t   _timer = NULL;

static void _driver_init(){
    //lv_disp_drv_init(NULL);
    //lv_indev_drv_init(NULL);
}

static void _register_driver(){

}

static void _lv_tick_init(){

}

void lv_support_init(){
    lv_init();
    _driver_init();
    _register_driver();
    _lv_tick_init();
}
