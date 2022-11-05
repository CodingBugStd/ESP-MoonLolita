/*
 * @Author: 没人要的野指针
 * @Date: 2022-10-06 14:11:31
 * @LastEditors: 没人要的野指针
 * @LastEditTime: 2022-11-05 20:04:38
 * @Description: MoonLolita_v1 板级支持包头文件
 * Copyright (c) 2022 CodingBugStd, All Rights Reserved. 
 */
#ifndef _MOON_LOLITA_BOARD_SUPPORT_H_
#define _MOON_LOLITA_BOARD_SUPPORT_H_

#ifdef __cplusplus
extern "C"{ 
#endif

#include "./peripheral/bsp_i2c/bsp_i2c.h"
#include "./peripheral/bsp_sdcard/bsp_sdcard.h"
#include "./peripheral/bsp_nvs/bsp_nvs.h"
#include "./peripheral/bsp_usb_cdc/bsp_usb_cdc.h"
#include "./peripheral/bsp_ledc/bsp_ledc.h"
#include "./peripheral/bsp_wifi/bsp_wifi.h"

#include "./device/aw9523b/aw9523b.h"
#include "./device/es8388/es8388.h"
#include "./device/led/led.h"
#include "./device/lvgl_support/lvgl_support.h"
#include "./device/mpu6050/mpu6050.h"
#include "./device/shtc3/shtc3.h"

#ifdef __cplusplus
}
#endif

#endif  //_MOON_LOLITA_BOARD_SUPPORT_H_

