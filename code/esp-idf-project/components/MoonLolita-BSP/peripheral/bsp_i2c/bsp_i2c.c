#include "bsp_i2c.h"
#include "esp_log.h"

#define TAG "bsp_i2c"

#define I2C_MASTER_NUM      I2C_NUM_1
#define I2C_MASTER_SDA_IO   GPIO_NUM_39
#define I2C_MASTER_SCL_IO   GPIO_NUM_40
#define I2C_MASTER_FREQ_HZ  400*1000
#define I2C_TIMEOUT_MS      100

esp_err_t bsp_i2c_init(void)
{
    static esp_err_t isOK = ESP_FAIL;
    if( isOK == ESP_OK ){
        return isOK;
    }

    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    ESP_LOGI(TAG,"i2c init");

    i2c_param_config(i2c_master_port, &conf);

    isOK = i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
    return isOK;
}

esp_err_t i2c_write(uint8_t dev_addr , uint8_t* buf , size_t len){
    esp_err_t ret = i2c_master_write_to_device( I2C_MASTER_NUM , dev_addr , buf , len , I2C_TIMEOUT_MS / portTICK_RATE_MS );
    if( ret != ESP_OK){
        ESP_LOGE(TAG,"i2c write err ! dev addr:0x%02X" , dev_addr );
    }
    return ret;
}

esp_err_t i2c_read(uint8_t dev_addr , uint8_t* buf , size_t len){
    esp_err_t ret = i2c_master_read_from_device( I2C_MASTER_NUM , dev_addr , buf, len , I2C_TIMEOUT_MS / portTICK_RATE_MS );
    if( ret != ESP_OK){
        ESP_LOGE(TAG,"i2c read err ! dev addr:0x%02X" , dev_addr );
    }
    return ret;
}

esp_err_t i2c_write_read(uint8_t dev_addr , uint8_t*w_buf , size_t w_len , uint8_t*r_buf , size_t r_len){
    esp_err_t ret = i2c_master_write_read_device( I2C_MASTER_NUM, dev_addr, w_buf, w_len , r_buf, r_len, I2C_TIMEOUT_MS / portTICK_RATE_MS);
    if( ret != ESP_OK){
        ESP_LOGE(TAG,"i2c write read err ! dev addr:0x%02X" , dev_addr );
    }
    return ret;
}

esp_err_t i2c_write_reg8(uint8_t dev_addr , uint8_t reg_addr , uint8_t dat){
    uint8_t temp[2] = { reg_addr , dat };
    return i2c_write(dev_addr , temp , 2);
}

esp_err_t i2c_read_reg8(uint8_t dev_addr , uint8_t reg_addr , uint8_t* buf){
    return i2c_write_read(dev_addr , &reg_addr , 1 , buf , 1);
}
