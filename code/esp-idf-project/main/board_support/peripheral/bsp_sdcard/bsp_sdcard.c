#include "bsp_sdcard.h"
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "esp_log.h"

#define TAG "bsp_sdcard"

#define MOUNT_POINT "/sdcard"
#define PIN_SLK     0
#define PIN_CMD     0
#define PIN_D0      0
#define PIN_D1      0
#define PIN_D2      0
#define PIN_D3      0

static sdmmc_card_t *card = NULL;

void bsp_sdcard_init(){
    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true, //如果SD卡没有装载fatfs文件系统就格式化SD卡为fatfs系统
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initializing SD card");

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.max_freq_khz = 40000;

    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

    slot_config.width = 4;
    slot_config.clk = PIN_SLK;
    slot_config.cmd = PIN_CMD;
    slot_config.d0 = PIN_D0;
    slot_config.d1 = PIN_D1;
    slot_config.d2 = PIN_D2;
    slot_config.d3 = PIN_D3;

    slot_config.flags |= SDMMC_SLOT_FLAG_INTERNAL_PULLUP;

    ESP_LOGI(TAG, "Mounting filesystem");
    ret = esp_vfs_fat_sdmmc_mount(mount_point, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        ESP_LOGI(TAG,"sdcard init fail");
        return;
    }

    ESP_LOGI(TAG, "Filesystem mounted");

    sdmmc_card_print_info(stdout, card);
}


