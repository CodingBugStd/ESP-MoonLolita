# ESP-MoonLolita

​	基于esp-idf开发的一块能联网的多功能桌面摆件。

[TOC]



## 1、组件(components)

- LolitaAppFrame : 应用层框架
- MoonLolita-BSP : 板级支持包
- MoonLolita-Ui : 用户界面

- lvgl : 开源GUI

### 1.1、LolitaAppFrame Components

### 1.2、MoonLolita-BSP Components

**片上外设:**

> peripheral/bsp_bt : 蓝牙，用于辅助配网以及数据传输等。
>
> peripheral/bsp_i2c : i2c总线，用于操作总线上的i2c器件。
>
> peripheral/bsp_nvs : nvs存储，用于保存重要设置到片内flash中。
>
> periphera/bsp_sdcard : sd卡驱动，挂载sd卡到vfs上，用于存储大块数据、图片、音频以及.bin文件。
>
> periphera/bsp_usb_cdc : usb驱动，用于与上位机通讯、自定义usb升级。
>
> peripheral/bsp_wifi : wifi，联网、ota升级、mqtt通讯、对接网络API接口。

**外围器件:**

> device/adc_keyboard : ADC键盘驱动。
>
> device/aw9523b : 数组io口拓展芯片。
>
> device/es8388 : 音频解码。
>
> device/led : RGB呼吸灯控制。
>
> device/lvgl_support : idf原生lcd和TP的驱动，用于驱动触摸屏以及对接LVGL。
>
> device/mpu6050 : 陀螺仪，摇晃唤醒以及姿态解算。
>
> device/shtc3 : 温湿度传感器。

### 1.3、MoonLolita-Ui Components



