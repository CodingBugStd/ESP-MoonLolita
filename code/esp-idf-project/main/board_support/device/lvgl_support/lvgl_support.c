#include "lvgl_support.h"
#include "esp_timer.h"
#include "lvgl.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"   //使用idf原生st7789驱动
#include "esp_lcd_panel_ops.h"

static esp_timer_handle_t   _timer = NULL; 
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf_1[LCD_WIDTH * 10];
static lv_color_t buf_2[LCD_WIDTH * 10];
static lv_disp_drv_t disp_drv;
static lv_disp_t * disp = NULL;

static esp_lcd_i80_bus_handle_t _bus_handle = NULL;  //总线句柄 8线并口
static esp_lcd_i80_bus_handle_t _io_handle = NULL;   //io句柄
static esp_lcd_panel_handle_t _panel_handle = NULL;  //屏幕句柄

//LCD 控制器 颜色发送完毕回调  用于通知lvgl颜色刷写完毕 -> lv_disp_flush_ready
static bool _notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

//ESP32S3 LCD Controller
static void _lcd_panel_handle_init(){
    //LCD 8080接口初始化
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = LCD_PIN_NUM_DC,
        .wr_gpio_num = LCD_PIN_NUM_PCLK,
        .data_gpio_nums = {
            LCD_DATA_PIN_NUM_DATA0,
            LCD_DATA_PIN_NUM_DATA1,
            LCD_DATA_PIN_NUM_DATA2,
            LCD_DATA_PIN_NUM_DATA3,
            LCD_DATA_PIN_NUM_DATA4,
            LCD_DATA_PIN_NUM_DATA5,
            LCD_DATA_PIN_NUM_DATA6,
            LCD_DATA_PIN_NUM_DATA7,
        },
        .bus_width = 8,
        .max_transfer_bytes = LCD_WIDTH * 40 * sizeof(uint16_t)
    };
    esp_lcd_new_i80_bus(&bus_config, &_bus_handle);

    //LCD IO 初始化
    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = LCD_PIN_NUM_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .on_color_trans_done = _notify_lvgl_flush_ready,
        .user_ctx = &disp_drv,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    esp_lcd_new_panel_io_i80(_bus_handle, &io_config, &_io_handle);

    //LCD 句柄初始化
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_NUM_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
    };
    esp_lcd_new_panel_st7789(_io_handle, &panel_config, &_panel_handle);
}

static void _lcd_init(){
    //复位
    esp_lcd_panel_reset(_panel_handle);
    esp_lcd_panel_init(_panel_handle);
    //色彩倒置
    esp_lcd_panel_invert_color(_panel_handle, true);
    //像素点间距
    esp_lcd_panel_set_gap(_panel_handle, 0, 0);
}

static void _lv_flush_callback(){

}

static void _lv_driver_init(){
    lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, LCD_WIDTH*10);
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = _lv_flush_callback;
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HIGH;
}

static void _lv_register_driver(){
    disp = lv_disp_drv_register(&disp_drv);
}

static void _timer_callback(void *args){
    lv_tick_inc(LV_TICK_INC);
    lv_timer_handler();
}

static void _lv_tick_init(){
    if(_timer)  return;
    esp_timer_create_args_t args;
    args.callback = _timer_callback;
    args.name = "lv_tick";
    args.arg = NULL;
    args.dispatch_method = ESP_TIMER_TASK;
    args.skip_unhandled_events = false;
    esp_timer_create(&args , &_timer);
    esp_timer_start_periodic(_timer,LV_TICK_INC*1000);
}

void lv_support_init(){
    _lcd_panel_handle_init();
    _lcd_init();
    lv_init();
    _lv_driver_init();
    _lv_register_driver();
    _lv_tick_init();
}
