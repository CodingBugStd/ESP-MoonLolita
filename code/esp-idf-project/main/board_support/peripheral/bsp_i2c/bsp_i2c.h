#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "driver/i2c.h"

esp_err_t bsp_i2c_init();
esp_err_t i2c_write(uint8_t dev_addr , uint8_t* buf , size_t len);
esp_err_t i2c_read(uint8_t dev_addr , uint8_t* buf , size_t len);
esp_err_t i2c_write_read(uint8_t dev_addr , uint8_t*w_buf , size_t w_len , uint8_t*r_buf , size_t r_len);
esp_err_t i2c_write_reg8(uint8_t dev_addr , uint8_t reg_addr , uint8_t dat);
esp_err_t i2c_read_reg8(uint8_t dev_addr , uint8_t reg_addr , uint8_t* buf);

#endif  //_BSP_I2C_H_

