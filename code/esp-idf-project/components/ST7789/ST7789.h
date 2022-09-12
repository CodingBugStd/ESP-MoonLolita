#ifndef _ST7789_H_
#define _ST7789_H_

#include "stdio.h"

#ifdef __cplusplus
extern "C"{
#endif

//RGB565

void ST7739_init();
void ST7739_deinit();
void ST7739_reset();
void ST7739_display_off();
void ST7739_display_on();
void ST7739_sleep();
void ST7739_wakeup();
void ST7739_draw_point(uint16_t x,uint16_t y,uint16_t color);
void ST7739_draw_region(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);

#ifdef __cplusplus
}
#endif

#endif  //_ST7789_H_
