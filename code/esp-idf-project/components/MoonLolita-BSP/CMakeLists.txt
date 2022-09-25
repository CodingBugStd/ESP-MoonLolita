set( BOARD_SUPPORT_SRC_DIRS
    "."
    "peripheral/bsp_i2c"
    "peripheral/bsp_sdcard"
    "peripheral/bsp_usb_cdc"
    "peripheral/bsp_bt"
    "peripheral/bsp_wifi"
    "peripheral/bsp_nvs"
    "device/aw9523b"
    "device/es8388"
    "device/led"
    "device/lvgl_support"
    "device/mpu6050"
    "device/shtc3"
    "device/adc_keyboard"
)

set( BOARD_SUPPORT_INCLUDE_DIRS
    "."
)

idf_component_register(SRC_DIRS ${BOARD_SUPPORT_SRC_DIRS}
                    INCLUDE_DIRS ${BOARD_SUPPORT_INCLUDE_DIRS}
                    REQUIRES lvgl esp_lcd fatfs)
