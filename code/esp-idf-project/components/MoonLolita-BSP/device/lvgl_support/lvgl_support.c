#include "lvgl_support.h"
#include "gt911.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"   //使用idf原生st7789驱动
#include "esp_lcd_panel_ops.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TAG "lvgl_support"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LCD_PIXEL_CLOCK_HZ     (8 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL      1
#define LCD_BK_LIGHT_OFF_LEVEL !LCD_BK_LIGHT_ON_LEVEL
#define LCD_PIN_NUM_DATA0          3
#define LCD_PIN_NUM_DATA1          46
#define LCD_PIN_NUM_DATA2          12
#define LCD_PIN_NUM_DATA3          10
#define LCD_PIN_NUM_DATA4          11
#define LCD_PIN_NUM_DATA5          13
#define LCD_PIN_NUM_DATA6          9
#define LCD_PIN_NUM_DATA7          14
#define LCD_PIN_NUM_PCLK           17
#define LCD_PIN_NUM_CS             16
#define LCD_PIN_NUM_DC             15       //RS->测试板标号
#define LCD_PIN_NUM_RST            18
#define LCD_PIN_NUM_BK_LIGHT       8

//mcu8080接口:RW引脚在没有使用时要接入3.3v接入写模式

// The pixel number in horizontal and vertical
#define LCD_H_RES              320
#define LCD_V_RES              240
// Bit number used to represent command and parameter
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8

#define LVGL_TICK_PERIOD_MS    2

static esp_lcd_panel_handle_t   panel_handle = NULL;
static esp_timer_handle_t       lvgl_tick_timer = NULL;

static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)
static lv_disp_drv_t disp_drv;      // contains callback functions
static lv_disp_t* disp = NULL;

static lv_indev_drv_t indev_drv;
static lv_indev_t*    indev = NULL;

static bool bsp_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

static void bsp_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    //esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

static void bsp_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static TaskHandle_t bsp_lvgl_handle_taskHandle = NULL;
static void bsp_lvgl_handle_task( void *arg ){
    while(1){
        lv_timer_handler();
        vTaskDelay( 10 / portTICK_PERIOD_MS );
    }
}

static void peripheral_init(){
    //背光GPIO初始化
    ESP_LOGI(TAG, "Turn off LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << LCD_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
    gpio_set_level(LCD_PIN_NUM_BK_LIGHT, LCD_BK_LIGHT_OFF_LEVEL);

    //i80bus
    ESP_LOGI(TAG, "Initialize Intel 8080 bus");
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_i80_bus_config_t bus_config = {
        .dc_gpio_num = LCD_PIN_NUM_DC,
        .wr_gpio_num = LCD_PIN_NUM_PCLK,
        .data_gpio_nums = {
            LCD_PIN_NUM_DATA0,
            LCD_PIN_NUM_DATA1,
            LCD_PIN_NUM_DATA2,
            LCD_PIN_NUM_DATA3,
            LCD_PIN_NUM_DATA4,
            LCD_PIN_NUM_DATA5,
            LCD_PIN_NUM_DATA6,
            LCD_PIN_NUM_DATA7,
        },
        .bus_width = 8,
        .max_transfer_bytes = LCD_V_RES * 40 * sizeof(uint16_t)
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    //i80 io handle
    ESP_LOGI(TAG, "Initialize Intel 8080 bus io handle");
    esp_lcd_panel_io_handle_t io_handle = NULL;
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
        .on_color_trans_done = bsp_notify_lvgl_flush_ready,
        .user_ctx = &disp_drv,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install LCD driver of st7789");
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_NUM_RST,
        .color_space = ESP_LCD_COLOR_SPACE_RGB,
        .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_st7789(io_handle, &panel_config, &panel_handle));
}

static void st7789_init(){
    ESP_LOGI(TAG, "ST7789 reset");
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_swap_xy( panel_handle , true );       //配置为横屏模式
    esp_lcd_panel_mirror( panel_handle , true , false );
    esp_lcd_panel_invert_color(panel_handle, false);    //使用lvgl的颜色字节倒置
    // the gap is LCD panel specific, even panels with the same driver IC, can have different gap value
    esp_lcd_panel_set_gap(panel_handle, 0, 0);
    ESP_LOGI(TAG, "Turn on LCD backlight");
    gpio_set_level(LCD_PIN_NUM_BK_LIGHT, LCD_BK_LIGHT_ON_LEVEL);
}

static void lv_buff_init(){
    ESP_LOGI(TAG, "Initialize LVGL library");
    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    lv_color_t *buf1 = heap_caps_malloc(LCD_V_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(LCD_V_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, LCD_V_RES * 20);
}

static void bsp_lvgl_indev_read_cb( lv_indev_drv_t *drv, lv_indev_data_t *data ){
    data->state = LV_INDEV_STATE_RELEASED;
    if( gt911_get_pos( (uint16_t*)&data->point.x , (uint16_t*)&data->point.y ) ){
        data->state = LV_INDEV_STATE_PRESSED;
    }
}

static void lv_indev_register(){
    lv_indev_drv_init(&indev_drv);              /*Basic initialization*/
    indev_drv.type = LV_INDEV_TYPE_POINTER;                 /*See below.*/
    indev_drv.read_cb = bsp_lvgl_indev_read_cb;         /*See below.*/
    /*Register the driver in LVGL and save the created input device object*/
    indev = lv_indev_drv_register(&indev_drv);
}

static void lv_disp_register(){
    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_H_RES;
    disp_drv.ver_res = LCD_V_RES;
    disp_drv.flush_cb = bsp_lvgl_flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    disp = lv_disp_drv_register(&disp_drv);
}

static void lv_inc_timer_init(){
    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &bsp_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));
}

void lvgl_support_init(){
    peripheral_init();
    st7789_init();
    gt911_init();
    lv_init();
    lv_buff_init();
    lv_disp_register();
    lv_indev_register();
    lv_inc_timer_init();

    //首先执行一次lv_timer_handler!
    //因为是异步执行handler 所以会导致一次handler没有执行就调用lv_src_load()
    lv_timer_handler();

    xTaskCreate(
        bsp_lvgl_handle_task,
        "lvgl",
        16 * 1024,
        NULL,
        2,
        &bsp_lvgl_handle_taskHandle
    );
}


