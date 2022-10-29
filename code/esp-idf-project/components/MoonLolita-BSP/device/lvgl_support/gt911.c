#include "gt911.h"
#include "./moon_lolita_board_v1_support.h"
#include "string.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "gt911"

#define GT911_ADDR	0xBA>>1

//高低字节已经倒置! -> 因为地址要使用强制类型转化和memcpy，大端模式下会导致高低字节倒置
#define GT911_CMD						0x4080
#define GT911_CONFIG_VERSION_REG_ADDR	0x4780
#define GT911_PRODUCT_ID1_REG_ADDR		0x4081
#define GT911_TOUCH_NUM_REG_ADDR		0x4E81
#define GT911_POINT_1_REG_START_ADDR	0x5081

//共183个寄存器
#define GT911_CONFIG {\
	0x35, 0x10, 0x01, 0x40, 0x01,  0x01, 0x00, 0x20, \
	0x01, 0x00, 0x28, 0x05, 0x50,  0x3C, 0x0F, 0x05,\
    0x00, 0x00, 0x00, 0x00, 0x00,  0x60, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x89, 0x2A,  0x0B, 0x2D, 0x2B,\
	0x0F, 0x0A, 0x00, 0x00, 0x01,  0xA9, 0x03, 0x2D,\
	0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,\
    0x00, 0x00, 0x00, 0x21, 0x59, 0x94, 0xC5, 0x02,\
	0x07, 0x00, 0x00, 0x04, 0x93, 0x24, 0x00, 0x7D,\
	0x2C, 0x00, 0x6B, 0x36, 0x00, 0x5D, 0x42, 0x00, \
	0x53, 0x50, 0x00, 0x53, 0x00, 0x00, 0x00, 0x00, \
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
	0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x10,\
	0x12, 0x14, 0x16, 0xFF, 0xFF, 0xFF, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, \
	0x04, 0x06, 0x08, 0x0A, 0x0F, 0x10, 0x12, 0x16,\
	0x18, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22,\
	0x24, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, \
	0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00, \
    }

#define GT911_RST_PIN_NUM   41
//#define GT911_INT_PIN_NUM   0 //直接接地 放弃中断

//reg_addr : 需要高低字节倒置
static esp_err_t gt911_read_register(uint16_t start_addr,uint8_t *buf,uint8_t len){
	return i2c_write_read( GT911_ADDR , &start_addr , 2 , buf , len );
}

//reg_addr : 需要高低字节倒置
static esp_err_t gt911_write_regiser(uint16_t start_addr,uint8_t *buf , uint8_t len){
	uint8_t send_buf[ len + 2 ];
	*((uint16_t*)send_buf) = start_addr;
	memcpy( send_buf + 2 , buf , len );
	return i2c_write( GT911_ADDR , send_buf , len + 2 );
}

static void gt911_soft_reset(){
	gt911_write_regiser(GT911_CMD  , (uint8_t*)"\x02" , 1 );
}

static void gt911_hard_reset(){
	ESP_LOGI(TAG,"gt911 init");
    gpio_set_direction( GT911_RST_PIN_NUM , GPIO_MODE_OUTPUT );
    gpio_set_level( GT911_RST_PIN_NUM , 0 );
    vTaskDelay(1);
    gpio_set_level( GT911_RST_PIN_NUM , 1 );
    vTaskDelay(1);
}

//因为硬件设计时发生了短路，导致x坐标不正常，用这个函数修正
static uint16_t gt911_x_correct(uint16_t x){
	return 240 - x;
}

//因为硬件设计时发生了短路，导致y坐标不正常，用这个函数修正
//0~58 133~201  58:68 -> 0.853 
static uint16_t gt911_y_correct(uint16_t y){
	float temp;
	if( y < 59 ){
		y *= 2.54;
	}else{
		y = 259 - y ;
		y *= 2.54;
	}
	return y;
}

bool gt911_get_pos(uint16_t *x , uint16_t *y){
	esp_err_t ret = ESP_FAIL;
	uint8_t buf[4];
	ret = gt911_read_register( GT911_TOUCH_NUM_REG_ADDR , buf , 1 );
	//清除0x814E
	gt911_write_regiser( GT911_TOUCH_NUM_REG_ADDR , (uint8_t*)"\0" , 1 );
	if( ret != ESP_OK ){
		ESP_LOGE(TAG,"error ! can't read point num!");
		return false;
	}else{
		buf[0] = buf[0] & 0x0F;
		if( buf[0] ){
			ret = gt911_read_register( GT911_POINT_1_REG_START_ADDR , buf , 4 );
			if( ret != ESP_OK ){
				ESP_LOGE(TAG,"error ! can't read point posistion");
				return false;
			}
			*x = buf[0];
			*x |= (uint16_t)buf[1]<<8;
			*y = buf[2];
			*y |= (uint16_t)buf[3]<<8;
			*x = gt911_x_correct(*x);
			*y = gt911_y_correct(*y);
			return true;
		}
		return false;
	}
}

esp_err_t gt911_printf_info(){
	esp_err_t ret = ESP_FAIL;

	uint8_t buf[5];
	ret = gt911_read_register( GT911_PRODUCT_ID1_REG_ADDR , buf  , 4 );
	buf[4] = '/0';
	if( ret != ESP_OK ){
		ESP_LOGE(TAG,"product id read error!");
		return ret;
	}
	ESP_LOGI(TAG,"product id : %s ",(char*)buf);

	ret = gt911_read_register( GT911_CONFIG_VERSION_REG_ADDR , buf , 1 );
	if( ret != ESP_OK ){
		ESP_LOGE(TAG,"config version read error!");
		return ret;
	}
	ESP_LOGI( TAG , "config version : 0x%02X" , buf[0] );

    return ESP_OK;
}

static void gt911_config(){
	uint8_t buf[ 184 + 2 ] = GT911_CONFIG;
	buf[185] = 0;	//0x8100 写入1 , 配置已更新标志
	buf[184] = 0;	//0x80FF 配置信息校验和
	for( uint8_t temp = 0; temp < 184 ;temp++){
		buf[184] += buf[temp];
	}
	buf[184] = (~buf[184]) + 1;
	ESP_LOGI(TAG,"config , version : 0x%02X , check sum :0x%2X" , buf[0] , buf[184] );
	gt911_write_regiser( GT911_CONFIG_VERSION_REG_ADDR , buf , 186 );
}

void gt911_init(){
	bsp_i2c_init();
	gt911_hard_reset();
	vTaskDelay( 10 / portTICK_PERIOD_MS );
	gt911_soft_reset();
	vTaskDelay( 10 / portTICK_PERIOD_MS );
	gt911_config();
}

