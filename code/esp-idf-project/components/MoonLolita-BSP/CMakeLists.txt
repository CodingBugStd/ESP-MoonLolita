set( BOARD_SUPPORT_SRC_DIRS
    "."
    "peripheral/bsp_i2c"
    "peripheral/bsp_sdcard"
    "peripheral/bsp_usb_cdc"
    "peripheral/bsp_bt"
    "peripheral/bsp_wifi"
    "peripheral/bsp_nvs"
    "peripheral/bsp_ledc"
    "peripheral/bsp_i2s"
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

set(
    COMPONENT_REQUIRES
    lvgl
    esp_lcd
    fatfs
    tinyusb
    nvs_flash
    esp_wifi
    esp_event
    lwip
)

idf_component_register(SRC_DIRS ${BOARD_SUPPORT_SRC_DIRS}
                    INCLUDE_DIRS ${BOARD_SUPPORT_INCLUDE_DIRS}
                    REQUIRES ${COMPONENT_REQUIRES})
