#ifndef _AW9523B_H_
#define _AW9523B_H_

#include "esp_err.h"

esp_err_t aw9523b_init();
esp_err_t aw9523b_reset();
esp_err_t aw9523b_sleep();
esp_err_t aw9523b_wakeup();
esp_err_t aw9523b_set_mode(uint8_t port , uint8_t mode);
esp_err_t aw9523b_write(uint8_t port , uint8_t byte);
esp_err_t aw9523b_read(uint8_t port , uint8_t *byte);

#endif  //_AW9523B_H_
