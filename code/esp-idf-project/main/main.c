#include <stdio.h>
#include "moon_lolita_board_v1_support.h"

void app_main(void)
{
    bsp_i2c_init();
    lv_support_init();
    printf("helloWorld!"); 
}
