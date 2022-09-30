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

### 1.3、MoonLolita-UI Components



## 2、LolitaAppFrame设计思路

​	LolitaAppFrame是为了更加快速、简单、便捷、规范的移植和开发嵌入式系统应用层所设计的一套应用层框架。

​	大四机电工程专业学生狗设计，不合理的地方轻点喷。QAQ！

### 2.1、BOOT

1. 调用llt_app_frame_init()初始化框架。
2. 通过llt_function_regist()注册所有能实现的function并且关联其对应的Service。
3. 通过LolitaAppFrame接口启动App。
4. App运行时会向LolitaAppFrame请求实现某些功能，AppFrame则会启动、管理相关服务以实现其请求的功能。

### 2.2、层级

1. App层
	顶层应用，通过LolitaAppFrame启动。上面仅存在用户逻辑，通过LolitaAppFrame去实现其具体功能。
2. Service层
	服务层，为App提供具体的功能实现。本层与App层通过LolitaAppFrame解耦。
3. HAL层
	硬件抽象层，为Service提供操作硬件的接口。