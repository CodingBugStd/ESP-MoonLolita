# ESP-MoonLolita

​	本项目是本人大四实习期间用于练手的物联网项目。

​	ESP-MoonLolita是一块使用esp-idf开发、使用ESP32S3模组做为主控的DIY桌面摆件。
​	ESP-MoonLolita具备通过wifi连接私有或公有云的能力，能访问互联网，获取诸如关注量、天气、邮件的能力。ESP-MoonLolita使用一块320x240的电容触摸屏幕、搭载3000mAh的锂电池，使用Type-C接口充电，能支持低功耗休眠以及摇晃唤醒。Type-C接口可以做为USB-Slave接口与计算机使用CDC协议通讯，可以使用上位机完成配网、OTA、控制业务逻辑等功能。