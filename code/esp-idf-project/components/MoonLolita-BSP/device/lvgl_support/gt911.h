#ifndef _GT911_H_
#define _GT911_H_

#include "esp_err.h"
#include <stdbool.h>

void gt911_init();
esp_err_t gt911_printf_info();
bool gt911_get_pos(uint16_t *x , uint16_t *y);

#endif	//_GT911_H_

