#ifndef _ADC_KEYBOARD_H_
#define _ADC_KEYBOARD_H_

#include "esp_err.h"

esp_err_t adc_keyboard_init();
esp_err_t adc_keyboard_deinit();
int adc_keyboard_get_keycode();
int adc_keyboard_get_adc_value();

#endif //_ADC_KEYBOARD_H_

