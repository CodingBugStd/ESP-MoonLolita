#include "lv_support.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"   //使用idf原生st7789驱动

static esp_lcd_i80_bus_handle_t bus_handle = NULL;  //总线句柄 8线并口
static esp_lcd_i80_bus_handle_t io_handle = NULL;   //io句柄
static esp_lcd_panel_handle_t panel_handle = NULL;  //屏幕句柄

//ESP32S3 LCD Controller
static void _lcd_panel_handle_init(){

}

static void _lv_driver_init(){
    //lv_disp_drv_init(NULL);
    //lv_indev_drv_init(NULL);
}

static void _lv_register_driver(){

}

static void _lv_tick_init(){

}

void lv_support_init(){
    _lcd_panel_handle_init();
    lv_init();
    _lv_driver_init();
    _lv_register_driver();
    _lv_tick_init();
}
