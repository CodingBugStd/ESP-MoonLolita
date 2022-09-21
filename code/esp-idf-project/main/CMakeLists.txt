#main组件板级支持包源文件
set( BOARD_SUPPORT_SRC_DIRS
    "board_support/peripheral/bsp_i2c"
    "board_support/peripheral/bsp_usb_cdc"
    "board_support/peripheral/bsp_bt"
    "board_support/peripheral/bsp_wifi"
    "board_support/device/aw9523b"
    "board_support/device/es8388"
    "board_support/device/led"
    "board_support/device/lvgl_support"
    "board_support/device/mpu6050"
    "board_support/device/shtc3"
    "board_support/device/adc_keyboard"
)
#main组件板级支持包头文件路径
set( BOARD_SUPPORT_INCLUDE_DIRS
    "board_support"
)

#main组件Lolita框架服务源文件

#main组件Lolita框架服务头文件目录

#main组件Lolita框架应用源文件

#main组件Lolita框架应用头文件目录

idf_component_register(SRC_DIRS     "."
                                    ${BOARD_SUPPORT_SRC_DIRS}
                    INCLUDE_DIRS    "." 
                                    ${BOARD_SUPPORT_INCLUDE_DIRS})