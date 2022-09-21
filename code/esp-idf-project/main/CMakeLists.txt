#main组件板级支持包源文件
set( BOARD_SUPPORT_SRC_DIRS
    "board_support/aw9523b"
    "board_support/bit_io"
    "board_support/es8388"
    "board_support/led"
    "board_support/lv_support"
    "board_support/mpu6050"
    "board_support/shtc3"
)
#main组件板级支持包头文件路径
set( BOARD_SUPPORT_INCLUDE_DIRS
    "board_support"
)

idf_component_register(SRC_DIRS     "."
                                    ${BOARD_SUPPORT_SRC_DIRS}
                    INCLUDE_DIRS    "." 
                                    ${BOARD_SUPPORT_INCLUDE_DIRS})