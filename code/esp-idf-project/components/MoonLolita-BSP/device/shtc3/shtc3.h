#ifndef _SHTC3_H_
#define _SHTC3_H_

#include "esp_err.h"

esp_err_t shtc3_sleep();
esp_err_t shtc3_wakeup();
esp_err_t shtc3_reset();
esp_err_t shtc3_get_value(float *temp , float *hum);

#endif  //_SHTC3_H_

