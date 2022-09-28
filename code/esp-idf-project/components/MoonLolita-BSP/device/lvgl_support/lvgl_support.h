#ifndef _LV_SUPPORT_H_
#define _LV_SUPPORT_H_

#define LV_TICK_INC     10
 
#define LCD_PIN_NUM_DC          0
#define LCD_PIN_NUM_PCLK        1
#define LCD_PIN_NUM_CS          2
#define LCD_PIN_NUM_RST         3
#define LCD_DATA_PIN_NUM_DATA0  0
#define LCD_DATA_PIN_NUM_DATA1  1
#define LCD_DATA_PIN_NUM_DATA2  2
#define LCD_DATA_PIN_NUM_DATA3  3
#define LCD_DATA_PIN_NUM_DATA4  4
#define LCD_DATA_PIN_NUM_DATA5  5
#define LCD_DATA_PIN_NUM_DATA6  6
#define LCD_DATA_PIN_NUM_DATA7  7

#define LCD_PIXEL_CLOCK_HZ      10*1000*1000

#define LCD_WIDTH   320
#define LCD_HIGH    240

void lvgl_support_init();

#endif  //_LV_SUPPORT_H_

