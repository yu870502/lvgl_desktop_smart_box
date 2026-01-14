#pragma once

#include "esp_log.h"

#define PRINT_FILE_LINE()    ESP_LOGI(TAG, "file:%s func:%s line:%d", __FILE__, __func__, __LINE__)
